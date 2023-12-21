#include "CodeGeneration.h"

namespace CodeGeneration {

	static int ifs = 1;

	void Head(Out::OUT out) {
		*out.stream << ".586P\n";
		*out.stream << ".model flat, stdcall\n";
		*out.stream << "includelib libucrt.lib\n";
		*out.stream << "includelib kernel32.lib\n";
		*out.stream << "includelib ../Debug/MathOperations.lib\n";
		*out.stream << "ExitProcess PROTO : DWORD\n\n";
		*out.stream << "SetConsoleCP PROTO : DWORD\n\n";
		*out.stream << "SetConsoleOutputCP PROTO : DWORD\n\n";
		*out.stream << "Pow PROTO : BYTE, : BYTE \n\n";
		*out.stream << "Sum PROTO : BYTE, : BYTE \n\n";
		*out.stream << "writestr PROTO : DWORD \n\n";
		*out.stream << "writeint PROTO : BYTE \n\n";
		*out.stream << "writebool PROTO : BYTE \n\n";
		*out.stream << "writechar PROTO : BYTE \n\n";
		*out.stream << ".stack 4096\n\n";
	}

	void Const(Out::OUT out, MFST::LEX lex) {
		*out.stream << ".const\n";
		for (int i = 0; i < lex.idtable.size; i++) {
			if (lex.idtable.table[i].idtype == IT::LITERAL) {
				*out.stream << lex.idtable.table[i].id;
				switch (lex.idtable.table[i].iddatatype)
				{
				case IT::BYTE: {
					*out.stream << " SBYTE " << lex.idtable.table[i].value.vbyte << " ; byte";
					break;
				}
				case IT::SYMBOL: {
					*out.stream << " BYTE " << (int)lex.idtable.table[i].value.vsymbol << " ; symbol";
					break;
				}
				case IT::TEXT: {
					*out.stream << " DB " << lex.idtable.table[i].value.vtext->str << ", 0 ; text";
					break;
				}
				case IT::BOOLEAN: {
					*out.stream << " BYTE " << lex.idtable.table[i].value.vboolean << " ; boolean";
				}
				}
				*out.stream << "\n";
			}
		}
		*out.stream << "\n";
	}

	void Data(Out::OUT out, MFST::LEX lex) {
		*out.stream << ".data\n";
		for (int i = 0; i < lex.idtable.size; i++) {
			if (lex.idtable.table[i].idtype == IT::VARIABLE) {
				*out.stream << lex.idtable.table[i].id;
				switch (lex.idtable.table[i].iddatatype) {
				case IT::BYTE: {
					*out.stream << " SBYTE 0 ; byte";
					break;
				}
				case IT::TEXT: {
					*out.stream << " DWORD 0 ; text";
					break;
				}
				case IT::SYMBOL: {
					*out.stream << " BYTE 0 ; symbol";
					break;
				}
				case IT::BOOLEAN: {
					*out.stream << " BYTE 0 ; boolean";
					;				}
				}
				*out.stream << "\n";
			}
		}
		*out.stream << "\n";
	}

	bool compareVectorToCString(const std::vector<char>& vec, const char* cstr) {
		if (vec.size() != std::strlen(cstr)) {
			return false;
		}

		for (size_t i = 0; i < vec.size(); ++i) {
			if (vec[i] != cstr[i]) {
				return false;
			}
		}

		return true;
	}

