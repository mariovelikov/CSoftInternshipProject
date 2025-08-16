#include "pch.h"
#include "CUsersAppService.h"
#include "CUsersTable.h"
#include "STRUCTURES.h"

bool CUsersAppService::GetAllUsers(CUsersMap& oUsersMap) const
{
	CDataSource oDataSource = CDataSourceConnection::GetInstance().GetDataSource();
	CSession* pSession = new CSession();

	HRESULT hResult = pSession->Open(oDataSource);
	if (FAILED(hResult)) {
		return false;
	}

	CUsersTable	oUsersTable(pSession);

	CUsersTypedPtrArray oUserTypedPtrArray;
	if (!oUsersTable.SelectAll(oUserTypedPtrArray))
	{
		pSession->Close();
		return false;
	}

	// fill map with users
	for (int i = 0; i < oUserTypedPtrArray.GetCount(); i++)
	{
		USERS* pUser = new USERS(*oUserTypedPtrArray[i]);
		oUsersMap.SetAt(pUser->lId, pUser);
	}

	pSession->Close();
	return true;
}	

bool CUsersAppService::AddUser(USERS& oRecUser) const
{
	CDataSource oDataSource = CDataSourceConnection::GetInstance().GetDataSource();
	CSession* pSession = new CSession();

	HRESULT hResult = pSession->Open(oDataSource);
	if (FAILED(hResult)) {
		return false;
	}

	if (FAILED(pSession->StartTransaction()))
	{
		return false;
	}

	CUsersTable oUsersTable(pSession);
	if (!oUsersTable.Insert(oRecUser))
	{
		pSession->Abort();
		pSession->Close();
		return false;
	}

	pSession->Commit();
	pSession->Close();
	return true;
}

bool CUsersAppService::UpdateUser(USERS& oRecUser) const
{
	CDataSource oDataSource = CDataSourceConnection::GetInstance().GetDataSource();
	CSession* pSession = new CSession();

	HRESULT hResult = pSession->Open(oDataSource);
	if (FAILED(hResult)) {
		return false;
	}

	if (FAILED(pSession->StartTransaction()))
	{
		return false;
	}

	CUsersTable oUsersTable(pSession);

	if (!oUsersTable.UpdateWhereID(oRecUser.lId, oRecUser))
	{
		pSession->Abort();
		pSession->Close();
		return false;
	}

	pSession->Commit();
	pSession->Close();
	return true;
}

bool CUsersAppService::DeleteUser(const long lID) const
{
	CDataSource oDataSource = CDataSourceConnection::GetInstance().GetDataSource();
	CSession* pSession = new CSession();

	HRESULT hResult = pSession->Open(oDataSource);
	if (FAILED(hResult)) {
		return false;
	}

	if (FAILED(pSession->StartTransaction()))
	{
		return false;
	}

	CUsersTable oUsersTable(pSession);

	if (!oUsersTable.DeleteWhereID(lID))
	{
		pSession->Abort();
		pSession->Close();
		return false;
	}

	pSession->Commit();
	pSession->Close();
	return true;
}