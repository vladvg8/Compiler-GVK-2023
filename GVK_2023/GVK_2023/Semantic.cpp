#include "Semantic.h"
#include "Error.h"
#pragma warning(disable : 1041)
namespace SA {

	void operands(MFST::LEX lex) {
		for (int i = 0; i < lex.lextable.size; i++) {
			
			if (lex.lextable.table[i].lexema == LEX_MORE || lex.lextable.table[i].lexema == LEX_ASSIGNMENT)
			{
				if (lex.lextable.table[i].lexema == ':') {
					int cur = -1;
					IT::IDDATATYPE datatype = (IT::IDDATATYPE)0;
					while (lex.lextable.table[i + cur].lexema != LEX_SEMICOLON) {
						if (lex.lextable.table[i + cur].lexema == LEX_ID || lex.lextable.table[i + cur].lexema == LEX_LITERAL)
						{
							if (datatype == (IT::IDDATATYPE)0) {
								datatype = lex.idtable.table[lex.lextable.table[i + cur].idxTI].iddatatype;
							}
							else {
						
								if (datatype != lex.idtable.table[lex.lextable.table[i + cur].idxTI].iddatatype) {
									throw ERROR_THROW_IN(703, lex.lextable.table[i + cur].sn, lex.lextable.table[i + cur].cn)
								}
							}
							if (lex.idtable.table[lex.lextable.table[i + cur].idxTI].idtype == IT::FUNCTION) {
								while (lex.lextable.table[i + cur].lexema != LEX_RIGHTTHESIS) {
									cur++;
								}
							}
						}
						if (datatype == IT::TEXT && lex.lextable.table[i + cur].lexema == LEX_MORE && cur != 0) {
							throw ERROR_THROW_IN(704, lex.lextable.table[i + cur].sn, lex.lextable.table[i + cur].cn)
						}
						if (datatype == IT::SYMBOL && lex.lextable.table[i + cur].lexema == LEX_MORE && cur != 0 && lex.lextable.table[i + cur].data[0] != '+' && lex.lextable.table[i + cur].data[0] != '-') {
							throw ERROR_THROW_IN(704, lex.lextable.table[i + cur].sn, lex.lextable.table[i + cur].cn)
						}
						if (datatype == IT::BOOLEAN && lex.lextable.table[i + cur].lexema == LEX_MORE && cur != 0) {
							throw ERROR_THROW_IN(704, lex.lextable.table[i + cur].sn, lex.lextable.table[i + cur].cn)
						}
						cur++;
					}
					i += cur - 1;
				}
			}
			if (lex.lextable.table[i].lexema == 'b') {
				if (lex.idtable.table[lex.lextable.table[i - 1].idxTI].iddatatype != lex.idtable.table[lex.lextable.table[i + 1].idxTI].iddatatype) {
					throw ERROR_THROW_IN(703, lex.lextable.table[i].sn, lex.lextable.table[i].cn)
				}
			}
		}
		for (int i = 0; i < lex.idtable.size; i++) {
			if (lex.idtable.table[i].iddatatype == IT::HALLOW && lex.idtable.table[i].idtype != IT::FUNCTION) {
				throw ERROR_THROW_IN(708, lex.idtable.table[i].idxfirstLE, lex.idtable.table[i].idxfirstLE);
			}
		}
	}

