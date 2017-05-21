#pragma once

#include "mysql.h"
#include "type.h"

class CDbAgent
{
private:

    //MySQL数据库的连接句柄
    MYSQL* m_ptrConn;

public:
    CDbAgent(void);
    ~CDbAgent(void);
    
public:
    int ConnectToDb(const CString& strHostName, const CString& strDbName, 
        const CString& strAccount, const CString& strPassword);

    int Query(CString strSQL);
    int getSelectResult(TCOLUMN& col, TRESULT& res );
    int getSelectResultFieldName(TCOLUMN& col);
    bool checkConnect();
    void getResult(unsigned long long& iAffectedRow);
};

