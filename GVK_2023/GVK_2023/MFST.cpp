#include <iomanip>
#include <iostream>
#include "MFST.h"

namespace MFST {
	int counter = 0;
	int saveCounter = 1;

	Mfst::MfstDiagnosis::MfstDiagnosis() {
		this->lenta_position = -1;
		this->rc_step = SURPRISE;
		this->nrule = -1;
		this->nrule_chain = -1;

	}
	Mfst::MfstDiagnosis::MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain) {
		this->lenta_position = plenta_position;
		this->rc_step = prc_step;
		this->nrule = pnrule;
		this->nrule_chain = pnrule_chain;
	}

	MfstState::MfstState() {
		this->lenta_position = 0;
		this->nrule = -1;
		this->nrulechain = -1;
	}

	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrulechain) {
		this->lenta_position = pposition;
		this->st = pst;
		this->nrulechain = pnrulechain;
	}

	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain) {
		this->lenta_position = pposition;
		this->st = pst;
		this->nrule = pnrule;
		this->nrulechain = pnrulechain;
	}

	Mfst::Mfst() {
		this->lenta = 0;
		this->lenta_size = 0;
		this->lenta_position = 0;
	}

	Mfst::Mfst(MFST::LEX plex, GRB::Greibach pgrebach, Log::LOG& plog) {
		this->lex = plex;
		this->greibach = pgrebach;
		this->log = plog;
		this->lenta = new short[plex.lextable.size];
		for (int i = 0; i < plex.lextable.size; i++) {
			this->lenta[i] = GRB::Rule::Chain::T(plex.lextable.table[i].lexema);
		}
		this->lenta_size = plex.lextable.size;
		this->lenta_position = 0;
		this->nrulechain = -1;
		this->st.push(greibach.stbottomT);
		this->st.push(greibach.startN);
	}

	char* Mfst::getCSt(char* buf) {
		MFSTSTSTACK pst;
		int size = st.size();
		for (int i = 0; i < size; i++) {
			pst.push(st.top());
			st.pop();
			buf[i] = GRB::Rule::Chain::alphabet_to_char(pst.top());
		}
		for (int i = 0; i < size; i++) {
			st.push(pst.top());
			pst.pop();
		}
		buf[size] = 0x00;
		return buf;
	}

	char* Mfst::getCLenta(char* buf, short pos, short n) {
		int i, k;
		if (pos + n < lenta_size) {
			k = pos + n;
		}
		else {
			k = lenta_size;
		}
		for (i = pos; i < k; i++) {
			buf[i - pos] = GRB::Rule::Chain::alphabet_to_char(lenta[i]);
		}
		buf[i - pos] = 0x00;
		return buf;
	}

	char* Mfst::getDiagnosis(short n, char* buf) {
		char* str = (char*)"";
		if (n < MFST_DIAGN_NUMBER && diagnosis[n].lenta_position > 0) {
			Error::ERROR error = Error::geterror(greibach.getRule(diagnosis[n].nrule).idError);
			sprintf_s(buf, ERROR_MAXSIZE_MESSAGE, "%d: строка %d,%s", error.id, lex.lextable.table[diagnosis[n].lenta_position].sn, error.message);
			str = buf;
		}
		return str;
	}

	bool Mfst::savestate() {
		storestate.push(MfstState(lenta_position, st, nrule, nrulechain));
		return true;
	}

	bool Mfst::reststate() {
		MfstState state;
		if (storestate.size() > 0) {
			state = storestate.top();
			lenta_position = state.lenta_position;
			st = state.st;
			nrulechain = state.nrulechain;
			storestate.pop();
			return true;
		}
		return false;
	}

	bool Mfst::push_chain(GRB::Rule::Chain chain) {
		for (int i = 0; i < chain.size; i++) {
			st.push(chain.nt[chain.size - 1 - i]);
		};
		return true;
	}

	Mfst::RC_STEP Mfst::step() {
		char* buff = new char[200];
		RC_STEP state = SURPRISE;
		*log.stream << std::endl;
		(*log.stream) << std::setw(4) << std::left << counter << ": ";
		if (lenta_position < lenta_size) {
			if (GRB::Rule::Chain::isN(st.top())) {
				GRB::Rule rule;
				nrule = greibach.getRule(st.top(), rule);
				if (nrule >= 0) {
					GRB::Rule::Chain chain;
					nrulechain = rule.getNextChain(lenta[lenta_position], chain, nrulechain + 1);
					if (nrulechain >= 0) {
						char* buff = new char[200];
						rule.getCRule(buff, nrulechain);
						*log.stream << std::setw(20) << std::left << buff;
						getCLenta(buff, lenta_position);
						*log.stream << std::setw(30) << std::left << buff;
						getCSt(buff);
						*log.stream << std::setw(20) << std::left << buff << std::endl;
						savestate();
						*log.stream << std::setw(4) << std::left << counter << ": " << std::setw(20) << std::left << "SAVESTATE:" << std::setw(30) << std::left << saveCounter++;
						st.pop();
						push_chain(chain);
						state = NS_OK;
					}
					else {
						savediagnosis(NS_NORULECHAIN);
						if (reststate()) {
							state = NS_NORULECHAIN;
							*log.stream << std::setw(20) << std::left << "TS_NOK/NS_NORULECHAIN" << std::endl << std::setw(4) << std::left << counter << ": " << std::setw(20) << std::left << "RESTSTATE";
							saveCounter--;
						}
						else {
							state = NS_NORULE;
						}
					}
				}
				else {
					state = NS_ERROR;
				}
			}
			else {
				if (st.top() == lenta[lenta_position]) {
					lenta_position++;
					st.pop();
					nrulechain = -1;
					state = TS_OK;
					char* buff = new char[200];
					*log.stream << std::setw(20) << std::left << "";
					getCLenta(buff, lenta_position);
					*log.stream << std::setw(30) << std::left << buff;
					getCSt(buff);
					*log.stream << std::setw(20) << std::left << buff;
					counter++;

				}
				else {
					if (reststate()) {
						state = TS_NOK;
						*log.stream << std::setw(20) << std::left << "TS_NOK/NS_NORULECHAIN" << std::endl << std::setw(4) << std::left << counter << ": " << std::setw(20) << std::left << "RESTSTATE";
						saveCounter--;
					}
					else {
						state = NS_NORULECHAIN;
						*log.stream << std::setw(20) << std::left << "TS_NOK/NS_NORULECHAIN" << std::endl << std::setw(4) << std::left << counter << ": " << std::setw(20) << std::left << "RESTSTATE";
						saveCounter--;
					}
				}
			}
		}
		else {
			state = LENTA_END;
			*log.stream << std::setw(20) << std::left << "LENTA_END";

		}
		return state;
	}

	bool Mfst::start() {
		*log.stream << std::endl << std::setw(4) << std::left << "Шаг" << ": " << std::setw(20) << std::left << "Правило" << std::setw(30) << std::left << "Входная лента" << std::setw(20) << std::left << "Стек" << std::endl;
		RC_STEP state = SURPRISE;
		char buff[200];
		state = step();
		while (state == NS_OK || state == NS_NORULECHAIN || state == TS_OK || state == TS_NOK) {
			state = step();
		}
		st.pop();
		if (!st.empty()) {
			state = NS_ERROR;
		}
		switch (state)
		{
		case MFST::Mfst::NS_NORULE: {
			*log.stream << std::setw(20) << std::left << "------>NO_RULE" << std::endl;
			*log.stream << "-------------------------------------------------------------------------------------" << std::endl;
			*log.stream << getDiagnosis(0, buff) << std::endl;
			*log.stream << getDiagnosis(1, buff) << std::endl;
			*log.stream << getDiagnosis(2, buff) << std::endl;
			return false;
			break;
		}
		case MFST::Mfst::NS_NORULECHAIN: {
			*log.stream << std::setw(20) << std::left << "------>NO_RULECHAIN";
			return false;
			break;
		}
		case MFST::Mfst::NS_ERROR: {
			*log.stream << std::setw(20) << std::left << "------>NS_ERROR";
			*log.stream << std::setw(20) << std::left << "\nОшибка 600. MFST: Неверная структура программы";
			std::cout << "Ошибка 600. MFST: Неверная структура программы" << std::endl;
			return false;
			break;
		}
		case MFST::Mfst::LENTA_END: {
			*log.stream << std::setw(4) << std::left << counter << std::setw(20) << std::left << "------>LENTA_END" << std::endl;
			*log.stream << "-------------------------------------------------------------------------------------" << std::endl;
			*log.stream << std::setw(4) << std::left << 0 << ": " << "Всего строк " << lenta_size << ", синтаксический анализ выполнен без ошибок";
			break;

		}
		case MFST::Mfst::SURPRISE: {
			*log.stream << std::setw(20) << std::left << "------>SURPRISE ";
			return false;
			break;
		}
		}
		return true;
	}

	bool Mfst::savediagnosis(RC_STEP pprc_step) {
		int i = 0;
		while (i < MFST_DIAGN_NUMBER && lenta_position <= diagnosis[i].lenta_position) {
			i++;
		}
		if (i < MFST_DIAGN_NUMBER) {
			diagnosis[i] = MfstDiagnosis(lenta_position, pprc_step, nrule, nrulechain);
			for (int j = i + 1; j < MFST_DIAGN_NUMBER; j++) {
				diagnosis[j].lenta_position = -1;
			}
			return true;
		}
		return false;
	}

	void Mfst::printrules() {
		int size = deducation.size;
		(*log.stream) << std::endl;
		for (unsigned int i = 0; i < size; i++) {
			char* buff = new char[200] {};
			GRB::Rule rule = greibach.getRule(deducation.nrules[i]);
			rule.getCRule(buff, deducation.nrulechains[i]);
			(*log.stream) << std::setw(4) << std::left << deducation.nsteps[i] << ": " << buff << std::endl;
		}
	}

	bool Mfst::savededucation()
	{
		MfstState state;
		std::stack<MfstState> st;
		GRB::Rule rule;
		deducation.size = (short)storestate.size();
		deducation.nsteps = new short[deducation.size];
		deducation.nrules = new short[deducation.size];
		deducation.nrulechains = new short[deducation.size];
		int size = storestate.size();
		for (unsigned short i = 0; i < size; i++) {
			st.push(storestate.top());
			storestate.pop();
		}
		for (unsigned short i = 0; i < size; i++)
		{
			state = st.top();
			st.pop();
			deducation.nsteps[i] = state.lenta_position;
			deducation.nrules[i] = state.nrule;
			deducation.nrulechains[i] = state.nrulechain;
		};
		return true;
	};
}