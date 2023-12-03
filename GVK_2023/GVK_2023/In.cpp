#define _CRT_SECURE_NO_WARNINGS
#include "In.h"
#include "Error.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <ios>

using namespace std;

namespace In {
	IN getin(wchar_t infile[]) {
		std::fstream fin(infile);
		if (!fin) {
			throw ERROR_THROW(110); // Ошибка при открытии файла с исходным кодом (-in)
		}
		fin.imbue(std::locale("ru_RU.UTF-8"));
		std::stringstream buffer;
		buffer << fin.rdbuf();
		string test = buffer.str();
		int size = test.size();
		fin.clear();
		fin.seekg(0, std::ios::beg);

		if (size > IN_MAX_LEN_TEXT) {
			throw ERROR_THROW(112);
		}

		IN in;
		in.size = 0;
		in.lines = 1;
		in.ignor = 0;
		in.text = new char[size] {};

		int col = 1;
		char prev = NULL;
		char ch;
		while ((ch = fin.get()) != '\0' && !fin.fail()) {
			if (ch == L' ' && prev == L' ' || ch == L' ' && prev == NULL) {
				continue;
			}
			int ich = (int)ch;
			if (ich < 0) {
				ich = ich + 256;
			}
			switch (in.code[ich]) {
			case IN::T: {
				if (ch == '\n') {
					in.text[in.size] = ch;
					in.size++;
					in.lines++;
					col = 0;
				}
				else {
					in.text[in.size] = ch;
					col++;
					in.size++;
				}
				break;
			}
			case IN::F: {
				col++;
				throw ERROR_THROW_IN(111, in.lines, col);
				break;
			}
			case IN::I: {
				in.ignor++;
				break;
			}
			case IN::SPACE: {
				in.text[in.size] = ch;
				in.size++;
				col++;
				break;
			}
			default: {
				throw ERROR_THROW(1);
				break;
			}
			}
			prev = ch;
		}
		fin.close();
		in.text[in.size] = ' ';
		in.size++;
		return in;
	}

}