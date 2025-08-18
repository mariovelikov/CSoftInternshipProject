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
	//DDV_MaxChars(pDX, m_strName, NAME_LENGTH);
}

void CLoginDialog::OnBnClicked() {
	CDialogEx::OnOK();
}


bool CLoginDialog::ValidateInput()
{
	//bool bValidate = true;
	//int nNameLength = sizeof(m_oUser.szName) / sizeof(TCHAR) - 1;
	//int nJobTitleLength = sizeof(m_oUser.szJobTitle) / sizeof(TCHAR) - 1;

	//CString strInputErrors;

	//// name
	//if (m_strName.IsEmpty() || m_strName.GetLength() > nNameLength) {
	//	if (m_strName.IsEmpty())
	//	{
	//		strInputErrors.Append(_T("Name box is empty !\n"));
	//	}
	//	else {
	//		strInputErrors.AppendFormat(_T("Name length is bigger than %d !\n"), nNameLength);
	//	}
	//	bValidate = false;
	//}

	//// Email
	//if (m_strEmail.Find(_T("@")) == -1 || m_strEmail.Find(_T(".")) == -1)
	//{
	//	strInputErrors.Append(_T("Your email is incorrect or empty !\n"));
	//	bValidate = false;
	//}

	////Job Title
	//if (m_strJobsTitle.IsEmpty() || m_strJobsTitle.GetLength() > nJobTitleLength)
	//{
	//	if (m_strJobsTitle.IsEmpty())
	//	{
	//		strInputErrors.Append(_T("Jobs title box is empty !\n"));
	//	}
	//	else {
	//		strInputErrors.AppendFormat(_T("Jobs title box length is bigger than %d !\n"), nJobTitleLength);
	//	}
	//	bValidate = false;
	//}

	//if (!strInputErrors.IsEmpty())
	//{
	//	AfxMessageBox(strInputErrors);
	//}

	//return bValidate;
	return false;
}

void CLoginDialog::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}
