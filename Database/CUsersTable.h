#pragma once
#include "CGenericTable.h"

class CUsersTable : public CGenericTable<USERS>
{
public:
	CUsersTable(CSession& oSession) : CGenericTable<USERS>(_T("USERS"), oSession) {};
};
