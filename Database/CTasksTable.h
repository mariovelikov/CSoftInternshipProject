#pragma once
#include "CGenericTable.h"

class CTasksTable : public CGenericTable<TASKS>
{
public:
	CTasksTable() : CGenericTable<TASKS>(_T("TASKS")) {};
};
