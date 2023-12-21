#pragma once
#include <iostream>
#include "Error.h"
#include "MFST.h"
#include "Out.h"
using namespace std;

namespace CodeGeneration {

	void GenerateCode(MFST::LEX lex, Out::OUT out);
}