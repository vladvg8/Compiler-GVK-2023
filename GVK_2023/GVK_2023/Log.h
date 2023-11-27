#pragma once

#include "stdafx.h"

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
	void Close(LOG log);
}