#pragma once

#include "IT.h"
#include "LT.h"
#include "MFST.h"

namespace PolishNotation {
	bool PolishNotation(int pos, LT::LexTable& lextable, IT::IdTable& idtable);
	void CreatePolishTable(MFST::LEX& lex);
	void FixLT(LT::LexTable& lextable, const std::string& str, int length, int pos, std::vector<int>& ids);
}