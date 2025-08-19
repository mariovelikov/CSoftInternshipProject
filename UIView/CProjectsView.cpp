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
#define ID_PROJECT_DETAILS 1002
#define ID_PROJECT_DELETE 1003
#define ID_PROJECT_UPDATE 1004

#define PROJECTS_COLUMN_ID 0
#define PROJECTS_COLUMN_PROJECT_NAME 1
#define PROJECTS_COLUMN_DESCRIPTION 2
#define PROJECTS_COLUMN_PROJECT_MANAGER 3

IMPLEMENT_DYNCREATE(CProjectsView, CListView)

BEGIN_MESSAGE_MAP(CProjectsView, CListView)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CProjectsView::OnNMRClick)
	ON_COMMAND(ID_PROJECT_ADD, &CProjectsView::OnProjectAdd)
	ON_COMMAND(ID_PROJECT_UPDATE, &CProjectsView::OnProjectUpdate)
	ON_COMMAND(ID_PROJECT_DETAILS, &CProjectsView::OnProjectDetails)
	ON_COMMAND(ID_PROJECT_DELETE, &CProjectsView::OnProjectDelete)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, &CProjectsView::OnLvnColumnclick)
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
	oListCtrl.InsertColumn(PROJECTS_COLUMN_ID, _T("ID"), LVCFMT_LEFT, 50);
	oListCtrl.InsertColumn(PROJECTS_COLUMN_PROJECT_NAME, _T("Project Name"), LVCFMT_LEFT, 150);
	oListCtrl.InsertColumn(PROJECTS_COLUMN_DESCRIPTION, _T("Description"), LVCFMT_LEFT, 200);
	oListCtrl.InsertColumn(PROJECTS_COLUMN_PROJECT_MANAGER, _T("Project Manager"), LVCFMT_LEFT, 200);

	// Load initial data
	CProjectsViewItemTypedPtrArray& oProjectsArray = GetDocument()->GetAllProjects();

	for (int i = 0; i < oProjectsArray.GetCount(); i++) {
		PROJECTS_VIEW_ITEM* pProjectItem = oProjectsArray.GetAt(i);

		InsertDataInCtrl(pProjectItem, i, ViewAdd);
	}

	GetDocument()->SetTitle(_T("Projects"));
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

	oListCtrl.SetItemText(nItemIndex, PROJECTS_COLUMN_PROJECT_NAME, pProject->recProject.szName);
	oListCtrl.SetItemText(nItemIndex, PROJECTS_COLUMN_DESCRIPTION, pProject->recProject.szDescription);
	oListCtrl.SetItemText(nItemIndex, PROJECTS_COLUMN_PROJECT_MANAGER, pProject->szProjectManagerName);
	oListCtrl.SetItemData(nItemIndex, (DWORD_PTR)pProject);
};


void CProjectsView::OnProjectAdd()
{
	CUsersMap& oUsersMap= GetDocument()->GetAllUsers();
	PROJECT_DETAILS oProjectDetails;

	CProjectsDialog oProjectsDialog(oUsersMap, oProjectDetails, ViewAdd);
	if (oProjectsDialog.DoModal() == IDOK)
	{
		if (!GetDocument()->AddProject(oProjectDetails.recProject, oProjectDetails.oTasksTypedPtrArray))
		{
			AfxMessageBox(_T("Failed to add the project. Please check the input data or try again."));
			
		}
	}
	else
	{
		int result = AfxMessageBox(
			_T("Are you sure you want to cancel? Unsaved changes will be lost."),
			MB_YESNO | MB_ICONQUESTION
		);

		if (result == IDYES)
		{
			return;
		}
		else {
			OnProjectAdd();
		}
	}
}

void CProjectsView::OnProjectDetails()
{
	CUsersMap& oUsersArray = GetDocument()->GetAllUsers();
	
	CListCtrl& oListCtrl = GetListCtrl();
	int nSelectedItem = oListCtrl.GetNextItem(-1, LVNI_SELECTED);

	if (nSelectedItem == -1)
	{
		return;
	}
	PROJECTS_VIEW_ITEM* pProject = (PROJECTS_VIEW_ITEM*)oListCtrl.GetItemData(nSelectedItem);
	
	PROJECT_DETAILS oProject;
	oProject.recProject = pProject->recProject;

	GetDocument()->GetProjectDetails(oProject);
	CProjectsDialog oProjectDialog(oUsersArray, oProject, ViewDetails);
	oProjectDialog.DoModal();
}

void CProjectsView::OnProjectDelete()
{
	CListCtrl& oListCtrl = GetListCtrl();
	int nSelectedItem = oListCtrl.GetNextItem(-1, LVNI_SELECTED);

	if (nSelectedItem == -1)
	{
		return;
	}

	int result = AfxMessageBox(_T("Are you sure you want to delete this item?"), MB_YESNO | MB_ICONQUESTION);
	if (result == IDYES)
	{
		PROJECTS_VIEW_ITEM* pProject = (PROJECTS_VIEW_ITEM*)oListCtrl.GetItemData(nSelectedItem);
		PROJECT_DETAILS oProject;
		oProject.recProject = pProject->recProject;

		GetDocument()->GetProjectDetails(oProject);

		bool bDeleted = GetDocument()->DeleteProject(oProject);
		if (!bDeleted)
		{
			AfxMessageBox(_T("Failed to delete project with tasks."));
		}
	}
}

