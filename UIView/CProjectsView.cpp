#include "pch.h"
#include "CProjectsView.h"
#include "framework.h"
#include "STRUCTURES.h"
#include <Resource.h>
#include "ViewActions.h"

// Macros
// ----------------
IMPLEMENT_DYNCREATE(CProjectsView, CListView)

BEGIN_MESSAGE_MAP(CProjectsView, CListView)
	//ON_NOTIFY_REFLECT(NM_RCLICK, &CProjectsView::OnNMRClick)
END_MESSAGE_MAP()

//Constructor / Destructor
//----------------
CProjectsView::CProjectsView() noexcept 
{
};
CProjectsView::~CProjectsView()
{
};

#ifdef _DEBUG
void CProjectsView::AssertValid() const
{
	CListView::AssertValid();
}

void CProjectsView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif 

BOOL CProjectsView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CListView::PreCreateWindow(cs);
}

//Methods
//----------------
void CProjectsView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
	GetParentFrame()->SetWindowText(_T("Projects View"));
	
	// Set the column headers
	CListCtrl& oListCtrl = GetListCtrl();
	oListCtrl.ModifyStyle(0, LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS);
	oListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	oListCtrl.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 50);
	oListCtrl.InsertColumn(1, _T("Project Name"), LVCFMT_LEFT, 150);
	oListCtrl.InsertColumn(2, _T("Description"), LVCFMT_LEFT, 200);

	// Load initial data
	CProjectsTypedPtrArray& oProjectsArray = GetDocument()->GetAllProjects();
	//USERS* pCurrentUser = GetDocument()->();

	for (int i = 0; i < oProjectsArray.GetCount(); i++) {
		PROJECTS* pProject = oProjectsArray.GetAt(i);

		InsertDataInCtrl(pProject, i, ViewAdd);
	}
}

CProjectsDocument* CProjectsView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CProjectsDocument)));
	return (CProjectsDocument*)m_pDocument;
}


void CProjectsView::InsertDataInCtrl(const PROJECTS* pProject, int nItemIndex, ViewActions eAction)
{
	CListCtrl& oListCtrl = GetListCtrl();

	if (eAction == ViewAdd) {
		CString strId;
		strId.Format(_T("%d"), pProject->lId);
		nItemIndex = oListCtrl.InsertItem(nItemIndex, strId);
	}

	oListCtrl.SetItemText(nItemIndex, 1, pProject->szName);
	oListCtrl.SetItemText(nItemIndex, 2, pProject->szDescription);
	oListCtrl.SetItemData(nItemIndex, (DWORD_PTR)pProject);
};