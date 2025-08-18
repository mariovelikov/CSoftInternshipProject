#pragma once
#include "CGenericTable.h"

class CUsersTable : public CGenericTable<USERS>
{
public:
	CUsersTable(CSession* oSession = nullptr) 
		: CGenericTable<USERS>(_T("USERS"), oSession) {};
};
