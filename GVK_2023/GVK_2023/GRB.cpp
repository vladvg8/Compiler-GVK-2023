#define _CRT_SECURE_NO_WARNINGS
#include "GRB.h"
#include <iostream>

typedef short GRBALPHABET;

#define GRB_ERROR_SERIES 600

using namespace std;

namespace GRB {
#define NS(n) Rule::Chain::N(n)
#define TS(n) Rule::Chain::T(n)	

	/*
	
		S - начало работы программы
		F - параметры функции
		C - параметры функции при добавлении еще нескольких параметров
		R - возвращение значения функции
		N - тело функции или конструкции языка
		O - параметры специальной функции display
		E - все возможные выражения для присваивания переменной какого-либо значения
		P - параметры функции
		K - вспомогательные выражения для добавления нескольких параметров
		W - то что может быть в скобках после if
		T - вспомогаетльный граф для того что может быть после скобок if
		J - выражение для else

	*/

	Greibach greibach(NS('S'), TS('$'),
		12,
		Rule(NS('S'), GRB_ERROR_SERIES + 0, // функции и main
			3,
			Rule::Chain(12, TS('d'), TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), NS('R'), TS('}'), NS('S')),
			Rule::Chain(4, TS('m'), TS('{'), NS('N'), TS('}')),
			Rule::Chain()
		),
		Rule(NS('F'), GRB_ERROR_SERIES + 1, // параметр функции
			3,
			Rule::Chain(3, TS('t'), TS('i'), NS('C')),
			Rule::Chain(4, TS(','), TS('t'), TS('i'), NS('C')),
			Rule::Chain()
		),
		Rule(NS('C'), GRB_ERROR_SERIES + 2, // параметр функции (проверка добавляем ли еще что-то)
			2,
			Rule::Chain(4, TS(','), TS('t'), TS('i'), NS('F')),
			Rule::Chain()
		),
		Rule(NS('R'), GRB_ERROR_SERIES + 3, // return
			3,
			Rule::Chain(3, TS('r'), TS('i'), TS(';')),
			Rule::Chain(3, TS('r'), TS('l'), TS(';')),
			Rule::Chain()
		),
		Rule(NS('N'), GRB_ERROR_SERIES + 4, // тело
			7,
			Rule::Chain(),
			Rule::Chain(7, TS('d'), TS('t'), TS('i'), TS(':'), NS('E'), TS(';'), NS('N')),
			Rule::Chain(5, TS('i'), TS(':'), NS('E'), TS(';'), NS('N')),
			Rule::Chain(6, TS('p'), TS('('), NS('O'), TS(')'), TS(';'), NS('N')),
			Rule::Chain(6, TS('i'), TS('('), NS('P'), TS(')'), TS(';'), NS('N')),
			Rule::Chain(8, TS('y'), TS('('), NS('W'), TS(')'), TS('{'), NS('N'), TS('}'), NS('N')),
			Rule::Chain(9, TS('y'), TS('('), NS('W'), TS(')'), TS('{'), NS('N'), TS('}'), NS('J'), NS('N'))
		),
		Rule(NS('E'), GRB_ERROR_SERIES + 5, 
			3,
			Rule::Chain(4, TS('i'), TS('('), NS('P'), TS(')')),
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l'))
		),
		Rule(NS('O'), GRB_ERROR_SERIES + 6, // то что ты передаешь в display
			4,
			Rule::Chain(4, TS('i'), TS('('), NS('P'), TS(')')),
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain()
		),
		Rule(NS('P'), GRB_ERROR_SERIES + 7,
			7,
			Rule::Chain(4, TS('i'), TS('('), NS('P'), TS(')')),
			Rule::Chain(2, TS('i'), NS('K')),
			Rule::Chain(2, TS('l'), NS('K')),
			Rule::Chain(5, TS(','), TS('i'), TS('('), NS('P'), TS(')')),
			Rule::Chain(3, TS(','), TS('i'), NS('K')),
			Rule::Chain(3, TS(','), TS('l'), NS('K')),
			Rule::Chain()
		),
		Rule(NS('K'), GRB_ERROR_SERIES + 8,
			4,
			Rule::Chain(6, TS(','), TS('i'), TS('('), NS('P'), TS(')'), NS('P')),
			Rule::Chain(3, TS(','), TS('i'), NS('P')),
			Rule::Chain(3, TS(','), TS('l'), NS('P')),
			Rule::Chain()
		),
		Rule(NS('W'), GRB_ERROR_SERIES + 9,
			3,
			Rule::Chain(5, TS('i'), TS('('), NS('P'), TS(')'), NS('T')),
			Rule::Chain(2, TS('i'), NS('T')),
			Rule::Chain(2, TS('l'), NS('T'))
		),
		Rule(NS('T'), GRB_ERROR_SERIES + 10,
			4,
			Rule::Chain(5, TS('k'), TS('i'), TS('('), NS('P'), TS(')')),
			Rule::Chain(2, TS('k'), TS('i')),
			Rule::Chain(2, TS('k'), TS('l')),
			Rule::Chain()
		),
		Rule(NS('J'), GRB_ERROR_SERIES + 11,
			1,
			Rule::Chain(4, TS('u'), TS('{'), NS('N'), TS('}'))
		)
		);



	Greibach getGreibach() { return greibach; }

	Rule::Chain::Chain(short psize, GRBALPHABET s, ...)
	{
		nt = new GRBALPHABET[size = psize];
		int* p = (int*)&s;						//Зачем приводим к int
		for (short i = 0; i < size; i++)
			nt[i] = (GRBALPHABET)p[i];			//Чтобы потом привести к short
	}

	Rule::Rule(GRBALPHABET pnn, int iderror, short psize, Chain c, ...)
	{
		nn = pnn;
		idError = iderror;
		chains = new Chain[size = psize];
		Chain* p = &c;
		for (short i = 0; i < size; i++)
			chains[i] = p[i];
	}

	Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottomT, short psize, Rule r, ...)
	{
		startN = pstartN;
		stbottomT = pstbottomT;
		rules = new Rule[size = psize];
		Rule* p = &r;
		for (short i = 0; i < size; i++)
			rules[i] = p[i];
	}

	short Greibach::getRule(GRBALPHABET pnn, Rule& prule)
	{
		short rc = -1;
		short k = 0;
		while (k < this->size && this->rules[k].nn != pnn)
			k++;
		if (k < this->size)
			prule = this->rules[rc = k];
		return rc;
	}

	Rule Greibach::getRule(short n)
	{
		Rule rc;
		if (n < this->size)
			rc = this->rules[n];
		return rc;
	}

	char* Rule::getCRule(char* b, short nchain)
	{
		char bchain[200];		//почему фиксировано?
		b[0] = Chain::alphabet_to_char(this->nn);
		b[1] = '-';
		b[2] = '>';
		b[3] = 0x00;
		chains[nchain].getCChain(bchain);		//как происходит присваивание?
		strcat_s(b, sizeof(bchain) + 5, bchain);	//Почему (+5) в количестве 
		return b;
	}

	short Rule::getNextChain(GRBALPHABET t, Chain& pchain, short j)
	{
		short rc = -1;
		while (j < size && chains[j].nt[0] != t && chains[j].nt[0] != 0)
			j++;
		rc = (j < size ? j : -1);
		if (rc >= 0)				//Можно ли заменить на (rc != -1)???
			pchain = chains[rc];
		return rc;
	}

	char* Rule::Chain::getCChain(char* b)
	{
		for (int i = 0; i < size; i++)
			b[i] = Chain::alphabet_to_char(nt[i]);
		b[size] = 0x00;
		return b;
	}
}