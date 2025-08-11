// CTasksDialog.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "CTasksDialog.h"
#include "Resource.h"


// CTasksDialog dialog

IMPLEMENT_DYNAMIC(CTasksDialog, CDialogEx)

CTasksDialog::CTasksDialog(CUsersTypedPtrArray& oUsersArray, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EDB_TASKS_DIALOG, pParent), m_oUsersArray(oUsersArray)
{

}

CTasksDialog::~CTasksDialog()
{
}

void CTasksDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_USERS, m_oUsersComboBox);
}


BEGIN_MESSAGE_MAP(CTasksDialog, CDialogEx)
END_MESSAGE_MAP()


// CTasksDialog message handlers

BOOL CTasksDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for (int i = 0; i < m_oUsersArray.GetCount(); i++)
	{
		if (m_oUsersArray[i] == nullptr)
			continue;

		USERS* pUser = m_oUsersArray[i];
		int index = m_oUsersComboBox.AddString(pUser->szName);
		m_oUsersComboBox.SetItemData(index, (DWORD_PTR)pUser->lId);
	};

	return TRUE;
}
