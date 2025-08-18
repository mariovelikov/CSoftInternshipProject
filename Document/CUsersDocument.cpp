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

CUsersMap& CUsersDocument::GetAllUsers()
{
	if (m_oUsersMap.IsEmpty()) 
	{
		CUsersAppService oUsersAppService;
		
		if (!oUsersAppService.GetAllUsers(m_oUsersMap))
		{
			TRACE(_T("Failed to retrieve users from the database.\n"));
			return m_oUsersMap;
		}
	}

	return m_oUsersMap;
}

bool CUsersDocument::AddUser(USERS& oRecUser) 
{
	CUsersAppService oUsersAppService;

	if (!oUsersAppService.AddUser(oRecUser))
	{
		return false;
	}
	USERS* pUser = new USERS(oRecUser);
	m_oUsersMap.SetAt(pUser->lId, pUser);

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

	UpdateAllViews(nullptr, (LPARAM)ViewUpdate, (CObject*)m_oUsersMap[oRecUser.lId]);
	return true;

	return false;
}

bool CUsersDocument::DeleteUser(const long lId) 
{
	CUsersAppService oUsersAppService;
	
	if (!oUsersAppService.DeleteUser(lId)) 
	{
		return false;
	}

	delete m_oUsersMap[lId];
	m_oUsersMap.RemoveKey(lId);

	UpdateAllViews(nullptr, (LPARAM)ViewDelete, nullptr);
	return true;
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

bool CUsersDocument::ClientAuthentication(const CString& strEmail, const CString& strPassword)
{
	return CUsersAppService().ClientAuthentication( strEmail, strPassword);
}