	void functions(MFST::LEX lex) {
		for (int i = 0; i < lex.lextable.size; i++) {
			if (lex.lextable.table[i].lexema == LEX_DISPLAY) {
				if (lex.lextable.table[i + 2].lexema == ')') {
					throw ERROR_THROW_IN(714, lex.lextable.table[i + 2].sn, lex.lextable.table[i + 2].cn);
				}
				IT::IDTYPE re = lex.idtable.table[lex.lextable.table[i + 2].idxTI].idtype;
				IT::IDDATATYPE ree = lex.idtable.table[lex.lextable.table[i + 2].idxTI].iddatatype;
				if (lex.idtable.table[lex.lextable.table[i + 2].idxTI].idtype == IT::FUNCTION && lex.idtable.table[lex.lextable.table[i + 2].idxTI].iddatatype == IT::HALLOW)
				{
					throw ERROR_THROW_IN(711, lex.lextable.table[i + 2].sn, lex.lextable.table[i + 2].cn);
				}
			}
			
			if (lex.lextable.table[i].lexema == LEX_ID && lex.lextable.table[i - 1].lexema == LEX_FUNCTION && lex.idtable.table[lex.lextable.table[i].idxTI].idtype == IT::FUNCTION)
			{
				int cur = 1;
				IT::IDDATATYPE returnType = lex.idtable.table[lex.lextable.table[i].idxTI].iddatatype;
				if (returnType == IT::HALLOW) {
					while (i + cur < lex.lextable.size && lex.lextable.table[i + cur].lexema != LEX_RETURN) {
						cur++;
					}
					if (i + cur < lex.lextable.size && lex.lextable.table[i + cur].lexema == LEX_RETURN) {
						throw ERROR_THROW_IN(709, lex.lextable.table[i + cur].sn, lex.lextable.table[i + cur].cn);
					}
				}
				else {
					while (i + cur < lex.lextable.size && lex.lextable.table[i + cur].lexema != LEX_RETURN) {
						cur++;
					}
					if (i + cur == lex.lextable.size) {
						throw ERROR_THROW_IN(700, lex.lextable.table[i].sn, lex.lextable.table[i].cn);
					}
					if (i + cur < lex.lextable.size && (lex.lextable.table[i + cur + 1].lexema == LEX_ID || lex.lextable.table[i + cur + 1].lexema == LEX_LITERAL)
						&& lex.idtable.table[lex.lextable.table[i + cur + 1].idxTI].idtype != IT::FUNCTION
						&& lex.idtable.table[lex.lextable.table[i + cur + 1].idxTI].iddatatype != returnType) {
						throw ERROR_THROW_IN(700, lex.lextable.table[i + cur].sn, lex.lextable.table[i + cur].cn);
					}
				}
			}
		}
		for (int i = 0; i < lex.lextable.size; i++) {
			if (lex.lextable.table[i].lexema == LEX_ID && lex.idtable.table[lex.lextable.table[i].idxTI].idtype == IT::FUNCTION && lex.lextable.table[i - 1].lexema == LEX_FUNCTION) {
				IT::IDDATATYPE* ids = new IT::IDDATATYPE[16];
				int idsSize = 0;
				int funcPos = lex.idtable.table[lex.lextable.table[i].idxTI].idxfirstLE;
				while (lex.lextable.table[funcPos + 1].lexema != LEX_RIGHTTHESIS)
				{
					if (lex.lextable.table[funcPos + 1].lexema == LEX_ID || lex.lextable.table[funcPos + 1].lexema == LEX_LITERAL) {
						ids[idsSize] = lex.idtable.table[lex.lextable.table[funcPos + 1].idxTI].iddatatype;
						idsSize++;
					}
					funcPos++;
					if (idsSize == 16) {
						throw ERROR_THROW_IN(705, lex.lextable.table[i].sn, lex.lextable.table[i].cn);
					}
				}
			}
			if (lex.lextable.table[i].lexema == LEX_ID && lex.idtable.table[lex.lextable.table[i].idxTI].idtype == IT::STATIC_FUNCTION) {
				int cur = 2;
				int numberOfParams = 0;
				while (lex.lextable.table[i + cur].lexema != LEX_RIGHTTHESIS) {
					if (lex.lextable.table[i + cur].lexema == LEX_ID || lex.lextable.table[i + cur].lexema == LEX_LITERAL) {
						if (lex.idtable.table[lex.lextable.table[i + cur].idxTI].iddatatype == IT::BYTE) {
							numberOfParams++;
						}
						else {
							throw ERROR_THROW_IN(713, lex.lextable.table[i].sn, lex.lextable.table[i].cn);
						}

					}
					cur++;
				}
				if (numberOfParams != 2) {
					throw ERROR_THROW_IN(713, lex.lextable.table[i].sn, lex.lextable.table[i].cn);
				}
			}
			if (lex.lextable.table[i].lexema == LEX_ID && lex.idtable.table[lex.lextable.table[i].idxTI].idtype == IT::FUNCTION && lex.lextable.table[i - 1].lexema != LEX_FUNCTION)
			{
				IT::IDDATATYPE* ids = new IT::IDDATATYPE[16];
				int idsSize = 0;
				int funcPos = lex.idtable.table[lex.lextable.table[i].idxTI].idxfirstLE;
				if (lex.lextable.table[i + 1].lexema != LEX_LEFTTHESIS) {
					throw ERROR_THROW_IN(706, lex.lextable.table[i].sn, lex.lextable.table[i].cn)
				}
				while (lex.lextable.table[funcPos + 1].lexema != LEX_RIGHTTHESIS)
				{
					if (lex.lextable.table[funcPos + 1].lexema == LEX_ID || lex.lextable.table[funcPos + 1].lexema == LEX_LITERAL) {
						ids[idsSize] = lex.idtable.table[lex.lextable.table[funcPos + 1].idxTI].iddatatype;
						idsSize++;
					}
					funcPos++;
					if (idsSize == 16) {
						throw ERROR_THROW_IN(705, lex.lextable.table[i].sn, lex.lextable.table[i].cn);
					}
				}
				int cur = 1;
				int paramCount = 0;
				while (lex.lextable.table[i + cur].lexema != LEX_RIGHTTHESIS) {
					if (lex.lextable.table[i + cur].lexema == LEX_ID || lex.lextable.table[i + cur].lexema == LEX_LITERAL) {
						if (lex.idtable.table[lex.lextable.table[i + cur].idxTI].iddatatype != ids[paramCount]) {
							throw ERROR_THROW_IN(702, lex.lextable.table[i + cur].sn, lex.lextable.table[i + cur].cn)
						}
						paramCount++;
					}
					cur++;
				}
				if (paramCount != idsSize) {
					throw ERROR_THROW_IN(701, lex.lextable.table[i + cur].sn, lex.lextable.table[i + cur].cn)
				}
				i += cur;
				delete[] ids;
			}
		}
		for (int i = 0; i < lex.lextable.size; i++) {
			if (lex.lextable.table[i].lexema == LEX_ASSIGNMENT && lex.lextable.table[i - 1].lexema == LEX_ID &&
				lex.lextable.table[i + 1].lexema == LEX_ID && lex.idtable.table[lex.lextable.table[i + 1].idxTI].idtype == IT::FUNCTION) {

				int funcIdx = lex.lextable.table[i + 1].idxTI;
				IT::IDDATATYPE returnType = lex.idtable.table[funcIdx].iddatatype;

				int destIdx = lex.lextable.table[i - 1].idxTI;
				IT::IDDATATYPE destType = lex.idtable.table[destIdx].iddatatype;

				if (returnType != destType) {
					throw ERROR_THROW_IN(707, lex.lextable.table[i].sn, lex.lextable.table[i].cn);
				}
			}
		}
	}

