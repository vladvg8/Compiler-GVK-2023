#define _CRT_SECURE_NO_WARNINGS
#include "In.h"
#include "Error.h"
#include <fstream>
#include <stdarg.h>
#include <iomanip>
#include <iostream>
#include <ctime>
#include "Log.h"


using namespace std;

namespace Log {
	LOG getlog(wchar_t logfile[]) {
		LOG log{};
		wcscpy_s(log.logfile, logfile);
		log.stream = new ofstream(logfile);
		if (!log.stream->is_open()) {
			throw ERROR_THROW(112)
		}
		return log;
	}

	void WriteLine(LOG log, char* c, ...) {
		char str[1024]{};
		char** p = &c;
		while (*p[0]) {
			strcat_s(str, *p);
			p++;
		}
		str[strlen(str)] = '\0';
		(*log.stream) << str;
	}

	void WriteInText(LOG log, In::IN in) {
		char* text = new char[in.size];
		strcpy(text, in.text);
		(*log.stream) << text;
	}

	void WriteLine(LOG log, wchar_t* c, ...) {
		wchar_t strw[1024]{};
		char str[1024]{};
		wchar_t** p = &c;
		while (*p[0]) {
			wcscat_s(strw, *p);
			p++;
		}
		wcstombs(str, strw, sizeof(strw));
		(*log.stream) << str;
	}

	void WriteLog(LOG log) {
		std::time_t t = std::time(nullptr);
		std::tm* now = std::localtime(&t);
		(*log.stream) << "----Протокол-----" << now->tm_mday << "." << now->tm_mon + 1 << "." <<
			now->tm_year + 1900 << " " << now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec << "----" << endl;
	}

	void WriteParm(LOG log, Parm::PARM parm) {
		char str[300]{};
		(*log.stream) << "----Параметры-----" << endl;
		wcstombs(str, parm.log, sizeof(parm.log));
		(*log.stream) << "-log: " << str << endl;
		wcstombs(str, parm.out, sizeof(parm.out));
		(*log.stream) << "-out: " << str << endl;
		wcstombs(str, parm.in, sizeof(parm.in));
		(*log.stream) << "-in: " << str << endl;
	}

	void WriteError(LOG log, Error::ERROR error) {
		(*log.stream) << "Ошибка " << error.id << " :" << error.message << ", строка " << error.inext.line << ", позиция " << error.inext.col;
	}

	void WriteIn(LOG log, In::IN in) {
		(*log.stream) << "----Исходные данные-----" << endl;
		(*log.stream) << "Количество символов: " << in.size << endl;
		(*log.stream) << "Проигнорировано: " << in.ignor << endl;
		(*log.stream) << "Количество строк: " << in.lines << endl;
	}

	void WriteTables(LOG log, IT::IdTable& idtable, LT::LexTable& lextable) {
		int line = 0;
		for (int i = 0; i < lextable.size; i++) {
			if (line != lextable.table[i].sn) {
				(*log.stream) << "\n" << lextable.table[i].sn << " ";
				line = lextable.table[i].sn;
			}
			(*log.stream) << lextable.table[i].lexema;
			//(*log.stream) << setw(4) << left << i << ":  " << lextable.table[i].lexema << endl;
		}
		(*log.stream) << "\n\n";
		for (int i = 0; i < idtable.size; i++) {
			char idtype[20]{};
			char iddatatype[20]{};
			if (idtable.table[i].idtype == IT::VARIABLE) {
				strcpy_s(idtype, "переменная");
			}
			else if (idtable.table[i].idtype == IT::FUNCTION) {
				strcpy_s(idtype, "функция");
			}
			else if (idtable.table[i].idtype == IT::PARAMETER) {
				strcpy_s(idtype, "параметр");
			}
			else {
				strcpy_s(idtype, "литерал");
			};
			if (idtable.table[i].iddatatype == IT::BYTE) {
				strcpy_s(iddatatype, "целочисленный");
			}
			else if (idtable.table[i].iddatatype == IT::TEXT) {
				strcpy_s(iddatatype, "строчный");
			}
			else if (idtable.table[i].iddatatype == IT::BOOLEAN) {
				strcpy_s(iddatatype, "логический");
			}
			else if (idtable.table[i].iddatatype == IT::HALLOW) {
				strcpy_s(iddatatype, "процедура");
			}
			else {
				strcpy_s(iddatatype, "символьный");
			}
			(*log.stream) << setw(20) << left << idtable.table[i].id << ": Первое вхождение в таблицу лексем: " << setw(3) << left << idtable.table[i].idxfirstLE << ". Тип: " << setw(10) << left << idtype << ". Тип данных: " << iddatatype << "\n";
		}
	}

	void Close(LOG log) {
		puts("Программа выполнена");
		(*log.stream).close();
	}
}