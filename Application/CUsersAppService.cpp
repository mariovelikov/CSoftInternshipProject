#include "pch.h"
#include "CUsersAppService.h"
#include "CUsersTable.h"
#include "STRUCTURES.h"

bool CUsersAppService::GetAllUsers(CUsersTypedPtrArray& oUserTypedPtrArray) const
{
	CUsersTable	oUsersTable;

	if (!oUsersTable.SelectAll(oUserTypedPtrArray))
	{
		return false;
	}
	return true;
}	

bool CUsersAppService::AddUser(USERS& oRecUser) const
{
	CUsersTable oUsersTable;
	if (!oUsersTable.Insert(oRecUser))
	{
		return false;
	}
	return true;
}

bool CUsersAppService::UpdateUser(USERS& oRecUser) const
{
	CUsersTable oUsersTable;

	if (!oUsersTable.UpdateWhereID(oRecUser.lId, oRecUser))
	{
		return false;
	}
	return true;
}

bool CUsersAppService::DeleteUser(const long lID) const
{
	CUsersTable oUsersTable;
	if (!oUsersTable.DeleteWhereID(lID))
	{
		return false;
	}
	return true;
}