#pragma once
#include "afxdialogex.h"
#include "DLLExport.h"
#include "STRUCTURES.h"

// CProjectsDialog dialog

class UIDialogDLL_EXP CProjectsDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CProjectsDialog)

public:
	CProjectsDialog(CUsersTypedPtrArray& oUsersArray, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CProjectsDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDB_PROJECTS_DIALOG };
#endif


protected:
    afx_msg void OnListTasksRightClick(NMHDR* pNMHDR, LRESULT* pResult);
	void OnAddTask();
	DECLARE_MESSAGE_MAP()
public:
	//Overrides
	// ----------------
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	//Members
	//----------------
	CComboBox m_oProjectManagerComboBox;
	CListCtrl m_oListTasks;
	CTasksTypedPtrArray m_oTasksList;
	CUsersTypedPtrArray& m_oUsersArray;
};
