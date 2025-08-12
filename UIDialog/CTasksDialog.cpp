// CTasksDialog.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "CTasksDialog.h"
#include "Resource.h"
#include "STRUCTURES.h"


// CTasksDialog dialog

IMPLEMENT_DYNAMIC(CTasksDialog, CDialogEx)

CTasksDialog::CTasksDialog(CUsersTypedPtrArray& oUsersArray, TASKS& oTask, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EDB_TASKS_DIALOG, pParent), m_oTask(oTask), m_oUsersArray(oUsersArray)
{
	m_strName = m_oTask.szName;
	m_strDescription = m_oTask.szDescription;
	m_nEffort = m_oTask.nEffort;
}

CTasksDialog::~CTasksDialog()
{
}

void CTasksDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_TASKS_USERS, m_oUsersComboBox);
	DDX_Control(pDX, IDC_CMB_TASKS_STATE, m_oStateComboBox);
	DDX_Text(pDX, IDC_EDB_TASKS_NAME, m_strName);
	DDX_Text(pDX, IDC_EDB_TASKS_DESCRIPTION, m_strDescription);
	DDX_Text(pDX, IDC_EDB_TASKS_EFFORT, m_nEffort);


	DDV_MinMaxInt(pDX, m_nEffort, 0, 100); // Optional: limit range
	DDV_MaxChars(pDX, m_strName, TASKS_NAME_LENGTH);
	DDV_MaxChars(pDX, m_strDescription, TASKS_DESCRIPTION_LENGTH);
}


BEGIN_MESSAGE_MAP(CTasksDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTasksDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CTasksDialog message handlers

BOOL CTasksDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// Set the edit box to accept only numeric input
	GetDlgItem(IDC_EDB_TASKS_EFFORT)->ModifyStyle(0, ES_NUMBER);

	FillComboBoxes();
	return TRUE;
}

void CTasksDialog::OnBnClickedOk()
{
	UpdateData(TRUE);
	if (!ValidateData())
	{
		return;
	};

	FillTaskData();
	CDialogEx::OnOK();
}


bool CTasksDialog::ValidateData()
{
	CString strErrorMessage;
	bool bValid = true;

	// Validate the task data before saving
	if (m_strName.IsEmpty())
	{
		strErrorMessage = _T("Task name cannot be empty.");
		bValid = false;
	}
	if (m_strDescription.IsEmpty())
	{
		strErrorMessage += _T("\nTask description cannot be empty.");
		bValid = false;
	}
	if (m_oUsersComboBox.GetCurSel() == CB_ERR)
	{
		strErrorMessage += _T("\nPlease select a user for the task.");
		bValid = false;
	}
	if (m_oStateComboBox.GetCurSel() == CB_ERR)
	{
		strErrorMessage += _T("\nPlease select a state for the task.");
		bValid = false;
	}

	if (!bValid) {
		AfxMessageBox(strErrorMessage);
	}

	return bValid;
}

void CTasksDialog::FillTaskData()
{
	// Fill the TASKS structure with data from the dialog controls
	int nSelected = m_oUsersComboBox.GetCurSel();
	if (nSelected != CB_ERR) {
		m_oTask.lUserId = static_cast<long>(m_oUsersComboBox.GetItemData(nSelected));
	}
	
	m_oTask.nState = (int)m_oStateComboBox.GetItemData(m_oStateComboBox.GetCurSel());
	m_oTask.nEffort = m_nEffort;
	_tcscpy_s(m_oTask.szName, TASKS_NAME_LENGTH, m_strName);
	_tcscpy_s(m_oTask.szDescription, TASKS_DESCRIPTION_LENGTH, m_strDescription);

	return;
}

void CTasksDialog::FillComboBoxes()
{
	// fil the combo box with users
	for (int i = 0; i < m_oUsersArray.GetCount(); i++)
	{
		if (m_oUsersArray[i] == nullptr)
			continue;

		USERS* pUser = m_oUsersArray[i];
		int index = m_oUsersComboBox.AddString(pUser->szName);
		m_oUsersComboBox.SetItemData(index, (DWORD_PTR)pUser->lId);
	}
	m_oUsersComboBox.SetCurSel(0);

	//fill the list control with State
	int index = m_oStateComboBox.AddString(_T("Pending"));
	m_oStateComboBox.SetItemData(index, Pending);

	index = m_oStateComboBox.AddString(_T("InProgress"));
	m_oStateComboBox.SetItemData(index, InProgress);

	index = m_oStateComboBox.AddString(_T("Ended"));
	m_oStateComboBox.SetItemData(index, Ended);

	m_oStateComboBox.SetCurSel(0);
}