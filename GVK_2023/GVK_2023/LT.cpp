#include "LT.h"
#include <vector>
#include "In.h"
#include "Error.h"
using namespace std;

namespace LT {
	LexTable Create() {
		LexTable* lextable = new LexTable();
		lextable->size = 0;
		return *lextable;
	}

	void AddEntry(LexTable& lextable, Entry entry) {
		if (lextable.size + 1 > LT_MAXSIZE) {
			throw ERROR_THROW(115); // При добавлении лексемы был превышен максимальный размер таблицы лексем(-LT)
		}
		lextable.table[lextable.size] = entry;
		lextable.size++;
	}

	Entry GetEntry(LexTable& lextable, int n) {
		return lextable.table[n];
	}

	void Delete(LexTable& lextable) {
		delete& lextable;
	}
}
