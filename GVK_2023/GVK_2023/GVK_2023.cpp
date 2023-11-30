#include "stdafx.h"

int _tmain(int argc, wchar_t* argv[]) {
	setlocale(LC_ALL, "Russian");
	Parm::PARM params;
	Log::LOG log = Log::INITLOG;
	try {
		params = Parm::getparm(argc, argv);
		log = Log::getlog(params.log);
		Log::WriteLog(log);
		Log::WriteParm(log, params);
		In::IN in = In::getin(params.in);
		Log::WriteIn(log, in);
		Log::WriteInText(log, in);
		LT::LexTable lexTable = LT::Create();
		IT::IdTable idTable = IT::Create();

	}
	catch (Error::ERROR error) {
		// обработка ошибки если параметр -in не задан.
		// Если даже параметр -in не задан и лог не задан, то создастся дефортный log в который будет записано сообщение
		if (params.log == L"") {
			wcscpy_s(params.log, L"default");
			wcscat_s(params.log, PARM_LOG_DEFAULT_EXT);
			log = Log::getlog(params.log);
			Log::WriteLog(log);
		}
		Log::WriteError(log, error);
	}
}