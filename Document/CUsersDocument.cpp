#include "pch.h"
#include "CUsersDocument.h"
#include "CUsersAppService.h"
#include "ViewActions.h"

/////////////////////////////////////////////////////////////////////////////
// CUsersDocument

// Macros
// ----------------
IMPLEMENT_DYNCREATE(CUsersDocument, CDocument)

// Constructor / Destructor
// ----------------
CUsersDocument::CUsersDocument() noexcept
{
}

CUsersDocument::~CUsersDocument()
{
}

// Methods
// ----------------

CUsersTypedPtrArray& CUsersDocument::GetAllUsers()
{
	if (m_oUsersArray.IsEmpty()) 
	{
		CUsersAppService oUsersAppService;
		
		if (!oUsersAppService.GetAllUsers(m_oUsersArray)) 
		{
			TRACE(_T("Failed to retrieve users from the database.\n"));
			return m_oUsersArray;
		}
	}

	return m_oUsersArray;
}

bool CUsersDocument::AddUser(USERS& oRecUser) 
{
	CUsersAppService oUsersAppService;

	if (!oUsersAppService.AddUser(oRecUser))
	{
		return false;
	}
	USERS* pUser = new USERS(oRecUser);
	m_oUsersArray.Add(pUser);

	UpdateAllViews(nullptr, (LPARAM)ViewAdd, (CObject*)pUser);
	return true;
}

bool CUsersDocument::UpdateUser(USERS& oRecUser) 
{
	CUsersAppService oUsersAppService;

	if (!oUsersAppService.UpdateUser(oRecUser)) 
	{
		return false;
	}

	for (int i = 0; i < m_oUsersArray.GetSize(); ++i) 
	{
		if (m_oUsersArray[i]->lId == oRecUser.lId) 
		{
			*m_oUsersArray[i] = oRecUser;
			UpdateAllViews(nullptr, (LPARAM)ViewUpdate, (CObject*)m_oUsersArray[i]);
			return true;
		}
	}

	return false;
}

bool CUsersDocument::DeleteUser(const long lId) 
{
	CUsersAppService oUsersAppService;
	
	if (!oUsersAppService.DeleteUser(lId)) 
	{
		return false;
	}

	for (int i = 0; i < m_oUsersArray.GetSize(); ++i) 
	{
		if (m_oUsersArray[i]->lId == lId) 
		{
			delete m_oUsersArray[i];
			m_oUsersArray.RemoveAt(i);

			UpdateAllViews(nullptr, (LPARAM)ViewDelete, nullptr);
			return true;
		}
	}
	return false;
}


// Overrides
// ----------------

BOOL CUsersDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}