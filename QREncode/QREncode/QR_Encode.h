// QR_Encode.h : CQR_Encode 僋儔僗愰尵偍傛傃僀儞僞乕僼僃僀僗掕媊
// Date 2006/05/17	Ver. 1.22	Psytec Inc.

#if !defined(AFX_QR_ENCODE_H__AC886DF7_C0AE_4C9F_AC7A_FCDA8CB1DD37__INCLUDED_)
#define AFX_QR_ENCODE_H__AC886DF7_C0AE_4C9F_AC7A_FCDA8CB1DD37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// 掕悢

// 岆傝掶惓儗儀儖
#define QR_LEVEL_L	0
#define QR_LEVEL_M	1
#define QR_LEVEL_Q	2
#define QR_LEVEL_H	3

// 编码方式 
#define QR_MODE_NUMERAL		0    // 数字模式
#define QR_MODE_ALPHABET	1    // 字母数字模式
#define QR_MODE_8BIT		2    // 8位字节模式
#define QR_MODE_KANJI		3    // 汉字模式

// 版本分组
#define QR_VRESION_S	0 // 1 到 9
#define QR_VRESION_M	1 // 10 到 26
#define QR_VRESION_L	2 // 27 到 40

#define MAX_ALLCODEWORD	 3706 // 憤僐乕僪儚乕僪悢嵟戝抣
#define MAX_DATACODEWORD 2956 // 僨乕僞僐乕僪儚乕僪嵟戝抣(僶乕僕儑儞40-L)
#define MAX_CODEBLOCK	  153 // 僽儘僢僋僨乕僞僐乕僪儚乕僪悢嵟戝抣(俼俽僐乕僪儚乕僪傪娷傓)
#define MAX_MODULESIZE	  177 // 堦曈儌僕儏乕儖悢嵟戝抣

// 價僢僩儅僢僾昤夋帪儅乕僕儞
#define QR_MARGIN	4


/////////////////////////////////////////////////////////////////////////////
typedef struct tagRS_BLOCKINFO
{
	int ncRSBlock;		// 俼俽僽儘僢僋悢
	int ncAllCodeWord;	// 僽儘僢僋撪僐乕僪儚乕僪悢
	int ncDataCodeWord;	// 僨乕僞僐乕僪儚乕僪悢(僐乕僪儚乕僪悢 - 俼俽僐乕僪儚乕僪悢)

} RS_BLOCKINFO, *LPRS_BLOCKINFO;


/////////////////////////////////////////////////////////////////////////////
// QR僐乕僪僶乕僕儑儞(宆斣)娭楢忣曬

typedef struct tagQR_VERSIONINFO
{
	int nVersionNo;	   // 僶乕僕儑儞(宆斣)斣崋(1乣40)
	int ncAllCodeWord; // 憤僐乕僪儚乕僪悢

	// 埲壓攝楍揧帤偼岆傝掶惓棪(0 = L, 1 = M, 2 = Q, 3 = H) 
	int ncDataCodeWord[4];	// 僨乕僞僐乕僪儚乕僪悢(憤僐乕僪儚乕僪悢 - 俼俽僐乕僪儚乕僪悢)

	int ncAlignPoint;	// 傾儔僀儊儞僩僷僞乕儞嵗昗悢
	int nAlignPoint[6];	// 傾儔僀儊儞僩僷僞乕儞拞怱嵗昗

	RS_BLOCKINFO RS_BlockInfo1[4]; // 俼俽僽儘僢僋忣曬(1)
	RS_BLOCKINFO RS_BlockInfo2[4]; // 俼俽僽儘僢僋忣曬(2)

} QR_VERSIONINFO, *LPQR_VERSIONINFO;


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode 僋儔僗

class CQR_Encode
{
// 峔抸/徚柵
public:
	CQR_Encode();
	~CQR_Encode();

public:
	int m_nLevel;		// 岆傝掶惓儗儀儖
	int m_nVersion;		// 僶乕僕儑儞(宆斣)
	BOOL m_bAutoExtent;	// 僶乕僕儑儞(宆斣)帺摦奼挘巜掕僼儔僌
	int m_nMaskingNo;	// 儅僗僉儞僌僷僞乕儞斣崋

public:
	int m_nSymbleSize;
	BYTE m_byModuleData[MAX_MODULESIZE][MAX_MODULESIZE]; // [x][y]
	// bit5:婡擻儌僕儏乕儖乮儅僗僉儞僌懳徾奜乯僼儔僌
	// bit4:婡擻儌僕儏乕儖昤夋僨乕僞
	// bit1:僄儞僐乕僪僨乕僞
	// bit0:儅僗僋屻僄儞僐乕僪昤夋僨乕僞
	// 20h偲偺榑棟榓偵傛傝婡擻儌僕儏乕儖敾掕丄11h偲偺榑棟榓偵傛傝昤夋乮嵟廔揑偵偼BOOL抣壔乯

private:
	int m_ncDataCodeWordBit; // 僨乕僞僐乕僪儚乕僪價僢僩挿
	BYTE m_byDataCodeWord[MAX_DATACODEWORD]; // 擖椡僨乕僞僄儞僐乕僪僄儕傾

	int m_ncDataBlock;
	BYTE m_byBlockMode[MAX_DATACODEWORD];
	int m_nBlockLength[MAX_DATACODEWORD];

	int m_ncAllCodeWord; // 憤僐乕僪儚乕僪悢(俼俽岆傝掶惓僨乕僞傪娷傓)
	BYTE m_byAllCodeWord[MAX_ALLCODEWORD]; // 憤僐乕僪儚乕僪嶼弌僄儕傾
	BYTE m_byRSWork[MAX_CODEBLOCK]; // 俼俽僐乕僪儚乕僪嶼弌儚乕僋

// 僨乕僞僄儞僐乕僪娭楢僼傽儞僋僔儑儞
public:
	BOOL EncodeData(int nLevel, int nVersion, BOOL bAutoExtent, int nMaskingNo, LPCSTR lpsSource, int ncSource = 0);

private:
	int GetEncodeVersion(int nVersion, LPCSTR lpsSource, int ncLength);
	BOOL EncodeSourceData(LPCSTR lpsSource, int ncLength, int nVerGroup);

	int GetBitLength(BYTE nMode, int ncData, int nVerGroup);

	int SetBitStream(int nIndex, WORD wData, int ncData);

	BOOL IsNumeralData(unsigned char c);
	BOOL IsAlphabetData(unsigned char c);
	BOOL IsKanjiData(unsigned char c1, unsigned char c2);

	BYTE AlphabetToBinaly(unsigned char c);
	WORD KanjiToBinaly(WORD wc);

	void GetRSCodeWord(LPBYTE lpbyRSWork, int ncDataCodeWord, int ncRSCodeWord);

// 儌僕儏乕儖攝抲娭楢僼傽儞僋僔儑儞
private:
	void FormatModule();

	void SetFunctionModule();
	void SetFinderPattern(int x, int y);
	void SetAlignmentPattern(int x, int y);
	void SetVersionPattern();
	void SetCodeWordPattern();
	void SetMaskingPattern(int nPatternNo);
	void SetFormatInfoPattern(int nPatternNo);
	int CountPenalty();
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_QR_ENCODE_H__AC886DF7_C0AE_4C9F_AC7A_FCDA8CB1DD37__INCLUDED_)
