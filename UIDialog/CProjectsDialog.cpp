// CProjectsDialog.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "CProjectsDialog.h"
#include "CTasksDialog.h"
#include "Resource.h"
#include "STRUCTURES.h"


#define ID_ADD_TASK 1001


BEGIN_MESSAGE_MAP(CProjectsDialog, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LSC_TASKS, &CProjectsDialog::OnListTasksRightClick)
	ON_COMMAND(ID_ADD_TASK, &CProjectsDialog::OnAddTask)
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CProjectsDialog, CDialogEx)


//Constructor / Destructor
//----------------
CProjectsDialog::CProjectsDialog(CUsersTypedPtrArray& oUsersArray, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EDB_PROJECTS_DIALOG, pParent), m_oUsersArray(oUsersArray)
{

}

CProjectsDialog::~CProjectsDialog()
{
}

void CProjectsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_USERS, m_oProjectManagerComboBox);
	DDX_Control(pDX, IDC_LSC_TASKS, m_oListTasks);
}

void CProjectsDialog::OnListTasksRightClick(NMHDR* pNMHDR, LRESULT* pResult)
{
    CPoint oPoint;
    GetCursorPos(&oPoint);

	CMenu oContextMenu;
	oContextMenu.CreatePopupMenu();

	int nSelectedItem = m_oListTasks.GetNextItem(-1, LVNI_SELECTED);
    if (nSelectedItem != -1)
    {
	}
	else {
		oContextMenu.AppendMenuW(MF_STRING, ID_ADD_TASK, _T("Add Tasks"));
	}


	oContextMenu.TrackPopupMenu(
		TPM_LEFTALIGN | TPM_RIGHTBUTTON,
		oPoint.x, oPoint.y, this);

    if (pResult)
        *pResult = 0;
}

BOOL CProjectsDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_oListTasks.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 50);
	m_oListTasks.InsertColumn(1, _T("Name"), LVCFMT_LEFT, 150);
	m_oListTasks.InsertColumn(2, _T("Total Effort"), LVCFMT_LEFT, 200);
	m_oListTasks.InsertColumn(3, _T("State"), LVCFMT_LEFT, 100);

	m_oListTasks.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	for (int i = 0; i < m_oUsersArray.GetCount(); i++)
	{
		USERS* pUser = m_oUsersArray.GetAt(i);

		int index = m_oProjectManagerComboBox.AddString(pUser->szName);
		m_oProjectManagerComboBox.SetItemData(index, (DWORD_PTR)pUser->lId);
	}

	return TRUE;
}


void CProjectsDialog::OnAddTask() 
{
	CTasksDialog oTasksDialog(m_oUsersArray);

	if (oTasksDialog.DoModal() == IDOK)
	{
		m_oListTasks.InsertItem(0, _T("New Task"));
		//m_oTasksList.Add(new TASKS());
		AfxMessageBox(_T("Task added successfully!"));
	}
	else
	{
		AfxMessageBox(_T("No task was added."));
	}
}