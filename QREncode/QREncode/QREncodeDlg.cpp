// QREncodeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "QREncode.h"
#include "QREncodeDlg.h"

#include "QR_Encode.h"

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


// CQREncodeDlg 对话框




CQREncodeDlg::CQREncodeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQREncodeDlg::IDD, pParent)
    , m_szMovieName(_T("MovieName"))
    , m_szPhoneNum(_T("022"))
    , m_szIDNum(_T("09201401"))
    , m_date(0)
    , m_time(0)
    , m_szRow(_T("A"))
    , m_szColumn(_T("1"))
    , m_szMovieNumber(_T("1"))
    , m_bDataEncoded(FALSE)
    , m_pSymbleDC(NULL)
    , m_pSymbleBitmap(NULL)
    , m_pOldBitmap(NULL)
    , m_iModuleSize(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    CTime t = CTime::GetCurrentTime();
    m_date = t;
    m_time = t;
}

CQREncodeDlg::~CQREncodeDlg()
{
    if (m_pSymbleDC != NULL)
    {
        m_pSymbleDC->SelectObject(m_pOldBitmap);

        delete m_pSymbleDC;
    }

    if (m_pSymbleBitmap != NULL)
    {
        delete m_pSymbleBitmap;
    }
}

void CQREncodeDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO3, m_comboVersion);
    DDX_Control(pDX, IDC_COMBO4, m_comboLevel);
    DDX_Text(pDX, IDC_EDIT1, m_szMovieName);
    DDX_Text(pDX, IDC_EDIT2, m_szPhoneNum);
    DDV_MaxChars(pDX, m_szMovieName, 32);
    DDV_MaxChars(pDX, m_szPhoneNum, 16);
    DDX_Text(pDX, IDC_EDIT3, m_szIDNum);
    DDV_MaxChars(pDX, m_szIDNum, 24);
    DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_date);
    DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_time);
    DDX_CBString(pDX, IDC_COMBO1, m_szRow);
    DDX_CBString(pDX, IDC_COMBO2, m_szColumn);
    DDX_Text(pDX, IDC_EDIT4, m_szMovieNumber);
    DDX_CBIndex(pDX, IDC_COMBO5, m_iModuleSize);
	DDV_MinMaxInt(pDX, m_iModuleSize, 0, 19);
}

BEGIN_MESSAGE_MAP(CQREncodeDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_EN_CHANGE(IDC_EDIT1, &CQREncodeDlg::OnEnChangeEdit1)
    ON_EN_CHANGE(IDC_EDIT2, &CQREncodeDlg::OnEnChangeEdit2)
    ON_EN_CHANGE(IDC_EDIT3, &CQREncodeDlg::OnEnChangeEdit3)
    ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, &CQREncodeDlg::OnDtnDatetimechangeDatetimepicker1)
    ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER2, &CQREncodeDlg::OnDtnDatetimechangeDatetimepicker2)
    ON_CBN_SELCHANGE(IDC_COMBO1, &CQREncodeDlg::OnCbnSelchangeCombo1)
    ON_CBN_SELCHANGE(IDC_COMBO2, &CQREncodeDlg::OnCbnSelchangeCombo2)
    ON_EN_CHANGE(IDC_EDIT4, &CQREncodeDlg::OnEnChangeEdit4)
    ON_CBN_SELCHANGE(IDC_COMBO3, &CQREncodeDlg::OnCbnSelchangeCombo3)
    ON_CBN_SELCHANGE(IDC_COMBO4, &CQREncodeDlg::OnCbnSelchangeCombo4)
    ON_BN_CLICKED(IDOK, &CQREncodeDlg::OnBnClickedOk)
	ON_STN_CLICKED(IDC_STATIC_IMAGE, &CQREncodeDlg::OnStnClickedStaticImage)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CQREncodeDlg::OnCbnSelchangeCombo5)
END_MESSAGE_MAP()


// CQREncodeDlg 消息处理程序

