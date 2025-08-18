#pragma once
#include "STRUCTURES.h"
#include "DLLExport.h"
#include <wincrypt.h>
#pragma comment(lib, "advapi32.lib")

class ApplicationDLL_EXP CUsersAppService
{
public:
	bool GetAllUsers(CUsersMap& oUsersMap) const;
	bool AddUser(USERS& oRecUser) const;
	bool UpdateUser(USERS& oRecUser) const;
	bool DeleteUser(const long lID) const;

private:
	/// <summary>Create hash of CString </summary>
	/// <param name="input"></param>
	/// <returns></returns>
	CString HashSHA256(const CString& input);

public:
	bool ClientAuthentication(const CString& strEmail, const CString& strPassword);
};