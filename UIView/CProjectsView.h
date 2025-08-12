#pragma once
#include <afxcview.h>
#include "DLLExport.h"
#include <afx.h>
#include "CProjectsDocument.h"
#include "ViewActions.h"

/////////////////////////////////////////////////////////////////////////////
//CCSoftInternshipProjectView
/// <summary>
/// CProjectsView class for displaying and managing user data.
///	<summary>
class UIViewDLL_EXP CProjectsView : public CListView
{
	// Macros
	// ----------------
	DECLARE_DYNCREATE(CProjectsView)
	DECLARE_MESSAGE_MAP()

	// Constructor / Destructor
	// ----------------
public:
	CProjectsView() noexcept;
	virtual ~CProjectsView();


	// Methods
	// ----------------
public:
	void CProjectsView::OnProjectAdd();
	void CProjectsView::OnProjectDetails();
	void CProjectsView::OnProjectDelete();
	void CProjectsView::OnProjectUpdate();

	void InsertDataInCtrl(const PROJECTS_VIEW_ITEM* pProject, int nItemIndex, ViewActions eAction = ViewUpdate);
public:
	CProjectsDocument* GetDocument() const;

	// Overrides
	// ----------------
protected:
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult);

private:
	// Members
	// ----------------
};