	void Expression(Out::OUT out, MFST::LEX lex, int startPos, int endPos) {
		for (int i = startPos; i < endPos; i++) {
			if (lex.lextable.table[i].lexema == LEX_ASSIGNMENT) {
				*out.stream << "\n" << "; string #" << lex.lextable.table[i].sn << " : ";
				for (int j = -1; lex.lextable.table[i + j].lexema != LEX_SEMICOLON; j++) {
					*out.stream << lex.lextable.table[i + j].lexema;
				}
				*out.stream << "\n";
				bool isArgs = false;
				IT::Entry* func, * save = nullptr;
				IT::Entry* recipent = &lex.idtable.table[lex.lextable.table[i - 1].idxTI];
				IT::Entry* sender = &lex.idtable.table[lex.lextable.table[i + 1].idxTI];
				if (sender->idtype != IT::FUNCTION && sender->idtype != IT::STATIC_FUNCTION) {
					switch (recipent->iddatatype) {
					case IT::BYTE: {
						*out.stream << "mov al, " << sender->id << "\n";
						*out.stream << "mov " << recipent->id << ", al \n";
						break;
					}
					case IT::BOOLEAN: {
						*out.stream << "mov al, " << sender->id << "\n";
						*out.stream << "mov " << recipent->id << ", al \n";
						break;
					}
					case IT::SYMBOL: {
						*out.stream << "mov al, " << sender->id << "\n";
						*out.stream << "mov " << recipent->id << ", al \n";
						break;
					}
					case IT::TEXT: { // для текста разное присваение
						if (sender->idtype == IT::LITERAL) {
							*out.stream << "push offset " << sender->id << '\n';
							*out.stream << "pop eax\n";
							*out.stream << "mov " << recipent->id << ", eax \n";
						}
						else {
							*out.stream << "mov esi, " << "offset " << sender->id << "\n";
							*out.stream << "mov edi, " << "offset " << recipent->id << "\n\n";
							*out.stream << "copy_loop: \n";
							*out.stream << "lodsd \n";
							*out.stream << "stosd \n";
							*out.stream << "cmp eax, 0 \n";
							*out.stream << "jnz copy_loop \n";
						}
						break;
					}
					}
				}
				else {
					stack<IT::Entry> stackForParams;
					int iterator = 2;
					while (lex.lextable.table[i + iterator].lexema != '@') {
						stackForParams.push(lex.idtable.table[lex.lextable.table[i + iterator].idxTI]);
						iterator += 1;
					}
					while (!stackForParams.empty()) {
						switch (stackForParams.top().iddatatype) {
						case IT::BYTE:
						case IT::BOOLEAN:
						case IT::SYMBOL: {
							*out.stream << "movsx eax, " << stackForParams.top().id << "\n";
							*out.stream << "push eax\n";
							break;
						}
						case IT::TEXT: {
							if (stackForParams.top().idtype == IT::LITERAL) {
								*out.stream << "push OFFSET " << stackForParams.top().id << "\n";
							}
							else {
								*out.stream << "push " << stackForParams.top().id << "\n";
							}
							break;
						}
						}
						stackForParams.pop();
					}
					if (sender->idtype == IT::FUNCTION) {
						*out.stream << "CALL F" << sender->id << "\n";
					}
					else {
						if (strcmp(sender->id, "Pow") == 0) {
							*out.stream << "CALL Pow" << "\n";
						}
						if (strcmp(sender->id, "Sum") == 0) {
							*out.stream << "CALL Sum" << "\n";
						}
					}
					if (sender->iddatatype == IT::BYTE || sender->iddatatype == IT::BOOLEAN || sender->iddatatype == IT::SYMBOL) {
						*out.stream << "mov " << recipent->id << ", al\n";
					}
					else {
						*out.stream << "mov " << recipent->id << ", eax\n";
					}
				}
			}
			else if (lex.lextable.table[i].lexema == LEX_IF) {
				int cur = 1;
				int startPos;
				bool isEnd = false;
				IT::Entry* first = nullptr, * second = nullptr;
				std::string op = "";
				int currentIf = ifs; // Запоминаем текущее значение ifs
				ifs++; // Увеличиваем для следующего if
				*out.stream << "\nIf_Begin" << currentIf << ": \n";
				while (true) {
					switch (lex.lextable.table[i + cur].lexema) {
					case LEX_RIGHTTHESIS: {
						startPos = i + 1;
						break;
					}
					case LEX_ID: {
						if (first == nullptr) {
							first = &lex.idtable.table[lex.lextable.table[i + cur].idxTI];
						}
						else {
							second = &lex.idtable.table[lex.lextable.table[i + cur].idxTI];
						}
						break;
					}
					case LEX_LITERAL: {
						if (first == nullptr) {
							first = &lex.idtable.table[lex.lextable.table[i + cur].idxTI];
						}
						else {
							second = &lex.idtable.table[lex.lextable.table[i + cur].idxTI];
						}
						break;
					}
					case LEX_MORE: {
						std::vector<char>data = lex.lextable.table[i + cur].data;
						if (compareVectorToCString(data, "==")) {
							op = "je";
						}
						else if (compareVectorToCString(data, "!=")) {
							op = "jne";
						} else if (compareVectorToCString(data, "<=")) {
							op = "jle";
						}
						else if (compareVectorToCString(data, ">=")) {
							op = "jge";
						}
						else if (compareVectorToCString(data, ">")) {
							op = "jg";
						}
						else if (compareVectorToCString(data, "<")) {
							op = "jl";
						}
						break;
					}
					case LEX_LEFTBRACE: {
						if (second == nullptr) {
							second = new IT::Entry;
							if (first->iddatatype == IT::SYMBOL || first->iddatatype == IT::BOOLEAN) {
								*out.stream << "movzx eax, " << first->id << "\n";
							}
							else {
								*out.stream << "movsx eax, " << first->id << "\n";
							}
							*out.stream << "mov ebx, 1\n";
							*out.stream << "cmp eax, ebx\n";
							*out.stream << "je If_End" << currentIf << "\n";
							*out.stream << "jmp If_End" << ifs << "\n";
							break;
						}

						if (first->iddatatype == IT::SYMBOL || first->iddatatype == IT::BOOLEAN) {
							*out.stream << "movzx eax, " << first->id << "\n";
						}
						else {
							*out.stream << "movsx eax, " << first->id << "\n";
						}
						if (second->iddatatype == IT::SYMBOL || second->iddatatype == IT::BOOLEAN) {
							*out.stream << "movzx ebx, " << second->id << "\n";
						}
						else {
							*out.stream << "movsx ebx, " << second->id << "\n";
						}
						*out.stream << "cmp eax, ebx\n";
						*out.stream << op.c_str() << " If_End" << currentIf << "\n";
						*out.stream << "jmp If_End" << ifs << "\n";
						break;

					}
					case LEX_BRACELET: {
						isEnd = true;
						break;
					}
					}
					if (isEnd) {
						*out.stream << "If_End" << currentIf << ":\n"; // Переход к текущему If_End
						ifs += 2;
						Expression(out, lex, startPos, i + cur);
						i = i + cur;
						break;
					}
					cur++;
				}
				*out.stream << "If_End" << currentIf + 1 << ":\n"; // Завершение текущего блока if
			}

			else if (lex.lextable.table[i].lexema == LEX_RETURN) {
				*out.stream << "\n;return\n";
				if (lex.idtable.table[lex.lextable.table[i + 1].idxTI].iddatatype == IT::BYTE) { 
					if (lex.idtable.table[lex.lextable.table[i + 1].idxTI].idtype == IT::LITERAL) {
						*out.stream << "movsx eax, " << lex.idtable.table[lex.lextable.table[i + 1].idxTI].id << "\n";
					}
					else {
						*out.stream << "movsx eax, " << lex.idtable.table[lex.lextable.table[i + 1].idxTI].id << "\n";
						*out.stream << "mov " << lex.idtable.table[lex.lextable.table[i + 1].idxTI].id << ", al\n";
					}
				}
				else if (lex.idtable.table[lex.lextable.table[i + 1].idxTI].iddatatype == IT::TEXT) {
					if (lex.idtable.table[lex.lextable.table[i + 1].idxTI].idtype == IT::LITERAL) {
						*out.stream << "mov eax, OFFSET " << lex.idtable.table[lex.lextable.table[i + 1].idxTI].id << "\n";
					}
					else {
						*out.stream << "mov eax,  " << lex.idtable.table[lex.lextable.table[i + 1].idxTI].id << "\n";
					}
				}
				else {
					*out.stream << "movzx eax, " << lex.idtable.table[lex.lextable.table[i + 1].idxTI].id << "\n";
				}
			}
			else if (lex.lextable.table[i].lexema == LEX_DISPLAY) {
				switch (lex.idtable.table[lex.lextable.table[i + 2].idxTI].iddatatype)
				{
				case (IT::SYMBOL): {
					*out.stream << "push eax\n";
					*out.stream << "movzx eax, " << lex.idtable.table[lex.lextable.table[i + 2].idxTI].id << '\n';
					*out.stream << "push eax\n";
					*out.stream << "CALL writechar" << '\n';
					*out.stream << "pop eax\n" << '\n';
					break;
				}
				case (IT::TEXT): {
					*out.stream << "\npush ";
					if (lex.idtable.table[lex.lextable.table[i + 2].idxTI].idtype == IT::LITERAL)
						*out.stream << "offset " << lex.idtable.table[lex.lextable.table[i + 2].idxTI].id << '\n';
					else
						*out.stream << lex.idtable.table[lex.lextable.table[i + 2].idxTI].id << '\n';

					*out.stream << "CALL writestr" << '\n';
					break;
				}
				case (IT::BYTE): {
					*out.stream << "\nmovsx eax, " << lex.idtable.table[lex.lextable.table[i + 2].idxTI].id << "\n";
					*out.stream << "push eax\n";
					*out.stream << "CALL writeint" << '\n';
					break;
				}
				case (IT::BOOLEAN): {
					*out.stream << "push eax\n";
					*out.stream << "movzx eax, " << lex.idtable.table[lex.lextable.table[i + 2].idxTI].id << '\n';
					*out.stream << "push eax\n";
					*out.stream << "CALL writebool" << '\n';
					*out.stream << "pop eax\n" << '\n';
					break;
				}
				}
			}
			else if (lex.lextable.table[i].lexema == LEX_ID &&
				(lex.lextable.table[i - 1].lexema == ';' || lex.lextable.table[i - 1].lexema == '}' || lex.lextable.table[i - 1].lexema == '{') &&
				lex.lextable.table[i + 1].lexema != ':') {
				stack<IT::Entry> stackForParams;
				IT::Entry* function = &lex.idtable.table[lex.lextable.table[i].idxTI];
				int iterator = 1;
				while (lex.lextable.table[i + iterator].lexema != '@') {
					stackForParams.push(lex.idtable.table[lex.lextable.table[i + iterator].idxTI]);
					iterator += 1;
				}
				while (!stackForParams.empty()) {
					switch (stackForParams.top().iddatatype) {
					case IT::BYTE:
					case IT::BOOLEAN:
					case IT::SYMBOL: {
						*out.stream << "movsx eax, " << stackForParams.top().id << "\n";
						*out.stream << "push eax\n";
						break;
					}
					case IT::TEXT: {
						if (stackForParams.top().idtype == IT::LITERAL) {
							*out.stream << "push OFFSET " << stackForParams.top().id << "\n";
						}
						else {
							*out.stream << "push " << stackForParams.top().id << "\n";
						}
						break;
					}
					}
					stackForParams.pop();
				}
				*out.stream << "CALL F" << function->id << "\n";

			}
		}
	}

