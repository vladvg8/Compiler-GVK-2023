#pragma once
#include <iostream>
#include "In.h"
#include "LT.h"
#include "IT.h"

namespace FST {
	struct RELATION { // ребро
		char symbol;
		short nnode; // смежная вершина
		RELATION(
			char c = 0x00,
			short ns = NULL
		);
	};

	struct NODE { // вершина (состояние)
		short n_relation; // количество инцидентных ребер
		RELATION* relations; // ребра
		NODE();
		NODE(
			short n,
			RELATION rel, ...
		);
	};

	struct FST {
		char* string; // цепочка
		short position; // текущая позиция в цепочке
		short nstates; // количество состояний
		NODE* nodes; // граф вереходов
		short* rstates; // возможные состояния
		FST(
			char* s,
			short ns,
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

	void Analyzer();

	void Analyze(In::IN in, LT::LexTable& lextable, IT::IdTable& idtable);
}