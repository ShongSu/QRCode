// QRDecodeDlg.h : 头文件
//
 
#pragma once
#include "QRcodeImage.h"
#include "ContentDecoder.h"
#include "resource.h"

// CQRDecodeDlg 对话框
class CQRDecodeDlg : public CDialog
{
// 构造
public:
	CQRDecodeDlg(CWnd* pParent = NULL);	// 标准构造函数
    ~CQRDecodeDlg();

// 对话框数据
	enum { IDD = IDD_QRDECODE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

    afx_msg void OnBnClickedOk();

public:

private:
    void StringSplit(CString &szData);

    QRcodeImage * QR_image;
    ContentDecoder * QR_Decode;

    int m_nCodeSize;            //编码区域大小
    int m_nMoudleSize;          //模块大小
    int m_nVersion;             //版本号
    int m_nLevel;               //纠错等级
    bool flag_draw;             //绘图标志
    int nShowSize;
    BYTE m_CodeData[MAX_MODULESIZE][MAX_MODULESIZE]; // 存储条形码位数据

    CString m_szMovieName;
    CString m_szPhoneNum;
    CString m_szIDNum;
    CString m_szRow;
    CString m_szColumn;
    CString m_szMovieNum;
    CString m_szVersion;
    CString m_szLevel;
    CString m_szDate;
    CString m_szTime;
};
