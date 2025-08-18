#include "pch.h"
#include "framework.h"
#include "CUsersView.h"
#include "STRUCTURES.h"
#include <CUsersDialog.h>
#include <Resource.h>
#include "ViewActions.h"

#define ID_USER_ADD 1001
#define ID_USER_UPDATE 1002
#define ID_USER_DELETE 1003
#define ID_USER_VIEW 1004
#define USER_TABLE_COLUMN_NAME 1

#define USER_TABLE_COLUMN_ID 0
#define USER_TABLE_COLUMN_NAME 1
#define USER_TABLE_COLUMN_EMAIL 2
#define USER_TABLE_COLUMN_JOB_TITLE 3

#define USER_TABLE_COLUMN_ID_WIDTH 50
#define USER_TABLE_COLUMN_NAME_WIDTH 150
#define USER_TABLE_COLUMN_EMAIL_WIDTH 150
#define USER_TABLE_COLUMN_JOB_TITLE_WIDTH 150


/////////////////////////////////////////////////////////////////////////////
//CCSoftInternshipProjectView

// Macros
// ----------------
IMPLEMENT_DYNCREATE(CUsersView, CListView)

BEGIN_MESSAGE_MAP(CUsersView, CListView)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CUsersView::OnNMRClick)
	ON_COMMAND(ID_USER_ADD, &CUsersView::OnUserAdd)
	ON_COMMAND(ID_USER_UPDATE, &CUsersView::OnUserUpdate)
	ON_COMMAND(ID_USER_DELETE, &CUsersView::OnUserDelete)
	ON_COMMAND(ID_USER_VIEW, &CUsersView::OnUserDetails)
END_MESSAGE_MAP()

// Constructor / Destructor
// ----------------
CUsersView::CUsersView() noexcept
{
}

CUsersView::~CUsersView()
{
}

// Methods
// ----------------
CUsersDocument* CUsersView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUsersDocument)));
	return (CUsersDocument*)m_pDocument;
}


// MFC Message Handlers
// ----------------


//Methods
// ----------------
void CUsersView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* pHint)
{
	switch ((ViewActions)lHint)
	{
	case ViewAdd:
	{
		USERS* pUser = (USERS*)pHint;
		CListCtrl& oListCtrl = GetListCtrl();
		int nItemIndex = oListCtrl.GetItemCount();
		InsertDataInCtrl(pUser, nItemIndex, ViewAdd);
		break;
	}

	case ViewUpdate:
	{
		USERS* pUser = (USERS*)pHint;
		CListCtrl& oListCtrl = GetListCtrl();

		int nSelectedItem = oListCtrl.GetNextItem(-1, LVNI_SELECTED);
		if (nSelectedItem != -1)
		{
			InsertDataInCtrl(pUser, nSelectedItem, ViewUpdate);
		}
		break;
	}

	case ViewDelete:
	{
		USERS* pUser = (USERS*)pHint;
		CListCtrl& oListCtrl = GetListCtrl();

		int nSelectedItem = oListCtrl.GetNextItem(-1, LVNI_SELECTED);
		if (nSelectedItem != -1)
		{
			oListCtrl.DeleteItem(nSelectedItem);
		}
		break;
	}
	default:
		break;
	}
}

#ifdef _DEBUG
void CUsersView::AssertValid() const
{
	CListView::AssertValid();
}

void CUsersView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif 
BOOL CUsersView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CListView::PreCreateWindow(cs);
}

void CUsersView::OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	CListCtrl& oListCtrl = GetListCtrl();
	CPoint oPoint;
	GetCursorPos(&oPoint);

	CMenu oContextMenu;
	oContextMenu.CreatePopupMenu();

	int nSelectedItem = oListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if (nSelectedItem == -1)
	{
		oContextMenu.AppendMenu(MF_STRING, ID_USER_ADD, _T("Add User"));
	}
	else 
	{
		oContextMenu.AppendMenu(MF_STRING, ID_USER_UPDATE, _T("Update User"));
		oContextMenu.AppendMenu(MF_STRING, ID_USER_DELETE, _T("Delete User"));
		oContextMenu.AppendMenu(MF_STRING, ID_USER_VIEW, _T("View User"));
	}


	oContextMenu.TrackPopupMenu(
		TPM_LEFTALIGN | TPM_RIGHTBUTTON,
		oPoint.x, oPoint.y, this);

	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);


	*pResult = 0;
}

void CUsersView::OnUserAdd()
{
	USERS oNewUser;
	CUsersDialog oUsersDialog(oNewUser, ViewAdd, false, true);

	if (oUsersDialog.DoModal() == IDOK)
	{
		if (GetDocument()->AddUser(oNewUser))
		{
			GetDocument()->GetAllUsers();
			return;
		}
		AfxMessageBox(_T("Failed to add user to the database."));
		OnUserAdd();
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
			OnUserAdd();
		}
	};
}