	void literals(MFST::LEX lex) {
		for (int i = 0; i < lex.idtable.size; i++) {
			if (lex.idtable.table[i].idtype == IT::LITERAL && lex.idtable.table[i].iddatatype == IT::BYTE) {
				long value = atoi(lex.idtable.table[i].id);
				if (value > 127 || value < -128) {
					throw ERROR_THROW_IN(712, lex.lextable.table[lex.idtable.table[i].idxfirstLE].sn, lex.lextable.table[lex.idtable.table[i].idxfirstLE].cn);
				}
			}
		}
	}

	void ifs(MFST::LEX lex) {
		for (int i = 0; i < lex.lextable.size; i++) {
			if (lex.lextable.table[i].lexema == LEX_IF) {
				int cur = 2;
				int counter = 0;
				IT::IDDATATYPE idDataTypes[2]{};
				while (lex.lextable.table[i + cur].lexema != LEX_RIGHTTHESIS) {
					if (lex.lextable.table[i + cur].lexema == LEX_ID || lex.lextable.table[i + cur].lexema == LEX_LITERAL) {
						idDataTypes[counter] = lex.idtable.table[lex.lextable.table[i + cur].idxTI].iddatatype;
						counter++;
					}
					cur++;
				}
				if (counter == 1) {
					if (lex.idtable.table[lex.lextable.table[i + cur - 1].idxTI].iddatatype != IT::BOOLEAN) {
						throw ERROR_THROW_IN(710, lex.lextable.table[i].sn, lex.lextable.table[i].cn);
					}
				}
				else {
					if (idDataTypes[0] != idDataTypes[1] || (idDataTypes[0] == IT::BOOLEAN || idDataTypes[1] == IT::BOOLEAN) || (idDataTypes[0] == IT::TEXT || idDataTypes[1] == IT::TEXT)) {
						throw ERROR_THROW_IN(710, lex.lextable.table[i].sn, lex.lextable.table[i].cn);
					}
				}
			}
		}
		for (int i = 0; i < lex.idtable.size; i++) {
			if (lex.idtable.table[i].iddatatype == IT::HALLOW && lex.idtable.table[i].idtype != IT::FUNCTION) {
				throw ERROR_THROW_IN(708, lex.idtable.table[i].idxfirstLE, lex.idtable.table[i].idxfirstLE);
			}
		}
		
	}

	bool startSA(MFST::LEX lex) {
		functions(lex);
		operands(lex);
		literals(lex);
		ifs(lex);
		return true;
	};

}
