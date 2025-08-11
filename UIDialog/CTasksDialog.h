#pragma once
#include "afxdialogex.h"
#include "STRUCTURES.h"


// CTasksDialog dialog

class CTasksDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CTasksDialog)

public:
	//CTasksDialog(CWnd* pParent = nullptr);   // standard constructor
	CTasksDialog(CUsersTypedPtrArray& oUsersArray, CWnd* pParent = nullptr);
	virtual ~CTasksDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDB_TASKS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()


	//Members 
	//----------------
private:
	CUsersTypedPtrArray& m_oUsersArray;
	CComboBox m_oUsersComboBox;
public:
	virtual BOOL OnInitDialog();
};
