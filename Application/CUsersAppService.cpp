#include "pch.h"
#include "CUsersAppService.h"
#include "CUsersTable.h"
#include "STRUCTURES.h"

bool CUsersAppService::GetAllUsers(CUsersTypedPtrArray& oUserTypedPtrArray) const
{
	CDataSourceConnection& oDataSourceConnection = CDataSourceConnection::GetInstance();
	CDataSource oDataSource = oDataSourceConnection.GetDataSource();
	CSession oSession;

	HRESULT hResult = oSession.Open(oDataSource);
	if (FAILED(hResult)) {
		//PrintError(hResult, _T("Open session failed"));

		return false;
	}

	CUsersTable	oUsersTable(oSession);

	if (!oUsersTable.SelectAll(oUserTypedPtrArray))
	{
		oSession.Close();
		return false;
	}

	oSession.Close();
	return true;
}	

bool CUsersAppService::AddUser(USERS& oRecUser) const
{
	CDataSourceConnection& oDataSourceConnection = CDataSourceConnection::GetInstance();
	CDataSource oDataSource = oDataSourceConnection.GetDataSource();
	CSession oSession;

	HRESULT hResult = oSession.Open(oDataSource);
	if (FAILED(hResult)) {
		//PrintError(hResult, _T("Open session failed"));

		return false;
	}

	CUsersTable oUsersTable(oSession);
	if (!oUsersTable.Insert(oRecUser))
	{
		oSession.Close();
		return false;
	}

	oSession.Close();
	return true;
}

bool CUsersAppService::UpdateUser(USERS& oRecUser) const
{
	CDataSourceConnection& oDataSourceConnection = CDataSourceConnection::GetInstance();
	CDataSource oDataSource = oDataSourceConnection.GetDataSource();
	CSession oSession;

	HRESULT hResult = oSession.Open(oDataSource);
	if (FAILED(hResult)) {
		//PrintError(hResult, _T("Open session failed"));

		return false;
	}

	CUsersTable oUsersTable(oSession);

	if (!oUsersTable.UpdateWhereID(oRecUser.lId, oRecUser))
	{
		oSession.Close();
		return false;
	}

	oSession.Close();
	return true;
}

bool CUsersAppService::DeleteUser(const long lID) const
{
	CDataSourceConnection& oDataSourceConnection = CDataSourceConnection::GetInstance();
	CDataSource oDataSource = oDataSourceConnection.GetDataSource();
	CSession oSession;

	HRESULT hResult = oSession.Open(oDataSource);
	if (FAILED(hResult)) {
		//PrintError(hResult, _T("Open session failed"));

		return false;
	}

	CUsersTable oUsersTable(oSession);

	if (!oUsersTable.DeleteWhereID(lID))
	{
		oSession.Close();
		return false;
	}

	oSession.Close();
	return true;
}