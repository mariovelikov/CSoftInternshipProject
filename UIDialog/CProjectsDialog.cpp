// CProjectsDialog.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "CProjectsDialog.h"
#include "CTasksDialog.h"
#include "Resource.h"
#include "STRUCTURES.h"

#define ID_ADD_TASK 1001
#define ID_UPDATE_TASK 1002
#define ID_DELETE_TASK 1003
#define ID_DETAILS_TASK 1004

#define TASKS_ID_COLUMN 0
#define TASKS_NAME_COLUMN 1
#define TASKS_DESCRIPTION_COLUMN 2
#define TASKS_EFFORT_COLUMN 3
#define TASKS_STATE_COLUMN 4

BEGIN_MESSAGE_MAP(CProjectsDialog, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LSC_TASKS, &CProjectsDialog::OnListTasksRightClick)
	ON_COMMAND(ID_ADD_TASK, &CProjectsDialog::OnAddTask)
	ON_COMMAND(ID_UPDATE_TASK, &CProjectsDialog::OnUpdateTask)
	ON_COMMAND(ID_DELETE_TASK, &CProjectsDialog::OnDeleteTask)
	ON_COMMAND(ID_DETAILS_TASK, &CProjectsDialog::OnViewDetails)
	ON_BN_CLICKED(IDOK, &CProjectsDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CProjectsDialog::OnBnClickedCancel)
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CProjectsDialog, CDialogEx)

//Constructor / Destructor
//----------------
CProjectsDialog::CProjectsDialog(CUsersMap& oUsersMap, PROJECT_DETAILS& oProjectDetails, ViewActions eAction, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EDB_PROJECTS_DIALOG, pParent), m_oUsersMap(oUsersMap), m_oProjectDetails(oProjectDetails), m_eCurrentAction(eAction)
{
	m_strName = m_oProjectDetails.recProject.szName;
	m_strDescription = m_oProjectDetails.recProject.szDescription;
	m_nTotalEffort = m_oProjectDetails.recProject.lTotalEffort;
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
	DDX_Text(pDX, IDC_CMB_TOTAL_EFFORT, m_nTotalEffort);

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
		oContextMenu.AppendMenuW(MF_STRING, ID_UPDATE_TASK, _T("Update Task"));
		oContextMenu.AppendMenuW(MF_STRING, ID_DELETE_TASK, _T("Delete Task"));
		oContextMenu.AppendMenuW(MF_STRING, ID_DETAILS_TASK, _T("View Details"));
	}
	else 
	{
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
	m_oListTasks.InsertColumn(TASKS_EFFORT_COLUMN, _T("Effort"), LVCFMT_LEFT, 100);
	m_oListTasks.InsertColumn(TASKS_STATE_COLUMN, _T("State"), LVCFMT_LEFT, 100);

	m_oListTasks.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_oUsersComboBox.ModifyStyle(CBS_SORT, 0);

	FillComboBoxes();

	switch (m_eCurrentAction)
	{
	case ViewDetails:
		ViewProjectDetails();
		break;

	case ViewUpdate:
		FillTasksInTable();

	default:
		break;
	}

	return TRUE;
}

void CProjectsDialog::FillTasksInTable()
{
	for (int i = 0; i < m_oProjectDetails.oTasksTypedPtrArray.GetCount(); i++)
	{
		VisualizeTask(*m_oProjectDetails.oTasksTypedPtrArray[i]);
	}
};

void CProjectsDialog::ViewProjectDetails()
{
	FillTasksInTable();

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

	// Disable controls if viewing details
	m_oStateComboBox.EnableWindow(FALSE);
	GetDlgItem(IDC_CMB_USERS)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDB_PROJECTS_NAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDB_PROJECTS_DESCRIPTION)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMB_STATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_STT_TOTAL_EFFORT_TITLE)->ShowWindow(TRUE);
	GetDlgItem(IDC_CMB_TOTAL_EFFORT)->ShowWindow(TRUE);
	GetDlgItem(IDC_CMB_TOTAL_EFFORT)->EnableWindow(FALSE);
}

void CProjectsDialog::FillComboBoxes()
{
	// fil the combo box with users
	POSITION pos = m_oUsersMap.GetStartPosition();
	while (pos != nullptr)
	{
		int userId;
		USERS* pUser;
		m_oUsersMap.GetNextAssoc(pos, userId, pUser);

		int index = m_oUsersComboBox.AddString(pUser->szName);
		m_oUsersComboBox.SetItemData(index, userId);

		if (pUser->lId != m_oProjectDetails.recProject.lProjectManagerId)
		{
			continue;
		}
		m_oUsersComboBox.SetCurSel(index);
	}

	int nIndex = m_oStateComboBox.AddString(_T("Active"));
	m_oStateComboBox.SetItemData(nIndex, Active);

	nIndex = m_oStateComboBox.AddString(_T("Finished"));
	m_oStateComboBox.SetItemData(nIndex, Finished);

	if (m_oStateComboBox.GetCount() > 0)
		m_oStateComboBox.SetCurSel(0);
}

