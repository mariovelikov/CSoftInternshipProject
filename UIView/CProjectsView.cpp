#include "pch.h"
#include "CProjectsView.h"
#include "framework.h"
#include "STRUCTURES.h"
#include <Resource.h>
#include "ViewActions.h"
#include "CProjectsDialog.h"
#include "CUsersView.h"

// Macros
// ----------------
#define ID_PROJECT_ADD 1001

IMPLEMENT_DYNCREATE(CProjectsView, CListView)

BEGIN_MESSAGE_MAP(CProjectsView, CListView)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CProjectsView::OnNMRClick)
	ON_COMMAND(ID_PROJECT_ADD, &CProjectsView::OnProjectAdd)
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
	oListCtrl.InsertColumn(3, _T("Project Manager"), LVCFMT_LEFT, 200);

	// Load initial data
	CProjectsViewItemTypedPtrArray& oProjectsArray = GetDocument()->GetAllProjects();

	for (int i = 0; i < oProjectsArray.GetCount(); i++) {
		PROJECTS_VIEW_ITEM* pProjectItem = oProjectsArray.GetAt(i);

		InsertDataInCtrl(pProjectItem, i, ViewAdd);
	}
}

CProjectsDocument* CProjectsView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CProjectsDocument)));
	return (CProjectsDocument*)m_pDocument;
}


void CProjectsView::InsertDataInCtrl(const PROJECTS_VIEW_ITEM* pProject, int nItemIndex, ViewActions eAction)
{
	CListCtrl& oListCtrl = GetListCtrl();

	if (eAction == ViewAdd) {
		CString strId;
		strId.Format(_T("%d"), pProject->recProject.lId);
		nItemIndex = oListCtrl.InsertItem(nItemIndex, strId);
	}

	oListCtrl.SetItemText(nItemIndex, 1, pProject->recProject.szName);
	oListCtrl.SetItemText(nItemIndex, 2, pProject->recProject.szDescription);
	oListCtrl.SetItemText(nItemIndex, 3, pProject->szProjectManagerName);
	oListCtrl.SetItemData(nItemIndex, (DWORD_PTR)pProject);
};


// MFC Message Handlers
void CProjectsView::OnProjectAdd()
{
	CUsersTypedPtrArray& oUsersArray = GetDocument()->GetAllUsers();
	PROJECTS oProject;
	CTasksTypedPtrArray oTasksArray;

	CProjectsDialog oProjectsDialog(oUsersArray, oProject, oTasksArray);
	if (oProjectsDialog.DoModal() == IDOK)
	{
		if (GetDocument()->AddProject(oProject, oTasksArray))
		{
			GetDocument()->GetAllProjects();
		}
		else
		{
			AfxMessageBox(_T("Failed to add the project. Please check the input data or try again."));
		}
	}
	else
	{
		AfxMessageBox(_T("No project was added."));
	}

	// Clear users array
	for (int i = 0; i < oUsersArray.GetCount(); i++)
	{
		delete oUsersArray[i];
	}

	oUsersArray.RemoveAll();
}


void CProjectsView::OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	CPoint oPoint;
	GetCursorPos(&oPoint);
	CMenu oContextMenu;
	oContextMenu.CreatePopupMenu();
	CListCtrl& oListCtrl = GetListCtrl();

	int nSelectedItem = oListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if (nSelectedItem == -1)
	{
		oContextMenu.AppendMenu(MF_STRING, ID_PROJECT_ADD, _T("Add Project"));
	}
	else 
	{/*
		oContextMenu.AppendMenu(MF_STRING, ID_PROJECT_UPDATE, _T("Update Project"));
		oContextMenu.AppendMenu(MF_STRING, ID_PROJECT_DELETE, _T("Delete Project"));
		oContextMenu.AppendMenu(MF_STRING, ID_PROJECT_VIEW, _T("View Project"));*/
	}

	oContextMenu.TrackPopupMenu(
		TPM_LEFTALIGN | TPM_RIGHTBUTTON,
		oPoint.x, oPoint.y, this);

	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	*pResult = 0;
}