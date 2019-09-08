#pragma once



#include <UIlib.h>
using namespace DuiLib;

#include "MySQL.h"

#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_ud.lib")
#   else
#       pragma comment(lib, "DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_u.lib")
#   else
#       pragma comment(lib, "DuiLib.lib")
#   endif
#endif


string UnicodeToANSI(const CDuiString& str);
CDuiString ANSIToUnicode(const string& str);


