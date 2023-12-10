#pragma once
#include "Error.h"
typedef short GRBALPHABET;		//символы алфавита грамматики. “ерминалы > 0, нетерминалы < 0
#define NS(n)	GRB::Rule::Chain::N(n)
#define TS(n)	GRB::Rule::Chain::T(n)
#define ISNS(n) GRB::Rule::Chain::isN(n)

namespace GRB
{
	struct Rule					// правило в грамматике √рейбах
	{
		GRBALPHABET nn;			// нетерминал (левый символ правила) < 0
		int idError;			// идентификатор диагностического сообщени€
		short size;				// количество цепочек - правых частей правила

		struct Chain			// цепочка (права€ часть правила)
		{
			short size;			// длина цепочки
			GRBALPHABET* nt;	// цепочка терминалов (> 0) и нетерминалов (< 0)

			Chain() { size = 0; nt = new GRBALPHABET[1]{}; };
			Chain(
				short psize,				// количество символов в цепочке
				GRBALPHABET s, ...			// символы (терминал или не терминал)
			);

			char* getCChain(char* b);									// получить правую сторону правила
			static GRBALPHABET T(char t) { return GRBALPHABET(t); };	// терминал
			static GRBALPHABET N(char n) { return -GRBALPHABET(n); };	// не терминал
			static bool isT(GRBALPHABET s) { return s > 0; };			// терминал?
			static bool isN(GRBALPHABET s) { return !isT(s); };			// нетерминал?
			static char alphabet_to_char(GRBALPHABET s) { return isT(s) ? char(s) : char(-s); };	// GRBALPHABET->char
		}*chains;		// массив цепочек - правых частей правила

		Rule() { nn = 0x00, size = 0; };
		Rule(
			GRBALPHABET pnn,				// нетерминал (< 0)
			int iderror,					// идентификатор диагностического сообщени€ (Error)
			short psize,					// количество цепочек - правых частей правила
			Chain c, ...					// множество цепочек - правых частей правила
		);

		char* getCRule(						// получить правила в виде N->цепочка (дл€ распечатки)
			char* b,						// буфер
			short nchain					// номер цепочки (правой части) в правиле
		);
		short getNextChain(					// получить следующую за j подход€щую цепочку, вернуть ее номер или -1
			GRBALPHABET t,					// первый символ цепочки
			Chain& pchain,					// возвращаема€ цепочка
			short j							// номер цепочки
		);
	};

	struct Greibach						// грамматика √рейбах
	{
		short size;						// количество правил
		GRBALPHABET startN;				// стартовый символ
		GRBALPHABET stbottomT;			// дно стека
		Rule* rules;					// множество правил

		Greibach() { size = 0; startN = 0; stbottomT = 0; rules = 0; };
		Greibach(
			GRBALPHABET pstartN,		// стартовый символ
			GRBALPHABET pstbottomT,		// дно стека
			short psize,				// количество правил
			Rule r, ...					// правила
		);

		short getRule(					// получить правило, возвращаетс€ номер правила или -1
			GRBALPHABET pnn,			// левый символ правила
			Rule& prule					// возвращаемое правило грамматики
		);
		Rule getRule(short n);			// получить правило по номеру
	};

	Greibach getGreibach();				// получить грамматику
}