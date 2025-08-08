#pragma once
#include "CGenericTable.h"

class CProjectsTable : public CGenericTable<PROJECTS>
{
public:
	CProjectsTable() : CGenericTable<PROJECTS>(_T("PROJECTS")) {};
};
