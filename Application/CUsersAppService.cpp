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

	CUsersTable oUsersTable(pSession);
	if (!oUsersTable.Insert(oRecUser))
	{
		pSession->Close();
		return false;
	}

	pSession->Close();
	return true;
}

bool CUsersAppService::UpdateUser(USERS& oRecUser) const
{
	CDataSourceConnection& oDataSourceConnection = CDataSourceConnection::GetInstance();
	CDataSource oDataSource = oDataSourceConnection.GetDataSource();
	CSession* pSession = new CSession();

	HRESULT hResult = pSession->Open(oDataSource);
	if (FAILED(hResult)) {
		//PrintError(hResult, _T("Open session failed"));

		return false;
	}

	CUsersTable oUsersTable(pSession);

	if (!oUsersTable.UpdateWhereID(oRecUser.lId, oRecUser))
	{
		pSession->Close();
		return false;
	}

	pSession->Close();
	return true;
}

bool CUsersAppService::DeleteUser(const long lID) const
{
	CDataSourceConnection& oDataSourceConnection = CDataSourceConnection::GetInstance();
	CDataSource oDataSource = oDataSourceConnection.GetDataSource();
	CSession* pSession = new CSession();

	HRESULT hResult = pSession->Open(oDataSource);
	if (FAILED(hResult)) {
		//PrintError(hResult, _T("Open session failed"));

		return false;
	}

	CUsersTable oUsersTable(pSession);

	if (!oUsersTable.DeleteWhereID(lID))
	{
		pSession->Close();
		return false;
	}

	pSession->Close();
	return true;
}