BOOL CQREncodeDlg::OnInitDialog()
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
    m_comboLevel.SetCurSel(0);
    m_comboVersion.SetCurSel(0);
    ((CComboBox*)GetDlgItem(IDC_COMBO5))->SetCurSel(3);

    ShowImage();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CQREncodeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CQREncodeDlg::OnPaint()
{
    CPaintDC dc(this); // 用于绘制的设备上下文
    CRect rect;
    GetClientRect(&rect);

	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		DrawImage();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CQREncodeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CQREncodeDlg::GetFormData(CString &szData)
{
    UpdateData();
    CString szTemp;
    szTemp = m_szMovieName + "/" +
             m_szPhoneNum + "/" +
             m_szIDNum + "/" +
             m_date.Format("%Y-%m-%d") + "/" +
             m_time.Format("%H:%M:%S") + "/" +
             m_szRow + "/" + 
             m_szColumn + "/" +
             m_szMovieNumber;

    szData = szTemp;
}

void CQREncodeDlg::ShowImage()
{
    int nLevel = m_comboLevel.GetCurSel();
    int nVersion = m_comboVersion.GetCurSel();
    BOOL bAutoExtent = 1;   // 只用这个
    int nMaskingNo   =  -1; // 只用这个

    CString strEncodeData;
    GetFormData(strEncodeData);

    int i, j;

    CQR_Encode* pQR_Encode = new CQR_Encode;

    m_bDataEncoded = pQR_Encode->EncodeData(nLevel, nVersion, bAutoExtent, nMaskingNo, strEncodeData);

    if (m_bDataEncoded)
    {
        if (m_pSymbleDC != NULL)
        {
            m_pSymbleDC->SelectObject(m_pOldBitmap);

            delete m_pSymbleBitmap;
            delete m_pSymbleDC;
        }

        m_nSymbleSize = pQR_Encode->m_nSymbleSize + (QR_MARGIN * 2);

        m_pSymbleBitmap = new CBitmap;
        m_pSymbleBitmap->CreateBitmap(m_nSymbleSize, m_nSymbleSize, 1, 1, NULL);

        m_pSymbleDC = new CDC;
        m_pSymbleDC->CreateCompatibleDC(NULL);

        m_pOldBitmap = m_pSymbleDC->SelectObject(m_pSymbleBitmap);

        m_pSymbleDC->PatBlt(0, 0, m_nSymbleSize, m_nSymbleSize, WHITENESS);

        for (i = 0; i < pQR_Encode->m_nSymbleSize; ++i)
        {
            for (j = 0; j < pQR_Encode->m_nSymbleSize; ++j)
            {
                if (pQR_Encode->m_byModuleData[i][j])
                {
                    m_pSymbleDC->SetPixel(i + QR_MARGIN, j + QR_MARGIN, RGB(0, 0, 0));
                }
            }
        }

        DrawImage();
    }
    else
    {
        MessageBox("编码失败", "错误", MB_OK | MB_ICONINFORMATION);
    }

    delete pQR_Encode;
}

void CQREncodeDlg::DrawImage()
{
    CDC *pDC = GetDC();
    CRect rt;
    GetDlgItem(IDC_STATIC_IMAGE)->GetClientRect(&rt);
    GetDlgItem(IDC_STATIC_IMAGE)->MapWindowPoints(this, &rt);

    if (m_bDataEncoded)
    {
        pDC->StretchBlt(rt.left, rt.top, rt.Width(), rt.Height(), m_pSymbleDC, 0, 0, m_nSymbleSize, m_nSymbleSize, SRCCOPY);
    }
    else
    {
        pDC->FillSolidRect(rt.left, rt.top, rt.Width(), rt.Height(), RGB(0, 0, 0));
    }

    ReleaseDC(pDC);
}
void CQREncodeDlg::OnEnChangeEdit1()
{
    ShowImage();
}

void CQREncodeDlg::OnEnChangeEdit2()
{
    ShowImage();
}

void CQREncodeDlg::OnEnChangeEdit3()
{
    ShowImage();
}

void CQREncodeDlg::OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
    ShowImage();
    *pResult = 0;
}

void CQREncodeDlg::OnDtnDatetimechangeDatetimepicker2(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
     ShowImage();
    *pResult = 0;
}

void CQREncodeDlg::OnCbnSelchangeCombo1()
{
    ShowImage();
}

void CQREncodeDlg::OnCbnSelchangeCombo2()
{
    ShowImage();
}

void CQREncodeDlg::OnEnChangeEdit4()
{
    ShowImage();
}

