// ImportPhotoshopDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImportPhotoshop.h"
#include "ImportPhotoshopDlg.h"
#include ".\importphotoshopdlg.h"

#include <string>
#include <math.h>
#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CImportPhotoshopDlg dialog



CImportPhotoshopDlg::CImportPhotoshopDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImportPhotoshopDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


	mnSuccess = -1;
}

CImportPhotoshopDlg::~CImportPhotoshopDlg()
{

}

void CImportPhotoshopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CImportPhotoshopDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CImportPhotoshopDlg message handlers

BOOL CImportPhotoshopDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CImportPhotoshopDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CImportPhotoshopDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		if ( 0 == mnSuccess )
		{
			CWnd* pWnd = GetDlgItem(IDC_STATIC_IMAGE);
			if ( pWnd )
			{
				CPaintDC dc(this); // device context for painting

				CRect rc;
				pWnd->GetWindowRect(&rc);
				ScreenToClient(&rc);

				int cx, cy;
				psd.Dimensions(cx, cy);

				HDC hClientDC = dc.GetSafeHdc();
				HDC hDC = ::CreateCompatibleDC( hClientDC );
				::SelectObject( hDC, psd.Detach() );

				if ( cx >= cy)
					::StretchBlt(hClientDC, rc.left, rc.top, rc.Width(), rc.Width()*cy/cx, hDC, 0, 0, cx, cy, SRCCOPY);
				else
					::StretchBlt(hClientDC, rc.left, rc.top, rc.Height()*cy/cx, rc.Height(), hDC, 0, 0, cx, cy, SRCCOPY);
				::DeleteDC( hDC);
			}
		}
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CImportPhotoshopDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CImportPhotoshopDlg::OnBnClickedButton1()
{
	CFileDialog dlg(TRUE, _T(".psd") );

	if ( IDOK == dlg.DoModal() )
	{
		CString cstrFileName = dlg.GetPathName();
		SetDlgItemText(IDC_EDIT_FILENAME, cstrFileName);

		bool bSuccess = ReadFile(cstrFileName);
	}

}

bool CImportPhotoshopDlg::ReadFile(CString cstrFileName)
{
	FILE* pFile = 0;
	bool bSuccess = false;

	mnSuccess = psd.Load(cstrFileName);
	if ( 0 == mnSuccess )
	{
		CString cstr, cstrMode;

		CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST);
		if ( pList )
			pList->ResetContent();

		int nDPI_x, nDPI_y;
		int nWidth, nHeight;

		psd.Dimensions(nWidth, nHeight);
		psd.DPI(nDPI_x, nDPI_y);

		if ( pList )
		{
			cstr.Format(_T("Bits/pixel = %d"), psd.BitsPerPixel() );
			pList->AddString(cstr);
			
			cstr.Format(_T("nWidth(pixels) = %d"), nWidth);
			pList->AddString(cstr);
			cstr.Format(_T("nHeight(pixels) = %d"), nHeight);
			pList->AddString(cstr);

			pList->AddString(_T("---"));
			cstr.Format(_T("Horizontal Pixels/inch = %d"), nDPI_x);
			pList->AddString(cstr);

			cstr.Format(_T("Vertical Pixels/inch = %d"), nDPI_y);
			pList->AddString(cstr);

		}

		Invalidate();
	}


	return bSuccess;

}
