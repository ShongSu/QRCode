// QRDecodeDlg.cpp : 实现文件
//
 
#include "stdafx.h"
#include "QRDecode.h"
#include "QRDecodeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CQRDecodeDlg 对话框




CQRDecodeDlg::CQRDecodeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQRDecodeDlg::IDD, pParent)
    , m_szMovieName(_T(""))
    , m_szPhoneNum(_T(""))
    , m_szIDNum(_T(""))
    , m_szRow(_T(""))
    , m_szColumn(_T(""))
    , m_szMovieNum(_T(""))
    , m_szVersion(_T(""))
    , m_szLevel(_T(""))
    , m_szDate(_T(""))
    , m_szTime(_T(""))
    , QR_image(NULL)
    , QR_Decode(NULL)
    , flag_draw(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CQRDecodeDlg::~CQRDecodeDlg()
{
    if (QR_image != NULL)
    {
        delete QR_image;
    }

    if (QR_Decode != NULL)
    {
        delete QR_Decode;
    }
}

void CQRDecodeDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT1, m_szMovieName);
    DDX_Text(pDX, IDC_EDIT2, m_szPhoneNum);
    DDX_Text(pDX, IDC_EDIT3, m_szIDNum);
    DDX_Text(pDX, IDC_EDIT5, m_szRow);
    DDX_Text(pDX, IDC_EDIT6, m_szColumn);
    DDX_Text(pDX, IDC_EDIT4, m_szMovieNum);
    DDX_Text(pDX, IDC_EDIT7, m_szVersion);
    DDX_Text(pDX, IDC_EDIT8, m_szLevel);
    DDX_Text(pDX, IDC_EDIT9, m_szDate);
    DDX_Text(pDX, IDC_EDIT10, m_szTime);
}

BEGIN_MESSAGE_MAP(CQRDecodeDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDOK, &CQRDecodeDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CQRDecodeDlg 消息处理程序

BOOL CQRDecodeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CQRDecodeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CQRDecodeDlg::OnPaint()
{
    CPaintDC dc(this); // 用于绘制的设备上下文

	if (IsIconic())
	{	
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
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
        if (flag_draw)
        {
            CRect rt;
            GetDlgItem(IDC_STATIC_IMAGE)->GetClientRect(&rt);
            GetDlgItem(IDC_STATIC_IMAGE)->MapWindowPoints(this, &rt);     

            dc.StretchBlt(rt.left, rt.top, rt.Width(), rt.Height(), QR_image->m_pDC, 0, 0, QR_image->nWidth, QR_image->nHeight, SRCCOPY);
        }
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CQRDecodeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CQRDecodeDlg::OnBnClickedOk()
{
    CString FilePathName;
    CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "支持的格式 (*.bmp)|*.bmp||");
    if(dlg.DoModal()==IDOK)
    {
        //获取选择的文件的路径显示
        FilePathName=dlg.GetPathName();

        if (QR_image != NULL)
        {
            delete QR_image;
        }
        QR_image = new QRcodeImage(FilePathName);
        QR_image->GetImageData();

        m_nVersion = QR_image->finderPattern->m_version;
        m_nCodeSize = m_nVersion * 4 +17;
        for(int i=0;i<m_nCodeSize;i++)
            for(int j=0;j<m_nCodeSize;j++)
                m_CodeData[i][j]=QR_image->bitMatrix[i][j];

        //缩放图像，显示在对话框中
        CRect rt;
        GetDlgItem(IDC_STATIC_IMAGE)->GetClientRect(&rt);
        GetDlgItem(IDC_STATIC_IMAGE)->MapWindowPoints(this, &rt);            

        nShowSize=120;
        CDC * pDC=this->GetDC();
        pDC->StretchBlt(rt.left, rt.top, rt.Width(), rt.Height(), QR_image->m_pDC, 0, 0, QR_image->nWidth, QR_image->nHeight, SRCCOPY);
        flag_draw=true;


        //解码
        if (QR_Decode != NULL)
        {
            delete QR_Decode;
        }
        QR_Decode = new ContentDecoder;
        QR_Decode->DecodeData(m_nCodeSize,m_nVersion,m_CodeData);

        StringSplit(QR_Decode->m_strData);

        m_szVersion.Format("%d", QR_Decode->m_nVersion);
        
        const char *vLevel[] = {"L(%7)", "M(%15)", "Q(%25)", "H(%30)"};
        m_szLevel   = vLevel[QR_Decode->m_nLevel];

        UpdateData(FALSE);
    }
}

void CQRDecodeDlg::StringSplit(CString &szData)
{
    if (szData.IsEmpty())
    {
        return;
    }

    CString *vMems[] = {&m_szMovieName, &m_szPhoneNum, &m_szIDNum, &m_szDate, &m_szTime, &m_szRow, &m_szColumn, &m_szMovieNum};

    int iPos = 0;
    for (int i=0; i<_countof(vMems); i++)
    {
        *vMems[i] = szData.Tokenize("/", iPos);
    }

    UpdateData(FALSE);
}