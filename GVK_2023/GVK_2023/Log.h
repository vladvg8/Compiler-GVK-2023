#pragma once

#include <iostream>
#include <cwchar>
#include <fstream>
#include <tchar.h>

#include "Error.h"
#include "Parm.h"
#include "In.h"
#include "FST.h"

namespace Log {
	struct LOG {
		wchar_t logfile[PARM_MAX_SIZE];
		std::ofstream* stream;
	};
	static const LOG INITLOG{ L"", nullptr };
	LOG getlog(wchar_t logfile[]);
	void WriteLine(LOG log, char* c, ...);
	void WriteLine(LOG log, wchar_t* c, ...);
	void WriteLog(LOG log);
	void WriteError(LOG log, Error::ERROR error);
	void WriteParm(LOG log, Parm::PARM parm);
	void WriteInText(LOG log, In::IN in);
	void WriteIn(LOG log, In::IN in);
	void WriteTables(LOG log, IT::IdTable& idtable, LT::LexTable& lextable);
	void Close(LOG log);
}