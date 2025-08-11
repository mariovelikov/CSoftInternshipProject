#pragma once
#include "afxdialogex.h"
#include "DLLExport.h"
#include "STRUCTURES.h"

// CProjectsDialog dialog

class UIDialogDLL_EXP CProjectsDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CProjectsDialog)

public:
	CProjectsDialog(CUsersTypedPtrArray& oUsersArray, PROJECTS& oProject, CTasksTypedPtrArray& oTasksArray, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CProjectsDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDB_PROJECTS_DIALOG };
#endif
	//Methods
	//----------------
private:
	/// <summary>
	/// Converts the TotalEffortEnum to a string representation.
	/// </summary>
	/// <param name="effort"></param>
	/// <returns></returns>
	CString TotalEffortToString(TotalEffortEnum effort);

	/// <summary>
	/// Converts the StateEnum to a string representation.
	/// </summary>
	/// <param name="state"></param>
	/// <returns></returns>
	CString StateToString(StateEnum state);


protected:
    afx_msg void OnListTasksRightClick(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

private:
	//Methods
	//----------------
	void OnAddTask();

	/// <summary>
	/// Validates the data entered in the dialog.
	/// </summary>
	bool ValidateData();

	void VisualizeTask(TASKS& oTask);

	void FillProjectData();
public:
	//Overrides
	// ----------------
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	//Members
	//----------------
	CListCtrl m_oListTasks;
	CUsersTypedPtrArray& m_oUsersArray;

	CComboBox m_oUsersComboBox;
	CString m_strName;
	CString m_strDescription;

	CTasksTypedPtrArray& m_oTasksArray;
	PROJECTS& m_oProject;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
