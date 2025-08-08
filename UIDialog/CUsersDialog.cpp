// CUsersDialog.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "CUsersDialog.h"
#include "Resource.h"

#define NAME_LENGTH 64
#define EMAIL_LENGTH 128
#define JOBS_TITLE_LENGTH 128


// CUsersDialog dialog
IMPLEMENT_DYNAMIC(CUsersDialog, CDialogEx)

BEGIN_MESSAGE_MAP(CUsersDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CUsersDialog::OnBnClicked)
END_MESSAGE_MAP()

CUsersDialog::CUsersDialog(USERS& oUser, ViewActions eViewAction, bool bHideOkButton, bool bHideUpdateButton, CWnd* pParent /* = nullptr*/)
	: CDialogEx(IDD_EDB_USERS_DIALOG, pParent), m_oUser(oUser), 
	m_eCurrentAction(eViewAction), m_bHideOkButton(bHideOkButton), m_bHideUpdateButton(bHideUpdateButton)
{
	m_strName = m_oUser.szName;
	m_strEmail = m_oUser.szEmail;
	m_strJobsTitle = m_oUser.szJobTitle;
}

BOOL CUsersDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (m_bHideOkButton)
	{
		CWnd* pUpdateButton = GetDlgItem(IDOK);
		if (pUpdateButton)
			pUpdateButton->SetWindowText(_T("Update"));
	}

	if (m_eCurrentAction == ViewDetails) {
		SetReadOnlyControls();
	}

	return TRUE;
}


CUsersDialog::~CUsersDialog()
{
}

void CUsersDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, EDB_NAME, m_strName);
	DDX_Text(pDX, EDB_EMAIL, m_strEmail);
	DDX_Text(pDX, EDB_JOBS_TITLE, m_strJobsTitle);

	DDV_MaxChars(pDX, m_strName, NAME_LENGTH);
	DDV_MaxChars(pDX, m_strEmail, EMAIL_LENGTH);
	DDV_MaxChars(pDX, m_strJobsTitle, JOBS_TITLE_LENGTH);
}

void CUsersDialog::OnBnClicked() {
	UpdateData(TRUE);
	
	switch (m_eCurrentAction)
	{
		case ViewAdd:
			AddUser();
			break;
		case ViewUpdate:
			UpdateUser();
			break;
	default:
		break;
	}
}

void CUsersDialog::AddUser()
{
	UpdateData(TRUE);

	if (ValidateInput())
	{
		SetValuesToMemberUser();
		CDialogEx::OnOK();
	}
}

void CUsersDialog::UpdateUser()
{
	UpdateData(TRUE);

	if (ValidateInput()) {
		SetValuesToMemberUser();
		CDialogEx::OnOK();
	}
}

bool CUsersDialog::ValidateInput()
{
	bool bValidate = true;
	int nNameLength = sizeof(m_oUser.szName) / sizeof(TCHAR) - 1;
	int nJobTitleLength = sizeof(m_oUser.szJobTitle) / sizeof(TCHAR) - 1;
	
	CString strInputErrors;

	// name
	if (m_strName.IsEmpty() || m_strName.GetLength() > nNameLength) {
		if (m_strName.IsEmpty())
		{
			strInputErrors.Append(_T("Name box is empty !\n"));
		}
		else {
			strInputErrors.AppendFormat(_T("Name length is bigger than %d !\n"), nNameLength);
		}
		bValidate = false;
	}

	// Email
	if (m_strEmail.Find(_T("@")) == -1 || m_strEmail.Find(_T(".")) == -1)
	{
		strInputErrors.Append(_T("Your email is incorrect or empty !\n"));
		bValidate = false;
	}

	//Job Title
	if (m_strJobsTitle.IsEmpty() || m_strJobsTitle.GetLength() > nJobTitleLength)
	{
		if (m_strJobsTitle.IsEmpty())
		{
			strInputErrors.Append(_T("Jobs title box is empty !\n"));
		}
		else {
			strInputErrors.AppendFormat(_T("Jobs title box length is bigger than %d !\n"), nJobTitleLength);
		}
		bValidate = false;
	}

	if (!strInputErrors.IsEmpty())
	{
		AfxMessageBox(strInputErrors);
	}

	return bValidate;
}

void CUsersDialog::SetReadOnlyControls()
{
	CEdit* pJobsTitleBox = reinterpret_cast<CEdit*>(GetDlgItem(EDB_JOBS_TITLE));
	if (pJobsTitleBox != nullptr) {
		pJobsTitleBox->SetReadOnly(TRUE);
	}
	CEdit* pNameBox = reinterpret_cast<CEdit*>(GetDlgItem(EDB_NAME));
	if (pNameBox != nullptr) {
		pNameBox->SetReadOnly(TRUE);
	}
	CEdit* pEmail = reinterpret_cast<CEdit*>(GetDlgItem(EDB_EMAIL));
	if (pEmail != nullptr) {
		pEmail->SetReadOnly(TRUE);
	}
}

void CUsersDialog::SetValuesToMemberUser()
{
	wcsncpy_s(m_oUser.szName, m_strName, sizeof(m_oUser.szName) / sizeof(TCHAR) - 1);
	wcsncpy_s(m_oUser.szEmail, m_strEmail, sizeof(m_oUser.szEmail) / sizeof(TCHAR) - 1);
	wcsncpy_s(m_oUser.szJobTitle, m_strJobsTitle, sizeof(m_oUser.szJobTitle) / sizeof(TCHAR) - 1);
}