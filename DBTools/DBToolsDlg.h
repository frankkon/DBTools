
// DBToolsDlg.h : 头文件
//

#pragma once
#include "DbAgent.h"
#include "Log.h"

// CDBToolsDlg 对话框
class CDBToolsDlg : public CDialog
{
// 构造
public:
	CDBToolsDlg(CWnd* pParent = NULL);	// 标准构造函数

    // 对话框数据
	enum { IDD = IDD_DBTOOLS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
    virtual void OnOK();
    virtual void OnCancel();


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
    CString m_strHostName;
    CString m_strDbName;
    CString m_strAccount;
    CString m_strPassword;
    CString m_strSQL;
    CString m_strLog;

    // MySQL数据库连接
    CDbAgent m_dbAgent;

    //操作日志记录
    CLog m_Log;

public:
    afx_msg void OnBnClickedConnect();
    afx_msg void OnBnClickedExcute();
    afx_msg void OnBnClickedSaveFavorite();
    afx_msg void OnBnClickedLoadFavorite();
    afx_msg void OnBnClickedExit();

private:
    //设置查询结果表
    void setResultList(TCOLUMN& col, const TRESULT& vecResult);

    //向日志文本框输出日志信息
    void logout(CString sLogInfo);
};
