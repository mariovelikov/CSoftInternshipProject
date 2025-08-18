#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "ClientApp.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "CSoftInternshipProjectDoc.h"
#include "CSoftInternshipProjectView.h"

#include "DLLExport.h"
#include "CUsersDocument.h"
#include "CUsersView.h"
#include "CProjectsDocument.h"
#include "CProjectsView.h"
#include "CLoginDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(ClientApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &ClientApp::OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()

ClientApp::ClientApp() noexcept
{
	SetAppID(_T("CSoftInternshipProject.AppID.NoVersion"));
}

ClientApp theApp;


BOOL ClientApp::InitInstance()
{
	CWinApp::InitInstance();

	EnableTaskbarInteraction(FALSE);

	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);

	HRESULT hResult = CoInitializeEx(NULL, 0);
	if (hResult != S_OK)
		return FALSE;
	
	CString strEmail, strPassword;
	CLoginDialog oLoginDialog(strEmail, strPassword);
	INT_PTR nBtnClick = oLoginDialog.DoModal();
	if (nBtnClick != IDOK)
	{
		return FALSE;
	};
	
	if (!CUsersDocument::ClientAuthentication(strEmail, strPassword))
	{
		for (int i = 0; i < 3; i++)
		{
			AfxMessageBox(_T("Authentication failed. Please try again."));
			if (i == 2)
				return FALSE;
			
			nBtnClick = oLoginDialog.DoModal();
			if (nBtnClick != IDOK)
			{
				return FALSE;
			};

			if (CUsersDocument::ClientAuthentication(strEmail, strPassword))
				break;
		}
	}

	CMultiDocTemplate* pDocTemplate = nullptr;
	pDocTemplate = new CMultiDocTemplate(IDR_CSoftInternshipProjectTYPE,
		RUNTIME_CLASS(CUsersDocument),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(CUsersView));

	CMultiDocTemplate* pProjectsDocTemplate = nullptr;
	pProjectsDocTemplate = new CMultiDocTemplate(IDR_CSoftInternshipProjectTYPE,
		RUNTIME_CLASS(CProjectsDocument),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(CProjectsView));

	if (!pDocTemplate)
		return FALSE;

	if (!pProjectsDocTemplate)
		return FALSE;

	AddDocTemplate(pDocTemplate);
	AddDocTemplate(pProjectsDocTemplate);

	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if (!ProcessShellCommand(cmdInfo))
		return FALSE;



	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();
	
	return TRUE;
}

int ClientApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

void ClientApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
};