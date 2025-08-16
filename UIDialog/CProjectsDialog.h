#pragma once
#include "afxdialogex.h"
#include "DLLExport.h"
#include "STRUCTURES.h"
#include "ViewACtions.h"

// CProjectsDialog dialog

class UIDialogDLL_EXP CProjectsDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CProjectsDialog)

public:
	CProjectsDialog(CUsersMap& oUsersMap, PROJECT_DETAILS& oProjectDetails, ViewActions eAction, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CProjectsDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDB_PROJECTS_DIALOG };
#endif
	//Methods
	//----------------
private:
	/// <summary>
	/// Converts the SprxTasksState to a string representation.
	/// </summary>
	/// <param name="state"></param>
	/// <returns></returns>
	CString StateToString(SprxTasksState state);

	/// <summary>Fill table with tasks</summary>
	void FillTasksInTable();
protected:
    afx_msg void OnListTasksRightClick(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

private:
	//Methods
	//----------------
	
	/// <summary>Add task</summary>
	void OnAddTask();

	/// <summary>Update task</summary>
	void OnUpdateTask();

	/// <summary>Delete task</summary>
	void OnDeleteTask();

	/// <summary> Show details on task</summary>
	void OnViewDetails();

	/// <summary> Validates the data entered in the dialog. </summary>
	bool ValidateData();

	void VisualizeTask(TASKS& oTask, ViewActions eAction = ViewAdd, int nSeletedIndex = NULL);

	/// <summary> Fill all data for project</summary>
	void FillProjectData();

	/// <summary> Fill all data in combo boxes </summary>
	void FillComboBoxes();

	/// <summary> Set all project details data in boxes </summary>
	void ViewProjectDetails();
public:
	//Overrides
	// ----------------
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	//Members
	//----------------
	CListCtrl m_oListTasks;
	CUsersMap& m_oUsersMap;

	CComboBox m_oUsersComboBox;
	CComboBox m_oStateComboBox;
	CString m_strName;
	CString m_strDescription;
	
	PROJECT_DETAILS& m_oProjectDetails;
	ViewActions m_eCurrentAction;
	int m_nTotalEffort;

	
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
