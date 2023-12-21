#define _CRT_SECURE_NO_WARNINGS

#include "Out.h"
#include "In.h"
#include "Parm.h"
#include <fstream>
#include <iostream>

namespace Out {

	void WriteLine(OUT out, char* c, ...) {
		char str[80]{};
		char** p = &c;
		while (*p[0]) {
			strcat_s(str, *p);
			p++;
		}
		str[strlen(str)] = '\0';
		(*out.stream) << str;
	}

	void WriteLine(OUT out, wchar_t* c, ...) {
		wchar_t strw[300]{};
		char str[300]{};
		wchar_t** p = &c;
		while (*p[0]) {
			wcscat_s(strw, *p);
			p++;
		}
		wcstombs(str, strw, sizeof(strw));
		(*out.stream) << str;
	}

	OUT getout(wchar_t* file) {
		OUT out;
		out.stream = new std::wofstream;
		out.stream->open(file);
		wcscpy_s(out.filename, file);
		return out;
	}

	void Close(OUT out) {
		out.stream->close();
	}
}