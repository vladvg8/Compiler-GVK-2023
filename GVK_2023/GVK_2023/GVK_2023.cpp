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
#include "CodeGeneration.h"
#include "Out.h"

using namespace std;

int _tmain(int argc, wchar_t* argv[]) {
	setlocale(LC_ALL, "Russian");
	Parm::PARM params;
	Log::LOG log = Log::INITLOG;
	try {
		params = Parm::getparm(argc, argv);
		log = Log::getlog(params.log);
		Out::OUT out = Out::getout(params.out);
		Log::WriteLog(log);
		Log::WriteParm(log, params);
		In::IN in = In::getin(params.in);
		Log::WriteIn(log, in);

		// Таблица лексем и идентификаторов

		LT::LexTable lexTable = LT::Create();
		IT::IdTable idTable = IT::Create();
		IT::AddFunctionStaticLib(idTable);

		// Лексический анализ

		FST::Analyze(in, lexTable, idTable);
		cout << "Лексический анализ завершен без ошибок\n";
		Log::WriteTables(log, idTable, lexTable);

		MFST::LEX lex(lexTable, idTable);
		MFST::Mfst mfst(lex, GRB::getGreibach(), log);
		if (!mfst.start()) {
			Log::WriteLine(log, (char*)"\nСинтаксический анализ завершен с ошибкой", "");
			return -1;
		};
		mfst.savededucation();
		mfst.printrules();
		Log::WriteLine(log, (char*)"\nСинтаксический анализ завершен без ошибок\n", "");
		cout << "Синтаксический анализ завершен без ошибок\n";
		// Семантический анализ

		if (SA::startSA(lex)) {
			Log::WriteLine(log, (char*)"\nСемантический анализ выполнен без ошибок\n", "");
			cout << "Семантический анализ выполнен без ошибок\n";
		}

		// Польская запись

		PolishNotation::CreatePolishTable(lex);

		Log::WriteLine(log, (char*)"\nПольская запись выполнена без ошибок\n", "");
		cout << "Польская запись выполнена без ошибок\n";

		for (int i = 0; i < lex.lextable.size; i++) {
			cout << lex.lextable.table[i].lexema;
		}

		// Генерация кода

		CodeGeneration::GenerateCode(lex, out);

		cout << endl << "Программа выполнена успешно!" << endl;
		

	}
	catch (Error::ERROR error) {
		
		Log::WriteError(log, error);
	}
	std::cout << std::endl;
	system("PAUSE");
	return 0;
}
