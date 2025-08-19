#pragma once
#include "CGenericTable.h"

class CProjectsTable : public CGenericTable<PROJECTS>
{
public:
	CProjectsTable(CSession* oSession = nullptr) : CGenericTable<PROJECTS>(_T("PROJECTS"), oSession) {};
};
