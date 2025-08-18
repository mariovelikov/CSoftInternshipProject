#include "pch.h"
#include "CUsersAppService.h"
#include "CUsersTable.h"
#include "STRUCTURES.h"

bool CUsersAppService::GetAllUsers(CUsersMap& oUsersMap) const
{
	CUsersTable	oUsersTable;

	CUsersTypedPtrArray oUserTypedPtrArray;
	if (!oUsersTable.SelectAll(oUserTypedPtrArray))
	{
		return false;
	}

	// fill map with users
	for (int i = 0; i < oUserTypedPtrArray.GetCount(); i++)
	{
		USERS* pUser = new USERS(*oUserTypedPtrArray[i]);
		oUsersMap.SetAt(pUser->lId, pUser);
	}

	return true;
}	

bool CUsersAppService::AddUser(USERS& oRecUser) const
{
	CDataSource oDataSource = CDataSourceConnection::GetInstance().GetDataSource();
	CSession oSession;

	HRESULT hResult = oSession.Open(oDataSource);
	if (FAILED(hResult)) {
		return false;
	}

	if (FAILED(oSession.StartTransaction()))
	{
		return false;
	}

	CUsersTable oUsersTable(&oSession);
	if (!oUsersTable.Insert(oRecUser))
	{
		oSession.Abort();
		oSession.Close();
		return false;
	}

	oSession.Commit();
	oSession.Close();
	return true;
}

bool CUsersAppService::UpdateUser(USERS& oRecUser) const
{
	CDataSource oDataSource = CDataSourceConnection::GetInstance().GetDataSource();
	CSession oSession;

	HRESULT hResult = oSession.Open(oDataSource);
	if (FAILED(hResult)) {
		return false;
	}

	if (FAILED(oSession.StartTransaction()))
	{
		return false;
	}

	CUsersTable oUsersTable(&oSession);

	if (!oUsersTable.UpdateWhereID(oRecUser.lId, oRecUser))
	{
		oSession.Abort();
		oSession.Close();
		return false;
	}

	oSession.Commit();
	oSession.Close();
	return true;
}

bool CUsersAppService::DeleteUser(const long lID) const
{
	CDataSource oDataSource = CDataSourceConnection::GetInstance().GetDataSource();
	CSession oSession;

	HRESULT hResult = oSession.Open(oDataSource);
	if (FAILED(hResult)) {
		return false;
	}

	if (FAILED(oSession.StartTransaction()))
	{
		return false;
	}

	CUsersTable oUsersTable(&oSession);

	if (!oUsersTable.DeleteWhereID(lID))
	{
		oSession.Abort();
		oSession.Close();
		return false;
	}

	oSession.Commit();
	oSession.Close();
	return true;
}


bool CUsersAppService::ClientAuthentication(const CString& strEmail, const CString& strPassword)
{
	CDataSource oDataSource = CDataSourceConnection::GetInstance().GetDataSource();
	CSession oSession;
	if (FAILED(oSession.Open(oDataSource)))
	{
		return false;
	}

	CUsersTable oUsersTable(&oSession);
	USERS recUser;
	if (!oUsersTable.LoadRecordByColumnValue(recUser, _T("EMAIL"), (LPCTSTR)strEmail))
	{
		return false;
	}


	if (recUser.lId != _ttoi(strPassword))
	{
		return false;
	}

	return true;
}

CString CUsersAppService::HashSHA256(const CString& input)
{

	CString hashedValue = input; 
	return hashedValue;
}