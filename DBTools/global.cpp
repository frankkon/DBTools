//全局公共函数实现文件
#include "stdafx.h"
#include "vld.h"

#include <ctime> 
#include <cstdio> 

#include "global.h"
#include "const.h"

std::string getLocalNowTime()
{
    time_t nowtime = time(0);
    struct tm _tm;
    localtime_s(&_tm, &nowtime);
    char szBuf[NORMAL_BUF_SIZE+1] = {0}; 
    strftime(szBuf, NORMAL_BUF_SIZE, "[%Y-%m-%d %X]", &_tm); 
    return szBuf;
}

void AutoAdjustColumnWidth(CListCtrl *pListCtrl)  
{  
    pListCtrl->SetRedraw(FALSE);  
    CHeaderCtrl *pHeader = pListCtrl->GetHeaderCtrl();  
    int nColumnCount = pHeader->GetItemCount();  
  
    for(int i = 0; i < nColumnCount; i++)  
    {  
        pListCtrl->SetColumnWidth(i, LVSCW_AUTOSIZE);  
        int nColumnWidth = pListCtrl->GetColumnWidth(i);  
        pListCtrl->SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);  
        int nHeaderWidth = pListCtrl->GetColumnWidth(i);  
  
        pListCtrl->SetColumnWidth(i, max(nColumnWidth, nHeaderWidth));  
    }  
    pListCtrl->SetRedraw(TRUE);  
}  

errno_t locale_mbstowcs_s(size_t *pReturnValue, wchar_t *wcstr, size_t sizeInWords,const char *mbstr, size_t count)
{
    //setlocale( LC_ALL, "Chinese_People's Republic of China.936" ); //win7上这样OK，win10就乱码
    //setlocale( LC_ALL, "" ); //win10上这样OK
    setlocale( LC_ALL, "chs" ); //win10上OK
    return mbstowcs_s(pReturnValue, wcstr, sizeInWords, mbstr, count);
}
