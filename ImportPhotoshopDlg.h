// ImportPhotoshopDlg.h : header file
//

#pragma once

#include "MyPSD.h"

//class CDib;

// CImportPhotoshopDlg dialog
class CImportPhotoshopDlg : public CDialog
{
// Construction
public:
	CImportPhotoshopDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CImportPhotoshopDlg();
// Dialog Data
	enum { IDD = IDD_IMPORTPHOTOSHOP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();

private:
	bool ReadFile(CString cstrFileName);

private:
	int mnSuccess;
	MyPSD::CPSD psd;

};
