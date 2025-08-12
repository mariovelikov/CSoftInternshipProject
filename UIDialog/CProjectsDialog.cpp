// CProjectsDialog.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "CProjectsDialog.h"
#include "CTasksDialog.h"
#include "Resource.h"
#include "STRUCTURES.h"

#define ID_ADD_TASK 1001

#define TASKS_ID_COLUMN 0
#define TASKS_NAME_COLUMN 1
#define TASKS_DESCRIPTION_COLUMN 2
#define TASKS_TOTAL_EFFORT_COLUMN 3
#define TASKS_STATE_COLUMN 4

BEGIN_MESSAGE_MAP(CProjectsDialog, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LSC_TASKS, &CProjectsDialog::OnListTasksRightClick)
	ON_COMMAND(ID_ADD_TASK, &CProjectsDialog::OnAddTask)
	ON_BN_CLICKED(IDOK, &CProjectsDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CProjectsDialog::OnBnClickedCancel)
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CProjectsDialog, CDialogEx)


//Constructor / Destructor
//----------------
CProjectsDialog::CProjectsDialog(CUsersTypedPtrArray& oUsersArray, PROJECT_DETAILS& oProjectDetails, ViewActions eAction, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EDB_PROJECTS_DIALOG, pParent), m_oUsersArray(oUsersArray), m_oProjectDetails(oProjectDetails), m_eCurrentAction(eAction)
{
	m_strName = m_oProjectDetails.recProject.szName;
	m_strDescription = m_oProjectDetails.recProject.szDescription;
}

CProjectsDialog::~CProjectsDialog()
{
}

void CProjectsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_USERS, m_oUsersComboBox);
	DDX_Control(pDX, IDC_CMB_STATE, m_oStateComboBox);
	DDX_Control(pDX, IDC_LSC_TASKS, m_oListTasks);
	DDX_Text(pDX, IDC_EDB_PROJECTS_NAME, m_strName);
	DDX_Text(pDX, IDC_EDB_PROJECTS_DESCRIPTION, m_strDescription);

	DDV_MaxChars(pDX, m_strName, TASKS_NAME_LENGTH);
	DDV_MaxChars(pDX, m_strDescription, TASKS_DESCRIPTION_LENGTH);
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

	m_oListTasks.InsertColumn(TASKS_ID_COLUMN, _T("ID"), LVCFMT_LEFT, 50);
	m_oListTasks.InsertColumn(TASKS_NAME_COLUMN, _T("Name"), LVCFMT_LEFT, 100);
	m_oListTasks.InsertColumn(TASKS_DESCRIPTION_COLUMN, _T("Description"), LVCFMT_LEFT, 150);
	m_oListTasks.InsertColumn(TASKS_TOTAL_EFFORT_COLUMN, _T("Effort"), LVCFMT_LEFT, 100);
	m_oListTasks.InsertColumn(TASKS_STATE_COLUMN, _T("State"), LVCFMT_LEFT, 100);

	m_oListTasks.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_oUsersComboBox.ModifyStyle(CBS_SORT, 0);

	FillComboBoxes();

	switch (m_eCurrentAction)
	{
	case ViewDetails:
		ViewProjectDetails();
		break;

	default:
		break;
	}


	return TRUE;
}

void CProjectsDialog::ViewProjectDetails()
{
	// Fill tasks
	for (int i = 0; i < m_oProjectDetails.oTasksTypedPtrArray.GetCount(); i++)
	{
		VisualizeTask(*m_oProjectDetails.oTasksTypedPtrArray[i]);
	}

	// Set the correct selection based on m_oProjectDetails.recProject.nState
	for (int i = 0; i < m_oStateComboBox.GetCount(); ++i)
	{
		DWORD_PTR data = m_oStateComboBox.GetItemData(i);
		if ((ProjectsStateEnum)data == m_oProjectDetails.recProject.nState)
		{
			m_oStateComboBox.SetCurSel(i);
			break;
		}
	}
}

