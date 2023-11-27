#define _CRT_SECURE_NO_WARNINGS
#include "In.h"
#include "Error.h"
#include <fstream>
#include <iostream>

using namespace std;

namespace In {
	IN getin(wchar_t infile[]) {
		std::wifstream fin(infile);
		if (!fin) {
			throw ERROR_THROW(110); // Ошибка при открытии файла с исходным кодом (-in)
		}
		fin.imbue(std::locale("ru_RU.UTF-8")); // правила чтения ru_RU.UTF-8
		fin.seekg(0, std::ios_base::end);      // перемещаемся в конец файла
		int size = (int)fin.tellg();		   // количество символов
		fin.seekg(0, std::ios_base::beg);	   // возвращаемся в начало файла
		if (size > IN_MAX_LEN_TEXT) {
			ERROR_THROW(112);
		}
		IN in;
		in.size = 0;
		in.lines = 1;
		in.ignor = 0;
		in.text = new wchar_t[size];

		int col = 1;
		wchar_t prev;
		wchar_t ch;
		while ((ch = fin.get()) != '\0' && !fin.fail()) {
			if (ch == L' ' && prev == L' ') {
				continue;
			}
			wchar_t wstr[3]{};
			wstr[0] = ch;
			char str[3]{};
			wcstombs(str, wstr, sizeof(wstr));
			int ich = (int)str[0];
			if (ich < 0) {
				ich = ich + 256;
			}
			in.text[in.size] = ch;
			switch (in.code[ich]) {
			case IN::T: {
				if (ch == '\n') {
					in.size++;
					in.lines++;
					col = 0;
				}
				else {
					col++;
					in.size++;
				}
				break;
			}
			case IN::F: {
				col++;
				std::wcout << ch;
				throw ERROR_THROW_IN(111, in.lines, col);
				break;
			}
			case IN::I: {
				in.ignor++;
				break;
			}
			case IN::SPACE: {
				in.size++;
				col++;
				break;
			}
			default: {
				ERROR_THROW(1);
				break;
			}
			}
			prev = ch;
		}
		fin.close();
		return in;
	}

}