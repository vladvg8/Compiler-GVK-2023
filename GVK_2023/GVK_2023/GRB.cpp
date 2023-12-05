#define _CRT_SECURE_NO_WARNINGS
#include "GRB.h"
#include <iostream>

typedef short GRBALPHABET;

#define GRB_ERROR_SERIES 600

using namespace std;

namespace GRB {
#define NS(n) Rule::Chain::N(n)
#define TS(n) Rule::Chain::T(n)	

	Greibach greibach(NS('S'), TS('$'),
		6,
		Rule(NS('S'), GRB_ERROR_SERIES + 0, // функции и main
			10,
			Rule::Chain(9, TS('d'), TS('t'), TS('f'), TS('i'), TS('('), TS(')'), TS('{'), TS('}'), NS('S')), // declare byte function pow(){} ...
			Rule::Chain(12, TS('d'), TS('t'), TS('f'), TS('i'), TS('('), TS(')'), TS('{'), TS('r'), NS('E'), TS(';'), TS('}'), NS('S')), // declare byte function pow(){return ... ;} ...
			Rule::Chain(10, TS('d'), TS('t'), TS('f'), TS('i'), TS('('), TS(')'), TS('{'), NS('N'), TS('}'), NS('S')), // declare byte function pow(){...} ...
			Rule::Chain(13, TS('d'), TS('t'), TS('f'), TS('i'), TS('('), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), NS('S')), // declare byte function pow(){... return ...;} ...

			Rule::Chain(9, TS('d'), TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), TS('}'), NS('S')), // declare byte function pow(...){} ...
			Rule::Chain(12, TS('d'), TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), TS('r'), NS('E'), TS(';'), TS('}'), NS('S')), // declare byte function pow(...){return ... ;} ...
			Rule::Chain(10, TS('d'), TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('}'), NS('S')), // declare byte function pow(...){...} ...
			Rule::Chain(13, TS('d'), TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), NS('S')), // declare byte function pow(...){... return ...;} ...

			Rule::Chain(3, TS('m'), TS('{'), TS('}')), // main {}
			Rule::Chain(4, TS('m'), TS('{'), NS('N'), TS('}')) // main {...}
		),
		Rule(NS('F'), GRB_ERROR_SERIES + 1, // параметр функции
			2,
			Rule::Chain(2, TS('t'), TS('i')), // byte number
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('F')) // byte number, ...
		),
		Rule(NS('N'), GRB_ERROR_SERIES + 2, // тело функции или какого-то блока
			24,
			// объявление переменных и работа их инициализацией
			Rule::Chain(6, TS('d'), TS('t'), TS('i'), TS(':'), TS('l'), TS(';')), // declare byte number: 123;
			Rule::Chain(7, TS('d'), TS('t'), TS('i'), TS(':'), TS('l'), TS(';'), NS('N')), // declare byte number: 123; ...
			Rule::Chain(4, TS('d'), TS('t'), TS('i'), TS(';')), // delcare byte number;
			Rule::Chain(5, TS('d'), TS('t'), TS('i'), TS(';'), NS('N')), // delcare byte number; ...
			Rule::Chain(4, TS('i'), TS(':'), TS('l'), TS(';')), // number: 123;
			Rule::Chain(5, TS('i'), TS(':'), TS('l'), TS(';'), NS('N')), // number: 123; ...
			Rule::Chain(5, TS('i'), TS(':'), TS('i'), NS('M'), TS(';')), // number: asd ...;
			Rule::Chain(6, TS('i'), TS(':'), TS('i'), NS('M'), TS(';'), NS('N')), // number: asd ...; ... 
			// вывод
			Rule::Chain(5, TS('p'), TS('('), TS('l'), TS(')'), TS(';')), // display("hello, WOrld!");
			Rule::Chain(6, TS('p'), TS('('), TS('l'), TS(')'), TS(';'), NS('N')), // display("hello, WOrld!"); ...
			Rule::Chain(5, TS('p'), TS('('), TS('i'), TS(')'), TS(';')), // display(number);
			Rule::Chain(6, TS('p'), TS('('), TS('i'), TS(')'), TS(';'), NS('N')), // display(number); ... 
			// условные конструкции
			Rule::Chain(7, TS('y'), TS('('), NS('Y'), TS(')'), TS('{'), NS('N'), TS('}')), // if (...) { ... }
			Rule::Chain(8, TS('y'), TS('('), NS('Y'), TS(')'), TS('{'), NS('N'), TS('}'), NS('N')), // if (...) { ... } ...
			Rule::Chain(6, TS('y'), TS('('), NS('Y'), TS(')'), TS('{'), TS('}')), // if (...) { }
			Rule::Chain(7, TS('y'), TS('('), NS('Y'), TS(')'), TS('{'), TS('}'), NS('N')), // if (...) { } ...
			Rule::Chain(11, TS('y'), TS('('), NS('Y'), TS(')'), TS('{'), NS('N'), TS('}'), TS('u'), TS('{'), NS('N'), TS('}')), // if (...) { ... } else { ... }
			Rule::Chain(12, TS('y'), TS('('), NS('Y'), TS(')'), TS('{'), NS('N'), TS('}'), TS('u'), TS('{'), NS('N'), TS('}'), NS('N')), // if (...) { ... } else { ... } ...
			Rule::Chain(10, TS('y'), TS('('), NS('Y'), TS(')'), TS('{'), TS('}'), TS('u'), TS('{'), NS('N'), TS('}')), // if (...) { } else { ... }
			Rule::Chain(11, TS('y'), TS('('), NS('Y'), TS(')'), TS('{'), TS('}'), TS('u'), TS('{'), NS('N'), TS('}'), NS('N')), // if (...) { } else { ... } ...
			Rule::Chain(10, TS('y'), TS('('), NS('Y'), TS(')'), TS('{'), NS('N'), TS('}'), TS('u'), TS('{'), TS('}')), // if (...) { ... } else { }
			Rule::Chain(11, TS('y'), TS('('), NS('Y'), TS(')'), TS('{'), NS('N'), TS('}'), TS('u'), TS('{'), TS('}'), NS('N')), // if (...) { ... } else { } ...
			Rule::Chain(9, TS('y'), TS('('), NS('Y'), TS(')'), TS('{'), TS('}'), TS('u'), TS('{'), TS('}')), // if (...) { } else { }
			Rule::Chain(10, TS('y'), TS('('), NS('Y'), TS(')'), TS('{'), TS('}'), TS('u'), TS('{'), TS('}'), NS('N')) // if (...) { } else { } ...

		),
		Rule(NS('Y'), GRB_ERROR_SERIES + 3, // тело if
			3,
			Rule::Chain(3, TS('i'), TS('k'), TS('i')),
			Rule::Chain(3, TS('i'), TS('k'), TS('l')),
			Rule::Chain(1, TS('l'))
		),
		Rule(NS('M'), GRB_ERROR_SERIES + 4, // математические операции
			2,
			Rule::Chain(2, TS('v'), TS('l')), // (+,/,*,-) 1;
			Rule::Chain(2, TS('v'), TS('i'))  // (+,/,*,-) number;
		),
		Rule(NS('E'), GRB_ERROR_SERIES + 5,
			2,
			Rule::Chain(1, TS('t')),
			Rule::Chain(1, TS('l'))
		)
		);



	Rule::Chain::Chain(short psize, GRBALPHABET s, ...) {
		int* p = (int*)&s;
		size = psize;
		nt = new GRBALPHABET[psize];
		for (int i = 0; i < psize; i++) {
			nt[i] = (short)p[i];
		}
	}

	char* Rule::Chain::getCChain(char* b) {
		for (int i = 0; i < size; i++) {
			b[i] = alphabet_to_char(this->nt[i]);
		}
		b[size] = 0;
		return b;
	}

	Rule::Rule(
		GRBALPHABET pnn,
		int iderror,
		short psize,
		Chain c, ...
	) {
		this->nn = pnn;
		this->iderror = iderror;
		this->size = psize;
		this->chains = new Chain[psize];
		Chain* p = &c;
		for (short i = 0; i < psize; i++) {
			this->chains[i] = p[i];
		}
	}

	char* Rule::getCRule(char* b, short nchain) {
		char* bchain = new char[this->chains[nchain].size];
		b[0] = Chain::alphabet_to_char(this->nn);
		b[1] = '-';
		b[2] = '>';
		b[3] = 0;
		this->chains[nchain].getCChain(bchain);
		strcat(b, bchain);
		return b;
	}

	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j) {
		while (j < this->size && chains[j].nt[0] != t) {
			j++;
		}
		if (j != this->size && chains[j].nt[0] == t) {
			if (j < this->size) {
				pchain = chains[j];
				return j;
			}
		}
		return -1;
	}

	Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottomT, short psize, Rule r, ...) {
		this->startN = pstartN;
		this->stbottomT = pstbottomT;
		this->size = psize;
		this->rules = new Rule[psize];
		Rule* p = &r;
		for (short i = 0; i < psize; i++) {
			this->rules[i] = p[i];
		}
	}

	short Greibach::getRule(GRBALPHABET pnn, Rule& prule) {
		short i = 0;
		while (i < this->size && this->rules[i].nn != pnn) {
			i++;
		}
		if (i != this->size && i < size) {
			prule = this->rules[i];
			return i;
		}
		return -1;
	}

	Rule Greibach::getRule(short n) {
		if (this->size < n) {
			throw ERROR_THROW(600);
		}
		return this->rules[n];
	}

	Greibach getGreibach() {
		return greibach;
	}
}