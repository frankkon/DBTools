#pragma once
#include "stdafx.h"

#include <string>

extern std::string getLocalNowTime();

extern void AutoAdjustColumnWidth(CListCtrl *pListCtrl); 

extern errno_t locale_mbstowcs_s(size_t *pReturnValue, wchar_t *wcstr, size_t sizeInWords,const char *mbstr, size_t count);
