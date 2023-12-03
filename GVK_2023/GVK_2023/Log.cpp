#define _CRT_SECURE_NO_WARNINGS
#include "Log.h"
#include "In.h"
#include "Error.h"
#include <fstream>
#include <stdarg.h>
#include <iostream>
#include <ctime>


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

	void Close(LOG log) {
		puts("Программа выполнена");
		(*log.stream).close();
	}
}