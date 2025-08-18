#pragma once
#include "afxdialogex.h"
#include "DLLExport.h"
#include <STRUCTURES.h>
#include "ViewActions.h"

/// <summary>
/// Users dialog
/// </summary>
class UIDialogDLL_EXP CUsersDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CUsersDialog)

public:
	//CUsersDialog(CWnd* pParent = nullptr);
	CUsersDialog(USERS& oUser, ViewActions eViewActions, bool bHideOkButton, bool bHideUpdateButton = true, CWnd* pParent = nullptr);
	
	virtual ~CUsersDialog();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDB_USERS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	// Members
	//----------------------
	USERS& m_oUser;
	CString m_strName;
	CString m_strEmail;
	CString m_strJobsTitle;
	CString m_strPassword;
	CString m_strPasswordRepeat;
	ViewActions m_eCurrentAction = ViewAdd;

private:
	// Buttons and controls
	//----------------------

	bool m_bHideOkButton = false;
	bool m_bHideUpdateButton = false;

	//Methods
	//----------------------

	bool ValidateInput();
	void AddUser();
	void UpdateUser();
	void SetReadOnlyControls();

	// Set values to member user from Dialog textboxes
	void SetValuesToMemberUser();

public:
	afx_msg void OnBnClicked();
};
