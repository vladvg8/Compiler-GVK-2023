#include <iostream>
#include <cwchar>
#include <tchar.h>

#include "Error.h"
#include "Parm.h"
#include "In.h"
#include "Log.h"
#include "LT.h"
#include "IT.h"
#include "MFST.h"
#include "GRB.h"
#include "Semantic.h"
#include "PolishN.h"
#include "FST.h"

using namespace std;

int _tmain(int argc, wchar_t* argv[]) {
	setlocale(LC_ALL, "Russian");
	Parm::PARM params;
	Log::LOG log;
	try {
		params = Parm::getparm(argc, argv);
		log = Log::getlog(params.log);
		Log::WriteLog(log);
		Log::WriteParm(log, params);
		In::IN in = In::getin(params.in);
		Log::WriteIn(log, in);
		// Таблица лексем и идентификаторов
		LT::LexTable lexTable = LT::Create();
		IT::IdTable idTable = IT::Create();
		// Лексический анализ
		FST::Analyze(in, lexTable, idTable);
		cout << "Лексический анализ завершен без ошибок\n";
		Log::WriteTables(log, idTable, lexTable);
		// Синтаксический анализ
		MFST::LEX lex(lexTable, idTable);
		MFST::Mfst mfst(lex, GRB::getGreibach(), log);
		if (!mfst.start()) {
			Log::WriteLine(log, (char*)"\nСинтаксический анализ завершен с ошибкой", "");
			return -1;
		};
		Log::WriteLine(log, (char*)"\nСинтаксический анализ завершен без ошибок\n", "");
		cout << "Синтаксический анализ завершен без ошибок\n";
		// Семантический анализ
		if (SA::startSA(lex)) {
			Log::WriteLine(log, (char*)"\nСемантический анализ выполнен без ошибок\n", "");
			cout << "Семантический анализ выполнен без ошибок\n";
		}
		PolishNotation::CreatePolishTable(lex);
		cout << "Польская запись завершена без ошибок\n";
		for (int i = 0; i < lexTable.size; i++) {
			cout << lexTable.table[i].lexema;
		}
		cout << "Программа выполнена успешно!" << endl;
		

	}
	catch (Error::ERROR error) {
		// обработка ошибки если параметр -in не задан.
		// Если даже параметр -in не задан и лог не задан, то создастся дефортный log в который будет записано сообщение
		//if (params.log != L"") {
		//	wcscpy_s(params.log, L"default");
		//	wcscat_s(params.log, PARM_LOG_DEFAULT_EXT);
		//	log = Log::getlog(params.log);
		//	Log::WriteLog(log);
		//}
		Log::WriteError(log, error);
	}
	return 0;
}