void CQREncodeDlg::OnCbnSelchangeCombo3()
{
     ShowImage();
}

void CQREncodeDlg::OnCbnSelchangeCombo4()
{
     ShowImage();
}

void CQREncodeDlg::SaveImage()
{
    int nPixelSize = m_nSymbleSize * (m_iModuleSize + 1);

    int nBmpSize = ((nPixelSize + 31) / 32) * 32 / 8;
    nBmpSize *= nPixelSize;

    HANDLE hFile = CreateFile(m_strSavePath, GENERIC_READ | GENERIC_WRITE, 0, NULL,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        m_strSavePath.Empty();
        MessageBox("保存失败", "错误", MB_ICONSTOP);
            return;
    }

    HANDLE hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE | SEC_COMMIT, 0, 
        sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 2 + nBmpSize, NULL);
    LPBYTE lpbyMapView = (LPBYTE)MapViewOfFile(hFileMapping, FILE_MAP_WRITE, 0, 0, 0);

    ZeroMemory(lpbyMapView, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 2 + nBmpSize);

    LPBITMAPFILEHEADER pbfh = (LPBITMAPFILEHEADER)lpbyMapView;
    LPBITMAPINFO       pbmi = (LPBITMAPINFO)(lpbyMapView + sizeof(BITMAPFILEHEADER));
    LPVOID             pbdt = lpbyMapView + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 2;

    // BITMAPFILEHEADER
    pbfh->bfType      = (WORD) (('M' << 8) | 'B'); // "BM"
    pbfh->bfSize      = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 2 + nBmpSize;
    pbfh->bfReserved1 = 0;
    pbfh->bfReserved2 = 0;
    pbfh->bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 2;

    // LPBITMAPINFO->BITMAPINFOHEADER
    pbmi->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
    pbmi->bmiHeader.biWidth			= nPixelSize;
    pbmi->bmiHeader.biHeight		= nPixelSize;
    pbmi->bmiHeader.biPlanes		= 1;
    pbmi->bmiHeader.biBitCount		= 1;
    pbmi->bmiHeader.biCompression	= BI_RGB;
    pbmi->bmiHeader.biSizeImage		= nBmpSize;
    pbmi->bmiHeader.biXPelsPerMeter = 3780;
    pbmi->bmiHeader.biYPelsPerMeter = 3780;
    pbmi->bmiHeader.biClrUsed		= 0;
    pbmi->bmiHeader.biClrImportant	= 0;

    CDC* pWorkDC = new CDC;
    pWorkDC->CreateCompatibleDC(NULL);

    CBitmap* pWorkBitmap = new CBitmap;
    pWorkBitmap->CreateBitmap(nPixelSize, nPixelSize, 1, 1, NULL);

    CBitmap* pOldBitmap = pWorkDC->SelectObject(pWorkBitmap);
    pWorkDC->StretchBlt(0, 0, nPixelSize, nPixelSize, m_pSymbleDC, 0, 0, m_nSymbleSize, m_nSymbleSize, SRCCOPY);

    GetDIBits(pWorkDC->m_hDC, (HBITMAP)*pWorkBitmap, 0, nPixelSize, pbdt, pbmi, DIB_RGB_COLORS);

    pWorkDC->SelectObject(pOldBitmap);
    delete pWorkBitmap;
    delete pWorkDC;

    UnmapViewOfFile(lpbyMapView);
    CloseHandle(hFileMapping);
    CloseHandle(hFile);
}

void CQREncodeDlg::OnBnClickedOk()
{
    if (!m_bDataEncoded)
    {
        MessageBox("请先编码", "失败", MB_OK | MB_ICONINFORMATION);
        return;
    }

    UpdateData();

    CFileDialog dlg(FALSE, "bmp", m_szMovieName + "_" + m_szPhoneNum + ".bmp", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "位图格式(*.bmp)|*.bmp||");
    if (dlg.DoModal() == IDOK)    
    {
        m_strSavePath = dlg.GetPathName();
        SaveImage();
    }
}

void CQREncodeDlg::OnStnClickedStaticImage()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CQREncodeDlg::OnCbnSelchangeCombo5()
{
	// TODO: 在此添加控件通知处理程序代码
}