void CUsersView::OnUserUpdate()
{
	CListCtrl& oListCtrl = GetListCtrl();
	int nSelectedItem = oListCtrl.GetNextItem(-1, LVNI_SELECTED);

	if (nSelectedItem == -1)
	{
		return;
	}

	USERS* pUser = (USERS*)oListCtrl.GetItemData(nSelectedItem);
	CString szName;
	szName.Format(_T("Update User: %s"), pUser->szName);

	CUsersDialog oUserDialog(*pUser, ViewUpdate, true, false);
	if (oUserDialog.DoModal() == IDOK)
	{
		if (GetDocument()->UpdateUser(*pUser))
		{
			GetDocument()->GetAllUsers();
		}
		else
		{
			AfxMessageBox(_T("Failed to update user to the database."));
			OnUserUpdate();
		}
	};
};

void CUsersView::OnUserDelete()
{
	int result = AfxMessageBox(_T("Are you sure you want to delete user ?"), MB_YESNO | MB_ICONQUESTION);

	if (result != IDYES) {
		return;
	}

	CListCtrl& oListCtrl = GetListCtrl();
	int nSelectedItem = oListCtrl.GetNextItem(-1, LVNI_SELECTED);
	USERS* pUser = (USERS*)oListCtrl.GetItemData(nSelectedItem);

	if (GetDocument()->DeleteUser(pUser->lId)) {
		GetDocument()->GetAllUsers();
	}
	else {
		AfxMessageBox(_T("Failed to delete user ?"));
	}
	return;
};

void CUsersView::OnUserDetails()
{
	CListCtrl& oListCtrl = GetListCtrl();
	int nSelectedItem = oListCtrl.GetNextItem(-1, LVNI_SELECTED);

	if (nSelectedItem == -1)
	{
		return;
	}

	USERS* pUser = (USERS*)oListCtrl.GetItemData(nSelectedItem);
	CString name;
	name.Format(_T("User Details: %s"), pUser->szName);
	CUsersDialog oUserDialog(*pUser, ViewDetails, false, true);
	oUserDialog.DoModal();
};

void CUsersView::InsertDataInCtrl(const USERS* pUser, int nItemIndex, ViewActions eAction)
{
	CListCtrl& oListCtrl = GetListCtrl();

	if (eAction == ViewAdd) {
		CString strId;
		strId.Format(_T("%d"), pUser->lId);
		nItemIndex = oListCtrl.InsertItem(nItemIndex, strId);
	}

	oListCtrl.SetItemText(nItemIndex, USER_TABLE_COLUMN_NAME, pUser->szName);
	oListCtrl.SetItemText(nItemIndex, USER_TABLE_COLUMN_EMAIL, pUser->szEmail);
	oListCtrl.SetItemText(nItemIndex, USER_TABLE_COLUMN_JOB_TITLE, pUser->szJobTitle);
	oListCtrl.SetItemData(nItemIndex, (DWORD_PTR)pUser);
}

// Overrides
// ----------------

void CUsersView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
	
	CListCtrl& oListCtrl = GetListCtrl();
	oListCtrl.ModifyStyle(0, LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS);
	oListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	oListCtrl.InsertColumn(USER_TABLE_COLUMN_ID, _T("ID"), LVCFMT_LEFT, USER_TABLE_COLUMN_ID_WIDTH);
	oListCtrl.InsertColumn(USER_TABLE_COLUMN_NAME, _T("Name"), LVCFMT_LEFT, USER_TABLE_COLUMN_NAME_WIDTH);
	oListCtrl.InsertColumn(USER_TABLE_COLUMN_EMAIL, _T("Email"), LVCFMT_LEFT, USER_TABLE_COLUMN_EMAIL_WIDTH);
	oListCtrl.InsertColumn(USER_TABLE_COLUMN_JOB_TITLE, _T("Jobs Title"), LVCFMT_LEFT, USER_TABLE_COLUMN_JOB_TITLE_WIDTH);
	 
	CUsersMap& oUsersMap = GetDocument()->GetAllUsers();

	POSITION pos = oUsersMap.GetStartPosition();
	int i = 0;

	while (pos != nullptr)
	{
		USERS* pUser = nullptr;
		int key = 0; // or whatever key type you're using

		oUsersMap.GetNextAssoc(pos, key, pUser); // Advance position and get key-value

		CString strId;
		strId.Format(_T("%d"), pUser->lId);

		int nItemIndex = oListCtrl.InsertItem(i, strId);

		InsertDataInCtrl(pUser, nItemIndex);

		++i;
	}

	GetDocument()->SetTitle(_T("Users"));
}

