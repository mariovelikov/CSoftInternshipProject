#pragma once
#include "CGenericTable.h"

class CUsersTable : public CGenericTable<USERS>
{
public:
	CUsersTable() : CGenericTable<USERS>(_T("USERS")) {};
};
