#pragma once
#include "afxdialogex.h"
#include "STRUCTURES.h"
#include "ViewActions.h"


// CTasksDialog dialog

class CTasksDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CTasksDialog)

public:
	//CTasksDialog(CWnd* pParent = nullptr);   // standard constructor
	CTasksDialog(CUsersMap& oUsersMap, TASKS& oTask, ViewActions eCurAction = ViewAdd, CWnd* pParent = nullptr);
	virtual ~CTasksDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDB_TASKS_DIALOG };
#endif

	//Methods
	//----------------

private:
	/// <summary>
	/// Validates the data entered in the dialog.
	/// </summary>
	/// <returns> Return true if everything is valid, otherwise return false</returns>
	bool ValidateData();

	/// <summary>
	/// Fills or initializes task-related data.
	/// </summary>
	void FillTaskData();

	/// <summary>
	/// Fills the combo boxes with users, states, and total effort options.
	/// </summary>
	void FillComboBoxes();

	/// <summary> Disable all controls </summary>
	void DisableConrols();

public:

	//Overrides
	//----------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()


	//Members 
	//----------------
private:
	ViewActions m_eCurentAction;
	//CUsersTypedPtrArray& m_oUsersArray;
	CUsersMap& m_oUsersMap;
	CComboBox m_oUsersComboBox;
	CComboBox m_oStateComboBox;
	CString m_strName;
	CString m_strDescription;
	int m_nEffort;

	TASKS& m_oTask;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
