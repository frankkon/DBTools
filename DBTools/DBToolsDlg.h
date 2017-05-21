
// DBToolsDlg.h : ͷ�ļ�
//

#pragma once
#include "DbAgent.h"
#include "Log.h"

// CDBToolsDlg �Ի���
class CDBToolsDlg : public CDialog
{
// ����
public:
	CDBToolsDlg(CWnd* pParent = NULL);	// ��׼���캯��

    // �Ի�������
	enum { IDD = IDD_DBTOOLS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
    virtual void OnOK();
    virtual void OnCancel();


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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

    // MySQL���ݿ�����
    CDbAgent m_dbAgent;

    //������־��¼
    CLog m_Log;

public:
    afx_msg void OnBnClickedConnect();
    afx_msg void OnBnClickedExcute();
    afx_msg void OnBnClickedSaveFavorite();
    afx_msg void OnBnClickedLoadFavorite();
    afx_msg void OnBnClickedExit();

private:
    //���ò�ѯ�����
    void setResultList(TCOLUMN& col, const TRESULT& vecResult);

    //����־�ı��������־��Ϣ
    void logout(CString sLogInfo);
};
