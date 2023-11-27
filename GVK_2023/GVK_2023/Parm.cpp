#include "stdafx.h"

namespace Parm {
	PARM getparm(int argc, wchar_t* argv[]) {
		bool isIn = 0, isOut = 0, isLog = 0;
		PARM parms;
		for (int i = 1; i < argc; i++) {
			if (wcslen(argv[i]) > PARM_MAX_SIZE) {
				throw ERROR_THROW(104); // Превышена длина входного параметра
			}
			if (wcsstr(argv[i], PATH_IN)) {
				isIn = 1;
				wcscpy_s(parms.in, argv[i] + 4);
			}
			if (wcsstr(argv[i], PATH_OUT)) {
				isOut = 1;
				wcscpy_s(parms.out, argv[i] + 5);
			}
			if (wcsstr(argv[i], PATH_LOG)) {
				isLog = 1;
				wcscpy_s(parms.log, argv[i] + 5);
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