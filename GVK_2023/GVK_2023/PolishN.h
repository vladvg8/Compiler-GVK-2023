#pragma once

#include "IT.h"
#include "LT.h"
#include "MFST.h"

namespace PolishNotation {
	bool PolishNotation(int pos, LT::LexTable& lextable, IT::IdTable& idtable);
	void CreatePolishTable(MFST::LEX& lex);
}