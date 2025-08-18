#pragma once
#include "STRUCTURES.h"
#include "DLLExport.h"
#include <wincrypt.h>
#pragma comment(lib, "advapi32.lib")

class ApplicationDLL_EXP CUsersAppService
{
public:
	bool GetAllUsers(CUsersMap& oUsersMap) const;

	/// <summary> Add user to the database and hash the password in function before insert</summary>
	bool AddUser(USERS& oRecUser);
	bool UpdateUser(USERS& oRecUser) const;
	bool DeleteUser(const long lID) const;

private:
	/// <summary>Create hash of CString </summary>
	/// <param name="input"></param>
	/// <returns>Hashed string</returns>
	CString HashSHA256(const CString& input);

public:
	bool ClientAuthentication(const CString& strEmail, const CString& strPassword);
};