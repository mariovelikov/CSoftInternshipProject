#pragma once
#include "CGenericTable.h"

class CProjectsTable : public CGenericTable<PROJECTS>
{
public:
	CProjectsTable(CSession& oSession) : CGenericTable<PROJECTS>(_T("PROJECTS"), oSession) {};
};
