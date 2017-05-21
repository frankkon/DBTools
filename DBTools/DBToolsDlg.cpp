
// DBToolsDlg.cpp : 实现文件
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


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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


// CDBToolsDlg 对话框
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


// CDBToolsDlg 消息处理程序
BOOL CDBToolsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MINIMIZE);

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void CDBToolsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
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
            logout( _T("链接MySQL数据库成功！") );
            m_Log.logInfo("链接MySQL数据库成功！");
            break;

        case INIT_FAILURE :
            logout( _T("创建MySQL数据链接句柄失败！") );
            m_Log.logError("创建MySQL数据链接句柄失败！");
            break;

        case CONNECT_FAILURE :
            logout( _T("链接MySQL数据库失败！") );
            m_Log.logError("链接MySQL数据库失败！");
            break;

        default :
            //todo
            ;
    }
}

void CDBToolsDlg::OnBnClickedExcute()
{
    //获取执行SQL
    UpdateData(TRUE);

    //检查连接状态
    if(!m_dbAgent.checkConnect())
    {
        logout( _T("请先连接数据库！") );
        return;
    }

    //执行查询
    m_strSQL.Trim();
    if(m_dbAgent.Query(m_strSQL))
    {
        //输出失败日志
        logout( _T("执行SQL：") + m_strSQL + _T(" 失败！") );
    }
    else
    {
        //输出成功日志
        logout( _T("执行SQL：") + m_strSQL + _T(" 成功！") );

        if(('S' == m_strSQL[0] || 's' == m_strSQL[0])
            && ('E' == m_strSQL[1] || 'e' == m_strSQL[1])
            && ('L' == m_strSQL[2] || 'l' == m_strSQL[2]))//Select语句
        {
            //获取查询结果
            TRESULT vecResult;
            TCOLUMN col;
            m_dbAgent.getSelectResult(col, vecResult);

            //根据查询结果，设置结果表格
            setResultList(col, vecResult);
        }
        else
        {
            unsigned long long iAffectedRow = 0;
            m_dbAgent.getResult(iAffectedRow);
            CString s;
            s.Format(_T("更新了：%d 行！"), iAffectedRow);
            logout( s );
        }
    }

}

void CDBToolsDlg::setResultList(TCOLUMN& col, const TRESULT& vecResult)
{
    //清理之前的查询结果
    CListCtrl* pList = (CListCtrl*) GetDlgItem(IDC_Result);
    pList->SetRedraw(FALSE);  
    pList->DeleteAllItems(); 
    int iCount = (pList->GetHeaderCtrl())->GetItemCount();
    for(int i=0; i<iCount; i++)
    {
      pList->DeleteColumn(0);
    }

    //设置结果表头
    _TCHAR szBuf[MAX_BUF_SIZE+1] = {0};
    size_t count = 0;
    for(unsigned int i = 0; i < col.size(); i++)
    {
        mbstowcs_s(&count, szBuf, MAX_BUF_SIZE, col[i].c_str(), MAX_BUF_SIZE);
        pList->InsertColumn(i, szBuf, LVCFMT_LEFT, 80);
    }
    
    //显示查询结果
    if(0 == vecResult.size())//没有结果时，显示一行空
    {
        int nRow = pList->InsertItem(0, _T(""));//插入空行
        for(unsigned int j = 0; j < col.size(); j++)
        {
            pList->SetItemText(nRow, j, _T("null"));//设置null数据
        }
    }
    else
    {
        for(unsigned int i = 0; i < vecResult.size(); i++)
        {
            int nRow = pList->InsertItem(i, _T(""));//插入行
            for(unsigned int j = 0; j < vecResult[i].size(); j++)
            {
                locale_mbstowcs_s(&count, szBuf, MAX_BUF_SIZE, vecResult[i][j].c_str(), MAX_BUF_SIZE);
                pList->SetItemText(nRow, j, szBuf);//设置数据
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

    logout(_T("收藏连接信息成功！"));
    m_Log.logInfo("收藏连接信息成功！");

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

        logout( _T("加载收藏的连接信息成功！") );
        m_Log.logInfo("加载收藏的连接信息成功！");

    }
    else
    {
        logout( _T("目前没有收藏连接信息！") );
        m_Log.logInfo("目前没有收藏连接信息！");
    }
}

void CDBToolsDlg::OnOK()
{
    // TODO: 在此添加专用代码和/或调用基类

    //CDialog::OnOK();
}

void CDBToolsDlg::OnCancel()
{
    // TODO: 在此添加专用代码和/或调用基类

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
