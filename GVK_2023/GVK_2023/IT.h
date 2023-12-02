#pragma once
#define ID_MAXSIZE 25 // максимальная длина идентификатора
#define TI_MAXSIZE 4096 // максимальное количество строк в таблице идентификаторов
#define TI_BYTE_DEFAULT 0 // Значение по умолчанию для byte - целочисленный тип данных в 1 байт
#define TI_TEXT_DEFAULT "" // Значение по умолчанию для text - аналог string
#define TI_SYMBOL_DEFAULT '\0' // Значение по умолчанию для symbol - аналог char
#define TI_BOOLEAN_DEFAULT false // Значение по умолчанию для boolean - true/false
#define TI_NULLIDX 0xffffffff // Нет значения в таблице идентификаторов
#define TI_TEXT_MAXSIZE 255 // максимальное количество символов в text
#define TI_SYMBOL_MAXSIZE 1 // максимальное количество символов в symbol

#include <vector>
#include "Error.h"

namespace IT {
	enum IDDATATYPE { // типы данных идентификаторов
		BYTE = 1, 
		TEXT = 2,
		SYMBOL = 3,
		BOOLEAN = 4,
		HALLOW = 5
	};
	enum IDTYPE { // тип идентификаторов
		VARIABLE = 1, 
		FUNCTION = 2, 
		PARAMETER = 3, 
		LITERAL = 4 
	};

	struct Entry {
		int idxfirstLE; // индекс первой строки в таблице лексем
		char id[ID_MAXSIZE]{}; // имя идентификатора
		IDDATATYPE iddatatype; // тип данных
		IDTYPE idtype; // тип ид
		union {
			unsigned short vbyte; // значение byte
			bool vboolean; // значение boolean
			char vsymbol; // значение symbol
			struct {
				int len; // количество символов
				char str[TI_TEXT_MAXSIZE - 1]; // символы
			} vtext[TI_TEXT_MAXSIZE]; // значение text
		} value;
		Entry(
			int ifLE,
			char* name,
			IDDATATYPE iddatatype,
			IDTYPE idtype
		) {
			this->idxfirstLE = ifLE;
			int nameSize = strlen(name);
			if (nameSize > ID_MAXSIZE) {
				throw ERROR_THROW(125);
			}
			strncpy_s(id, name, ID_MAXSIZE);
			id[ID_MAXSIZE] = '\0';
			this->iddatatype = iddatatype;
			this->idtype = idtype;
		}
		Entry() {};
	};

	struct IdTable {
		int size;
		std::vector<Entry>table;
	};
	IdTable Create();
	void Add(
		IdTable& idtable,
		Entry entry
	);
	Entry GetEntry(
		IdTable& idtable,
		int n
	);
	int IsId(
		IdTable& idtable,
		char id[ID_MAXSIZE]
	);
	void Delete(IdTable& idtable);
}