void CProjectsDialog::FillComboBoxes()
{
	// fil the combo box with users
	for (int i = 0; i < m_oUsersArray.GetCount(); i++)
	{
		if (m_oUsersArray[i] == nullptr)
			continue;

		USERS* pUser = m_oUsersArray[i];

		int nIndex = m_oUsersComboBox.AddString(pUser->szName);
		if (nIndex != CB_ERR)
		{
			m_oUsersComboBox.SetItemData(nIndex, static_cast<DWORD_PTR>(pUser->lId));
		}
	}

	if (m_oUsersComboBox.GetCount() > 0)
		m_oUsersComboBox.SetCurSel(0);

	int nIndex = m_oStateComboBox.AddString(_T("Active"));
	m_oStateComboBox.SetItemData(nIndex, Active);

	nIndex = m_oStateComboBox.AddString(_T("Finished"));
	m_oStateComboBox.SetItemData(nIndex, Finished);

	if (m_oStateComboBox.GetCount() > 0)
		m_oStateComboBox.SetCurSel(0);
}

void CProjectsDialog::OnAddTask()
{
	TASKS oTask;

	CTasksDialog oTasksDialog(m_oUsersArray, oTask);
	if (oTasksDialog.DoModal() == IDOK)
	{
		VisualizeTask(oTask);
		m_oProjectDetails.oTasksTypedPtrArray.Add(new TASKS(oTask));
		m_oProjectDetails.recProject.nTotalEffort += oTask.nEffort;
	}
	else
	{
		AfxMessageBox(_T("No task was added."));
	}
}

void CProjectsDialog::VisualizeTask(TASKS& oTask)
{
	int nIndex = m_oListTasks.InsertItem(1, _T(""));
	m_oListTasks.SetItemText(nIndex, TASKS_NAME_COLUMN, oTask.szName);
	m_oListTasks.SetItemText(nIndex, TASKS_DESCRIPTION_COLUMN, oTask.szDescription);

	CString strTotalEffort = _T("7");

	CString strState = StateToString((StateEnum)oTask.nState);
	m_oListTasks.SetItemText(nIndex, TASKS_STATE_COLUMN, strState);
}

void CProjectsDialog::OnBnClickedOk()
{
	UpdateData(TRUE);
	if (!ValidateData())
	{
		return;
	}
	FillProjectData();
	CDialogEx::OnOK();
}

void CProjectsDialog::FillProjectData()
{
	int nSelected = m_oUsersComboBox.GetCurSel();
	if (nSelected != CB_ERR) {
		m_oProjectDetails.recProject.lProjectManagerId = static_cast<long>(m_oUsersComboBox.GetItemData(nSelected));
	}

	nSelected = m_oStateComboBox.GetCurSel();
	if (nSelected != CB_ERR) {
		m_oProjectDetails.recProject.nState = static_cast<int>(m_oStateComboBox.GetItemData(nSelected));
	}

	_tcscpy_s(m_oProjectDetails.recProject.szName, PROJECTS_NAME_LENGTH, m_strName);
	_tcscpy_s(m_oProjectDetails.recProject.szDescription, PROJECTS_DESCRIPTION_LENGTH, m_strDescription);
}

CString CProjectsDialog::StateToString(StateEnum state)
{
	switch (state)
	{
	case Pending:
		return _T("Pending");
	case InProgress:
		return _T("InProgress");
	case Ended:
		return _T("Ended");
	default:
		return _T("Unknown");
	}
}

bool CProjectsDialog::ValidateData()
{
	// Validate the data entered in the dialog
	CString strErrorMessage;
	bool bValid = true;

	if (m_strName.IsEmpty())
	{
		strErrorMessage += _T("\nPlease enter a name for the project.");
		bValid = false;
	}
	if (m_strDescription.IsEmpty())
	{
		strErrorMessage += _T("\nPlease enter a description for the project.");
		bValid = false;
	}
	if (m_oUsersComboBox.GetCurSel() == CB_ERR)
	{
		strErrorMessage += _T("\nPlease select a project manager.");
		bValid = false;
	}

	if (m_oStateComboBox.GetCurSel() == CB_ERR)
	{
		strErrorMessage += _T("\nPlease select a state of project.");
		bValid = false;
	}

	if (!bValid) {
		AfxMessageBox(strErrorMessage);
	}

	return bValid;
}
void CProjectsDialog::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}
