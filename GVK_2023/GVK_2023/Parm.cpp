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
#include "FST.h"

using namespace std;

// длина начал параметров 
#define PARM_IN 4   // -in:
#define PARM_LOG 5  // -log:
#define PARM_OUT 5  // -out:

namespace Parm {
	PARM getparm(int argc, wchar_t* argv[]) {
		bool isIn = false, isOut = false, isLog = false;
		PARM parms;
		for (int i = 1; i < argc; i++) {
			if (wcslen(argv[i]) > PARM_MAX_SIZE) {
				throw ERROR_THROW(104); // Превышена длина входного параметра
			}
			if (wcsstr(argv[i], PATH_IN)) {
				isIn = true;
				wcscpy_s(parms.in, argv[i] + PARM_IN);
			}
			if (wcsstr(argv[i], PATH_OUT)) {
				isOut = true;
				wcscpy_s(parms.out, argv[i] + PARM_LOG);
			}
			if (wcsstr(argv[i], PATH_LOG)) {
				isLog = true;
				wcscpy_s(parms.log, argv[i] + PARM_OUT);
			}
		};
		if (!isIn) {
			throw ERROR_THROW(100); // Параметр -in должен быть задан
		}
		if (!isOut) {
			wcscpy_s(parms.out, parms.in);
			wcscat_s(parms.out, PARM_OUT_DEFAULT_EXT);
		}
		if (!isLog) {
			wcscpy_s(parms.log, parms.in);
			wcscat_s(parms.log, PARM_LOG_DEFAULT_EXT);
		}
		return parms;
	}
}