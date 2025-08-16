#pragma once
#include "CGenericTable.h"

class CTasksTable : public CGenericTable<TASKS>
{
public:
	CTasksTable(CSession* oSession) : CGenericTable<TASKS>(_T("TASKS"), oSession) {};
};
