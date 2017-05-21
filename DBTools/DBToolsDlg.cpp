
// DBToolsDlg.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "vld.h"
#include "DBTools.h"
#include "DBToolsDlg.h"
#include "tinyxml.h"

#include "type.h"
#include "errno.h"
#include "const.h"
#include "global.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CDBToolsDlg �Ի���
CDBToolsDlg::CDBToolsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDBToolsDlg::IDD, pParent)
    , m_strHostName(_T(""))
    , m_strDbName(_T(""))
    , m_strAccount(_T(""))
    , m_strPassword(_T(""))
    , m_strSQL(_T(""))
    , m_strLog(_T(""))
    , m_Log("dbtools.log")
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_LOGO);
}

void CDBToolsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_HostName, m_strHostName);
    DDV_MaxChars(pDX, m_strHostName, 16);
    DDX_Text(pDX, IDC_DbName, m_strDbName);
    DDV_MaxChars(pDX, m_strDbName, 16);
    DDX_Text(pDX, IDC_Account, m_strAccount);
    DDV_MaxChars(pDX, m_strAccount, 16);
    DDX_Text(pDX, IDC_Password, m_strPassword);
    DDV_MaxChars(pDX, m_strPassword, 16);
    DDX_Text(pDX, IDC_SQL, m_strSQL);
    DDV_MaxChars(pDX, m_strSQL, 1024);
    DDX_Text(pDX, IDC_Log, m_strLog);
	DDV_MaxChars(pDX, m_strLog, 2048);
}

BEGIN_MESSAGE_MAP(CDBToolsDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_Connect, &CDBToolsDlg::OnBnClickedConnect)
    ON_BN_CLICKED(IDC_Excute, &CDBToolsDlg::OnBnClickedExcute)
    ON_BN_CLICKED(IDC_SAVE_FAVORITE, &CDBToolsDlg::OnBnClickedSaveFavorite)
    ON_BN_CLICKED(IDC_LOAD_FAVORITE, &CDBToolsDlg::OnBnClickedLoadFavorite)
    ON_BN_CLICKED(IDC_EXIT, &CDBToolsDlg::OnBnClickedExit)
END_MESSAGE_MAP()


// CDBToolsDlg ��Ϣ�������
BOOL CDBToolsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_MINIMIZE);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CDBToolsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�
void CDBToolsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CDBToolsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDBToolsDlg::OnBnClickedConnect()
{
    UpdateData(TRUE);

    int result = m_dbAgent.ConnectToDb(m_strHostName, m_strDbName, m_strAccount, m_strPassword);
    switch(result)
    {
        case CONNECT_SUCCESS :
            logout( _T("����MySQL���ݿ�ɹ���") );
            m_Log.logInfo("����MySQL���ݿ�ɹ���");
            break;

        case INIT_FAILURE :
            logout( _T("����MySQL�������Ӿ��ʧ�ܣ�") );
            m_Log.logError("����MySQL�������Ӿ��ʧ�ܣ�");
            break;

        case CONNECT_FAILURE :
            logout( _T("����MySQL���ݿ�ʧ�ܣ�") );
            m_Log.logError("����MySQL���ݿ�ʧ�ܣ�");
            break;

        default :
            //todo
            ;
    }
}

void CDBToolsDlg::OnBnClickedExcute()
{
    //��ȡִ��SQL
    UpdateData(TRUE);

    //�������״̬
    if(!m_dbAgent.checkConnect())
    {
        logout( _T("�����������ݿ⣡") );
        return;
    }

    //ִ�в�ѯ
    m_strSQL.Trim();
    if(m_dbAgent.Query(m_strSQL))
    {
        //���ʧ����־
        logout( _T("ִ��SQL��") + m_strSQL + _T(" ʧ�ܣ�") );
    }
    else
    {
        //����ɹ���־
        logout( _T("ִ��SQL��") + m_strSQL + _T(" �ɹ���") );

        if(('S' == m_strSQL[0] || 's' == m_strSQL[0])
            && ('E' == m_strSQL[1] || 'e' == m_strSQL[1])
            && ('L' == m_strSQL[2] || 'l' == m_strSQL[2]))//Select���
        {
            //��ȡ��ѯ���
            TRESULT vecResult;
            TCOLUMN col;
            m_dbAgent.getSelectResult(col, vecResult);

            //���ݲ�ѯ��������ý�����
            setResultList(col, vecResult);
        }
        else
        {
            unsigned long long iAffectedRow = 0;
            m_dbAgent.getResult(iAffectedRow);
            CString s;
            s.Format(_T("�����ˣ�%d �У�"), iAffectedRow);
            logout( s );
        }
    }

}

