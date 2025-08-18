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

bool CUsersAppService::AddUser(USERS& oRecUser)
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

	// Hash the password before inserting
	CString strHashedPass = HashSHA256(oRecUser.szPassword);
	_tcscpy_s(oRecUser.szPassword, sizeof(oRecUser.szPassword) / sizeof(TCHAR), strHashedPass);
	
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

bool CUsersAppService::UpdateUser(USERS& oRecUser)
{
	CDataSource oDataSource = CDataSourceConnection::GetInstance().GetDataSource();
	CSession oSession;

	HRESULT hResult = oSession.Open(oDataSource);
	if (FAILED(hResult)) {
		return false;
	}

	// If password is provided, hash it before updating
	if (_tcslen(oRecUser.szPassword) > 0) {
		CString strHashedPass = HashSHA256(oRecUser.szPassword);
		_tcscpy_s(oRecUser.szPassword, sizeof(oRecUser.szPassword) / sizeof(TCHAR), strHashedPass);
	}
	else
	{
		CUsersTable oUsersTable(&oSession);
		USERS oExistingUser;
		if (oUsersTable.SelectWhereID(oRecUser.lId, oExistingUser))
		{
			oSession.Close();
			return false;
		};
		_tcscpy_s(oRecUser.szPassword, _countof(oRecUser.szPassword), oExistingUser.szPassword);
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
	HCRYPTPROV hProv = NULL;
	HCRYPTHASH hHash = NULL;
	BYTE hash[32];
	DWORD hashLen = sizeof(hash);
	CString result;

	CT2CA utf8Str(input);
	const char* raw = static_cast<const char*>(utf8Str);

	if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
		return _T("");

	if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash))
	{
		CryptReleaseContext(hProv, 0);
		return _T("");
	}

	if (!CryptHashData(hHash, reinterpret_cast<const BYTE*>(raw), (DWORD)strlen(raw), 0))
	{
		CryptDestroyHash(hHash);
		CryptReleaseContext(hProv, 0);
		return _T("");
	}

	if (CryptGetHashParam(hHash, HP_HASHVAL, hash, &hashLen, 0))
	{
		for (DWORD i = 0; i < hashLen; ++i)
		{
			CString hex;
			hex.Format(_T("%02x"), hash[i]);
			result += hex;
		}
	}

	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);

	return result;
}
