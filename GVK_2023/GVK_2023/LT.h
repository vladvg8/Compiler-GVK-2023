#pragma once
#define LEXEMA_FIXSIZE 1 // размер лексемы
#define LT_MAXSIZE 4096 // максимальное количество строк в таблице лексем
#define LT_TI_NULLIDX 0xffffffff // нет элемента в таблице идентификаторов
#define LEX_HALLOW 't'    // void
#define LEX_BYTE 't'      // byte
#define LEX_TEXT 't'	  // text (string)
#define LEX_SYMBOL 't'    // symbol (char)
#define LEX_BOOLEAN 't'   // boolean
#define LEX_ID 'i'        // идентификатор
#define LEX_LITERAL 'l'   // литерал
#define LEX_FUNCTION 'f'  // function
#define LEX_IF 'y'		  // if
#define LEX_ELSE 'u'      // else
#define LEX_DECLARE 'd'   // declare
#define LEX_RETURN 'r'    // return
#define LEX_DISPLAY 'p'   // display (print)
#define LEX_MAIN 'm'      // main - точка входа в программу
#define LEX_ASSIGNMENT ':'// присваивание
#define LEX_SEMICOLON ';'  
#define LEX_COMMA ','
#define LEX_LEFTBRACE '{'
#define	LEX_BRACELET '}'
#define LEX_LEFTTHESIS '('
#define LEX_RIGHTTHESIS ')'
#define LEX_MORE 'k'		// больше
#define LEX_LESS 'k'        // меньше
#define LEX_EQUAL 'k'       // равенство
#define LEX_EXCLAMATION 'k' // неравенство
#define LEX_MOREEQUAL 'k'   // больше равно
#define LEX_LESSEQUAL 'k'   // меньше равно
#define LEX_PLUS 'v'        // +
#define LEX_MINUS 'v'       // -
#define LEX_START 'v'       // *
#define LEX_DIRSLASH 'v'   // /

#include "In.h"
#include <vector>

namespace LT {
	struct Entry {
		char lexema; // лексема
		int sn; // номер строки в исходном коде
		int col;
		int idxTI; // индекс в таблице идентификаторов
		char data;

		Entry( // конструктор для строки таблицы лексем
			char lexema,
			int sn,
			int col,
			int idxTI
		) {
			this->lexema = lexema;
			this->sn = sn;
			this->col = col;
			this->idxTI = idxTI;
		};

		Entry() = default;
	};

	struct LexTable {
		int size;			 // текущий размер
		std::vector<Entry>table; // строки таблицы лексем
	};
	LexTable Create();
	void AddEntry(
		LexTable& lextable,
		Entry entry
	);
	Entry GetEntry(
		LexTable& lextable,
		int n // номер получаемой строки
	);
	void Delete(LexTable& lextable);
}