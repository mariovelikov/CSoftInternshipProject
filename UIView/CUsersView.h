#pragma once
#include <afxcview.h>
#include "DLLExport.h"
#include <afx.h>
#include "CUsersDocument.h"
#include "ViewActions.h"

/////////////////////////////////////////////////////////////////////////////
//CCSoftInternshipProjectView
/// <summary>
/// CUsersView class for displaying and managing user data.
///	<summary>
class UIViewDLL_EXP CUsersView : public CListView
{
	// Macros
	// ----------------
	DECLARE_DYNCREATE(CUsersView)
	DECLARE_MESSAGE_MAP()

	// Constructor / Destructor
	// ----------------
public:
	CUsersView() noexcept;
	virtual ~CUsersView();

	// Methods
	// ----------------
public:
	void CUsersView::OnUserAdd();
	void CUsersView::OnUserUpdate();
	void CUsersView::OnUserDelete();
	void CUsersView::OnUserDetails();

	void InsertDataInCtrl(const USERS* pUser, int nItemIndex, ViewActions eAction = ViewUpdate);

public:
	CUsersDocument* GetDocument() const;

	// Overrides
	// ----------------
protected:
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* /* pSender */, LPARAM /* lHint */, CObject* /* pHint */);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	// Members
	// ----------------
public:
	afx_msg void OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult);
};