void CDBToolsDlg::setResultList(TCOLUMN& col, const TRESULT& vecResult)
{
    //����֮ǰ�Ĳ�ѯ���
    CListCtrl* pList = (CListCtrl*) GetDlgItem(IDC_Result);
    pList->SetRedraw(FALSE);  
    pList->DeleteAllItems(); 
    int iCount = (pList->GetHeaderCtrl())->GetItemCount();
    for(int i=0; i<iCount; i++)
    {
      pList->DeleteColumn(0);
    }

    //���ý����ͷ
    _TCHAR szBuf[MAX_BUF_SIZE+1] = {0};
    size_t count = 0;
    for(unsigned int i = 0; i < col.size(); i++)
    {
        mbstowcs_s(&count, szBuf, MAX_BUF_SIZE, col[i].c_str(), MAX_BUF_SIZE);
        pList->InsertColumn(i, szBuf, LVCFMT_LEFT, 80);
    }
    
    //��ʾ��ѯ���
    if(0 == vecResult.size())//û�н��ʱ����ʾһ�п�
    {
        int nRow = pList->InsertItem(0, _T(""));//�������
        for(unsigned int j = 0; j < col.size(); j++)
        {
            pList->SetItemText(nRow, j, _T("null"));//����null����
        }
    }
    else
    {
        for(unsigned int i = 0; i < vecResult.size(); i++)
        {
            int nRow = pList->InsertItem(i, _T(""));//������
            for(unsigned int j = 0; j < vecResult[i].size(); j++)
            {
                locale_mbstowcs_s(&count, szBuf, MAX_BUF_SIZE, vecResult[i][j].c_str(), MAX_BUF_SIZE);
                pList->SetItemText(nRow, j, szBuf);//��������
            }

        }
    }

    AutoAdjustColumnWidth(pList);
}

void CDBToolsDlg::OnBnClickedSaveFavorite()
{
    UpdateData(TRUE);
    TiXmlDocument doc;
    TiXmlDeclaration decl( "1.0", "", "" );
    doc.InsertEndChild( decl );

    //Unicode to ASCII
    size_t count;
    char szHostName[NORMAL_BUF_SIZE+1] = {0};
    char szDbName[NORMAL_BUF_SIZE+1] = {0};
    char szAccount[NORMAL_BUF_SIZE+1] = {0};
    char szPassword[NORMAL_BUF_SIZE+1] = {0};
    wcstombs_s(&count, szHostName, NORMAL_BUF_SIZE, m_strHostName, NORMAL_BUF_SIZE);
    wcstombs_s(&count, szDbName, NORMAL_BUF_SIZE, m_strDbName, NORMAL_BUF_SIZE);
    wcstombs_s(&count, szAccount, NORMAL_BUF_SIZE, m_strAccount, NORMAL_BUF_SIZE);
    wcstombs_s(&count, szPassword, NORMAL_BUF_SIZE, m_strPassword, NORMAL_BUF_SIZE);

    TiXmlText txHostName(szHostName);
    TiXmlText txDbName(szDbName);
    TiXmlText txAccount(szAccount);
    TiXmlText txPassword(szPassword);

    TiXmlElement hostname("hostname");
    TiXmlElement dbname("dbname");
    TiXmlElement account("account");
    TiXmlElement password("password");

    hostname.InsertEndChild(txHostName);
    dbname.InsertEndChild(txDbName);
    account.InsertEndChild(txAccount);
    password.InsertEndChild(txPassword);

    TiXmlElement connectinfo("connectinfo");

    connectinfo.InsertEndChild(hostname);
    connectinfo.InsertEndChild(dbname);
    connectinfo.InsertEndChild(account);
    connectinfo.InsertEndChild(password);

    doc.InsertEndChild(connectinfo);

    doc.SaveFile("favorite.xml");

    logout(_T("�ղ�������Ϣ�ɹ���"));
    m_Log.logInfo("�ղ�������Ϣ�ɹ���");

}

void CDBToolsDlg::OnBnClickedLoadFavorite()
{
    TiXmlDocument doc("favorite.xml");
    if(doc.LoadFile())
    {
        TiXmlElement* root = doc.RootElement();
        TiXmlNode* child = root->FirstChild();
        m_strHostName = child->ToElement()->GetText(); 


        child = root->IterateChildren(child); 
        m_strDbName = child->ToElement()->GetText(); 

        child = root->IterateChildren(child); 
        m_strAccount = child->ToElement()->GetText(); 

        child = root->IterateChildren(child); 
        m_strPassword = child->ToElement()->GetText(); 

        logout( _T("�����ղص�������Ϣ�ɹ���") );
        m_Log.logInfo("�����ղص�������Ϣ�ɹ���");

    }
    else
    {
        logout( _T("Ŀǰû���ղ�������Ϣ��") );
        m_Log.logInfo("Ŀǰû���ղ�������Ϣ��");
    }
}

void CDBToolsDlg::OnOK()
{
    // TODO: �ڴ����ר�ô����/����û���

    //CDialog::OnOK();
}

void CDBToolsDlg::OnCancel()
{
    // TODO: �ڴ����ר�ô����/����û���

    //CDialog::OnCancel();
}

void CDBToolsDlg::OnBnClickedExit()
{
    CDialog::OnOK();
}

void CDBToolsDlg::logout(CString sLogInfo)
{
    m_strLog += sLogInfo + _T("\r\n");
    UpdateData(FALSE);
    CEdit* hLogEdit = (CEdit*)GetDlgItem(IDC_Log);
    hLogEdit->LineScroll(hLogEdit->GetLineCount());

}
