#pragma once
#include "In.h"
#include "Parm.h"
#include "Error.h"
#include <fstream>

namespace Out {

	struct OUT {
		wchar_t filename[PARM_MAX_SIZE];
		std::wofstream* stream;
	};

	static const OUT INITOUT = { L"", NULL };

	void WriteLine(OUT out, char* c, ...);
	void WriteLine(OUT out, wchar_t* c, ...);
	OUT getout(wchar_t* file);
	void Close(OUT out);
}