	void Functions(Out::OUT out, MFST::LEX lex) {
		for (int i = 0; i < lex.idtable.size; i++) {
			if (lex.idtable.table[i].idtype == IT::FUNCTION) {
				*out.stream << "\nF" << lex.idtable.table[i].id << " PROC uses ebx ecx edi esi";
				int cur = 1;
				while (lex.lextable.table[lex.idtable.table[i].idxfirstLE + cur].lexema != LEX_RIGHTTHESIS) {
					if (lex.lextable.table[lex.idtable.table[i].idxfirstLE + cur].lexema == LEX_ID
						&& lex.idtable.table[lex.lextable.table[lex.idtable.table[i].idxfirstLE + cur].idxTI].idtype == IT::PARAMETER) {
						*out.stream << ", " << lex.idtable.table[lex.lextable.table[lex.idtable.table[i].idxfirstLE + cur].idxTI].id;
						switch (lex.idtable.table[lex.lextable.table[lex.idtable.table[i].idxfirstLE + cur].idxTI].iddatatype)
						{
						case IT::BYTE: {
							*out.stream << " : SBYTE";
							break;
						}
						case IT::TEXT: {
							*out.stream << " : DWORD";
							break;
						}
						case IT::SYMBOL:
						case IT::BOOLEAN: {
							*out.stream << " : BYTE";
							break;
						}
						}
					}
					cur++;
				}
				int startPos = lex.idtable.table[i].idxfirstLE + cur;
				cout << lex.lextable.table[lex.idtable.table[i].idxfirstLE + cur].lexema;
				if (lex.idtable.table[i].iddatatype != IT::HALLOW) {
					while (lex.lextable.table[lex.idtable.table[i].idxfirstLE + cur].lexema != LEX_RETURN) {
						cur++;
					}
					cur += 4;
				}
				else {
					cur += 1;
					stack<char>expectedBrace;
					expectedBrace.push(lex.lextable.table[lex.idtable.table[i].idxfirstLE + cur].lexema);
					while (!expectedBrace.empty()) {
						cur++;
						if (lex.lextable.table[lex.idtable.table[i].idxfirstLE + cur].lexema == '{') {
							expectedBrace.push(lex.lextable.table[lex.idtable.table[i].idxfirstLE + cur].lexema);
						}
						if (lex.lextable.table[lex.idtable.table[i].idxfirstLE + cur].lexema == '}') {
							expectedBrace.pop();
						}
					}
					cur += 1;
				}
				int endPos = lex.idtable.table[i].idxfirstLE + cur;
				Expression(out, lex, startPos, endPos);
				*out.stream << "ret\n";
				*out.stream << "F" << lex.idtable.table[i].id << " ENDP\n\n";
			}
		}
	}

	void Code(Out::OUT out, MFST::LEX lex) {
		*out.stream << ".code\n";
		Functions(out, lex);
		*out.stream << "main PROC\n";
		*out.stream << "Invoke SetConsoleCP, 1251\n";
		*out.stream << "Invoke SetConsoleOutputCP, 1251\n";
		int mainPos = 0;
		int endPos = 0;
		for (int i = 0; i < lex.lextable.size; i++) {
			if (lex.lextable.table[i].lexema == LEX_MAIN) {
				mainPos = i;
				break;
			}
		}
		endPos = lex.lextable.size ;
		Expression(out, lex, mainPos, endPos);
		*out.stream << "push -1\n";
		*out.stream << "call ExitProcess\n";
		*out.stream << "main ENDP\n";
		*out.stream << "end main\n";

	}

	void GenerateCode(MFST::LEX lex, Out::OUT out) {
		Head(out);
		Const(out, lex);
		Data(out, lex);
		Code(out, lex);
	}

}

