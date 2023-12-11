#include "IT.h"
#include "Error.h"
#include <string.h>

namespace IT {
	IdTable Create() {
		IdTable* idtable = new IdTable();
		idtable->size = 0;
		return *idtable;
	}

	void Add(IdTable& idtable, Entry entry) {
		if (idtable.size + 1 > TI_MAXSIZE) {
			throw ERROR_THROW(117); // При добавлении инентификатора был превышен максимальный размер таблицы идентификаторов(IT)
		}
		bool isUnique = true;
		if (entry.idtype != IT::LITERAL) {
			for (int i = 0; i < idtable.table.size(); i++) {
				if (strcmp(idtable.table[i].id, entry.id) == 0) {
					isUnique = false;
					break;
				}
			}
		}
		if (isUnique) {
			idtable.table.push_back(entry);
			idtable.size++;
		}
		else {
			throw ERROR_THROW(118);
		}
	}

	Entry GetEntry(IdTable& idtable, int n) {
		return idtable.table[n];
	}

	int IsId(IdTable& idtable, char id[ID_MAXSIZE]) {
		for (int i = 0; i < idtable.size; i++) {
			if (strcmp(idtable.table[i].id, id) == 0) {
				return i;
			}
		}
		return TI_NULLIDX;
	}

	void Delete(IdTable& idtable) {
		delete& idtable;
	}
}