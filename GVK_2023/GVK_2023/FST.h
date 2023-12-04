#pragma once
#include <iostream>
#include "In.h"
#include "LT.h"
#include "IT.h"

namespace FST {
	struct RELATION { // ребро
		char symbol;
		unsigned short nnode; // смежная вершина
		RELATION(
			char c = 0x00,
			unsigned short ns = NULL
		);
	};

	struct NODE { // вершина (состояние)
		unsigned short n_relation; // количество инцидентных ребер
		RELATION* relations; // ребра
		NODE();
		NODE(
			unsigned short n,
			RELATION rel, ...
		);
	};

	struct FST {
		std::vector<char>string; // цепочка
		unsigned short position; // текущая позиция в цепочке
		unsigned short nstates; // количество состояний
		std::vector<NODE> nodes; // граф вереходов
		unsigned short* rstates; // возможные состояния
		FST(
			std::vector<char> s,
			unsigned short ns,
			NODE n, ...
		);
		void FSTreturn() {
			this->position = -1;
			this->rstates[0] = 0;
		}
	};

	struct FSTAssigned {
		FST* fst; // конечные автоматы
		IT::IDDATATYPE iddatatype; // тип
		char lex; // лексема
		FSTAssigned(FST* f, IT::IDDATATYPE i, char l) {
			fst = f;
			iddatatype = i;
			lex = l;
		}
	};

	bool execute(
		FST& fst
	);

	void Analyze(In::IN in, LT::LexTable& lextable, IT::IdTable& idtable);
}