void CProjectsView::OnProjectUpdate()
{
	CUsersMap& oUsersMap = GetDocument()->GetAllUsers();

	CListCtrl& oListCtrl = GetListCtrl();
	int nSelectedItem = oListCtrl.GetNextItem(-1, LVNI_SELECTED);

	if (nSelectedItem == -1)
	{
		return;
	}
	PROJECTS_VIEW_ITEM* pProject = (PROJECTS_VIEW_ITEM*)oListCtrl.GetItemData(nSelectedItem);

	PROJECT_DETAILS oProject;
	oProject.recProject = pProject->recProject;

	GetDocument()->GetProjectDetails(oProject);
	CProjectsDialog oProjectDialog(oUsersMap, oProject, ViewUpdate);

	if (oProjectDialog.DoModal() != IDOK)
	{
		return;
	}

	CString strName = oUsersMap[oProject.recProject.lProjectManagerId]->szName;
	if (!GetDocument()->UpdateProject(oProject, pProject, strName))
	{
		AfxMessageBox(_T("Failed to update the project. Please check the input and try again."));
	};
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
	{	oContextMenu.AppendMenu(MF_STRING, ID_PROJECT_UPDATE, _T("Update Project"));
		oContextMenu.AppendMenu(MF_STRING, ID_PROJECT_DELETE, _T("Delete Project"));
		oContextMenu.AppendMenu(MF_STRING, ID_PROJECT_DETAILS, _T("View Project Details"));
	}

	oContextMenu.TrackPopupMenu(
		TPM_LEFTALIGN | TPM_RIGHTBUTTON,
		oPoint.x, oPoint.y, this);

	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	*pResult = 0;
}

void CProjectsView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* pHint)
{
	switch ((ViewActions)lHint)
	{
	case ViewAdd:
	{
		PROJECTS_VIEW_ITEM* pViewItem = (PROJECTS_VIEW_ITEM*)pHint;
		CListCtrl& oListCtrl = GetListCtrl();
		int nItemIndex = oListCtrl.GetItemCount();

		InsertDataInCtrl(pViewItem, nItemIndex, ViewAdd);
		break;
	}

	case ViewDelete:
	{
		PROJECTS_VIEW_ITEM* pViewItem = (PROJECTS_VIEW_ITEM*)pHint;
		CListCtrl& oListCtrl = GetListCtrl();

		int nSelectedItem = oListCtrl.GetNextItem(-1, LVNI_SELECTED);
		if (nSelectedItem != -1)
		{
			oListCtrl.DeleteItem(nSelectedItem);
		}
		break;
	}
	case ViewUpdate:
	{
		PROJECTS_VIEW_ITEM* pProjectDetails= (PROJECTS_VIEW_ITEM*)pHint;
		CListCtrl& oListCtrl = GetListCtrl();

		int nSelectedItem = oListCtrl.GetNextItem(-1, LVNI_SELECTED);
		if (nSelectedItem != -1)
		{
			InsertDataInCtrl(pProjectDetails, nSelectedItem, ViewUpdate);
		}
		break;
	}

	default:
		break;
	}
}

bool CProjectsView::bAscending = true;
void CProjectsView::OnLvnColumnclick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int nColumn = pNMLV->iSubItem;

	SortListCtrl(nColumn, bAscending);
	bAscending = !bAscending;

	*pResult = 0;
}

struct SortParams
{
	int nColumnIndex;
	bool bAscending;
};

static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	PROJECTS_VIEW_ITEM* pItem1 = (PROJECTS_VIEW_ITEM*)lParam1;
	PROJECTS_VIEW_ITEM* pItem2 = (PROJECTS_VIEW_ITEM*)lParam2;

	int nResult = 0;
	SortParams* pSortParams = (SortParams*)lParamSort;

	switch (pSortParams->nColumnIndex)
	{
	case PROJECTS_COLUMN_ID:
		nResult = pItem1->recProject.lId - pItem2->recProject.lId;
		break;
	case PROJECTS_COLUMN_PROJECT_NAME:
		nResult = _tcscmp(pItem1->recProject.szName, pItem2->recProject.szName);
		break;
	case PROJECTS_COLUMN_DESCRIPTION:
		nResult = _tcscmp(pItem1->recProject.szDescription, pItem2->recProject.szDescription);
		break;
	case PROJECTS_COLUMN_PROJECT_MANAGER:
		nResult = _tcscmp(pItem1->szProjectManagerName, pItem2->szProjectManagerName);
		break; 
	default:
		return 0;
	}
	
	return (nResult) * (pSortParams->bAscending ? 1 : -1);
};

static SortParams oSortParams;

void CProjectsView::SortListCtrl(int nColumnIndex, bool bAscending)
{
		oSortParams.nColumnIndex = nColumnIndex;
		oSortParams.bAscending = bAscending;

		CListCtrl& oListCtrl = GetListCtrl();
		oListCtrl.SortItems(CompareFunc, (LPARAM)&oSortParams);
}	