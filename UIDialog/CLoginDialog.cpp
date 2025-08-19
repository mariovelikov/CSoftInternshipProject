#include "pch.h"
#include "CLoginDialog.h"
#include "Resource.h"

#define NAME_LENGTH 64
#define EMAIL_LENGTH 128
#define JOBS_TITLE_LENGTH 128


// CUsersDialog dialog
IMPLEMENT_DYNAMIC(CLoginDialog, CDialogEx)

BEGIN_MESSAGE_MAP(CLoginDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLoginDialog::OnBnClicked)
	ON_BN_CLICKED(IDCANCEL, &CLoginDialog::OnBnClickedCancel)
END_MESSAGE_MAP()

CLoginDialog::CLoginDialog(CString& strEmail, CString& strPassword, CWnd* pParent /*nullptr*/ )
	: CDialogEx(IDD_EDB_LOGIN_DIALOG, pParent), m_strEmail(strEmail), m_strPassword(strPassword)
{
}

BOOL CLoginDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	GetDlgItem(IDC_EDB_EMAIL)->SetFocus();
	return FALSE;
}


CLoginDialog::~CLoginDialog()
{
}

void CLoginDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDB_EMAIL, m_strEmail);
	DDX_Text(pDX, IDC_EDB_PASSWORD, m_strPassword);

	DDV_MaxChars(pDX, m_strEmail, EMAIL_LENGTH);
}

void CLoginDialog::OnBnClicked() {
	UpdateData(TRUE); 
	if (ValidateInput())
	{
		CDialogEx::OnOK();
	};
}


bool CLoginDialog::ValidateInput()
{
	CString errorMessage;
	if (m_strEmail.IsEmpty())
	{
		errorMessage = _T("Email cannot be empty.\n");
	}

	if (m_strPassword.IsEmpty())
	{
		errorMessage += _T("Password cannot be empty.");
	}

	if (!errorMessage.IsEmpty())
	{
		AfxMessageBox(errorMessage);
		return false;
	}

	return true;
}

void CLoginDialog::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}
