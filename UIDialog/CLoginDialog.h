#pragma once
#include "afxdialogex.h"
#include "DLLExport.h"


class UIDialogDLL_EXP CLoginDialog: public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDialog)

public:
	//CUsersDialog(CWnd* pParent = nullptr);
	CLoginDialog(CString& strEmail, CString& strPassword, CWnd* pParent = nullptr);

	virtual ~CLoginDialog();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDB_LOGIN_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	// Members
	//----------------------
	CString& m_strEmail;
	CString& m_strPassword;

private:
	//Methods
	//----------------------
	bool ValidateInput();


public:
	afx_msg void OnBnClicked();
	afx_msg void OnBnClickedCancel();
};