void CProjectsDialog::OnAddTask()
{
	TASKS* oTask = new TASKS();

	CTasksDialog oTasksDialog(m_oUsersMap, *oTask);
	if (oTasksDialog.DoModal() == IDOK)
	{
		oTask->lProjectId = m_oProjectDetails.recProject.lId;
		m_oProjectDetails.oTasksTypedPtrArray.Add(oTask);
		m_oProjectDetails.recProject.lTotalEffort += oTask->nEffort;
		VisualizeTask(*oTask);
	}
	else
	{
		AfxMessageBox(_T("No task was added."));
	}
}

void CProjectsDialog::OnUpdateTask() 
{
	int nSelectedItem = m_oListTasks.GetNextItem(-1, LVNI_SELECTED);
	if (nSelectedItem == -1)
	{
		return;
	}

	TASKS* pTask = (TASKS*)m_oListTasks.GetItemData(nSelectedItem);
	CTasksDialog oTasksDialog(m_oUsersMap, *pTask);

	if (oTasksDialog.DoModal() != IDOK)
	{
		return;
	}

	// Sum and set new TotalEffort
	m_oProjectDetails.recProject.lTotalEffort = 0;
	for (int i = 0; i < m_oProjectDetails.oTasksTypedPtrArray.GetCount(); i++)
	{
		m_oProjectDetails.recProject.lTotalEffort += m_oProjectDetails.oTasksTypedPtrArray.GetAt(i)->nEffort;
	}

	VisualizeTask(*pTask, ViewUpdate, nSelectedItem);
}

void CProjectsDialog::OnDeleteTask()
{
	int nSelectedItem = m_oListTasks.GetNextItem(-1, LVNI_SELECTED);
	if (nSelectedItem == -1)
	{
		return;
	}

	TASKS* pTask = reinterpret_cast<TASKS*>(m_oListTasks.GetItemData(nSelectedItem));
	if (pTask->lId != 0)
	{
		m_oProjectDetails.recProject.lTotalEffort -= pTask->nEffort;
		m_oProjectDetails.m_oTaskIdsToDelete.Add(pTask->lId);

		// delete 
		for (int i = 0; m_oProjectDetails.oTasksTypedPtrArray.GetCount(); i++)
		{
			if (pTask->lId != m_oProjectDetails.oTasksTypedPtrArray.GetAt(i)->lId)
			{
				continue;
			}

			delete m_oProjectDetails.oTasksTypedPtrArray[i];
			m_oProjectDetails.oTasksTypedPtrArray[i] = nullptr;
			m_oProjectDetails.oTasksTypedPtrArray.RemoveAt(i); 
			break;
		}
	}
	else
	{
		m_oProjectDetails.oTasksTypedPtrArray.RemoveAt(nSelectedItem);
	}

	m_oListTasks.DeleteItem(nSelectedItem);
}

void CProjectsDialog::OnViewDetails()
{
	int nSelectedItem = m_oListTasks.GetNextItem(-1, LVNI_SELECTED);
	if (nSelectedItem == -1)
	{
		return;
	}

	TASKS* pTask = reinterpret_cast<TASKS*>(m_oListTasks.GetItemData(nSelectedItem));

	CTasksDialog oTasksDialog(m_oUsersMap, *pTask, ViewDetails);
	oTasksDialog.DoModal();
}

void CProjectsDialog::VisualizeTask(TASKS& oTask, ViewActions eAction, int nSelectedItem)
{
	int nIndex;

	if (eAction == ViewUpdate)
	{
		nIndex = nSelectedItem;
	}
	else
	{
		nIndex = m_oListTasks.InsertItem(1, _T(""));
	}

	CString strEffort;
	strEffort.Format(_T("%d"), oTask.nEffort);

	CString strTaskId;
	strTaskId.Format(_T("%d"), oTask.lId);
	
	m_oListTasks.SetItemText(nIndex, TASKS_ID_COLUMN, strTaskId);
	m_oListTasks.SetItemText(nIndex, TASKS_NAME_COLUMN, oTask.szName);
	m_oListTasks.SetItemText(nIndex, TASKS_DESCRIPTION_COLUMN, oTask.szDescription);
	m_oListTasks.SetItemText(nIndex, TASKS_EFFORT_COLUMN, strEffort);
	CString strState = StateToString((StateEnum)oTask.nState);
	m_oListTasks.SetItemText(nIndex, TASKS_STATE_COLUMN, strState);

	m_oListTasks.SetItemData(nIndex, (DWORD_PTR)&oTask);
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
