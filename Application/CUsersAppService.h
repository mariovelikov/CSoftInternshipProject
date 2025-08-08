#pragma once
#include "STRUCTURES.h"
#include "DLLExport.h"

class ApplicationDLL_EXP CUsersAppService
{
public:
	bool GetAllUsers(CUsersTypedPtrArray& oUserTypedPtrArray) const;
	bool AddUser(USERS& oRecUser) const;
	bool UpdateUser(USERS& oRecUser) const;
	bool DeleteUser(const long lID) const;
};