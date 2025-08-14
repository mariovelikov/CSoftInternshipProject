#pragma once
#include "DLLExport.h"
#include <atldbcli.h>
#include <STRUCTURES.h>
#include "CGenericAccessor.h"


// Project Indexes
#define PROJECTS_ACCESSORS_COUNT 2

#define PROJECTS_IDENTITY_ACCESSOR_INDEX 0
#define PROJECTS_DATA_ACCESSOR_INDEX 1

#define PROJECTS_COLUMN_ENTRY_ID 1
#define PROJECTS_COLUMN_ENTRY_UPDATE_COUNTER 2
#define PROJECTS_COLUMN_ENTRY_NAME 3
#define PROJECTS_COLUMN_ENTRY_DESCRIPTION 4
#define PROJECTS_COLUMN_ENTRY_MANAGER_ID 5
#define PROJECTS_COLUMN_ENTRY_STATE 6
#define PROJECTS_COLUMN_ENTRY_TOTAL_EFFORT 7

/// <summary>
/// Specialization of the CGenericAccessor class for the PROJECTS type, providing access to PROJECTS records and mapping their fields to database columns.
/// </summary>
template<>
class CGenericAccessor<PROJECTS>
{
protected:
	BEGIN_ACCESSOR_MAP(CGenericAccessor<PROJECTS>, PROJECTS_ACCESSORS_COUNT)
		BEGIN_ACCESSOR(PROJECTS_IDENTITY_ACCESSOR_INDEX, true)
			COLUMN_ENTRY(PROJECTS_COLUMN_ENTRY_ID, m_rec.lId)
		END_ACCESSOR()
		BEGIN_ACCESSOR(PROJECTS_DATA_ACCESSOR_INDEX, true)
			COLUMN_ENTRY(PROJECTS_COLUMN_ENTRY_UPDATE_COUNTER, m_rec.lUpdateCounter)
			COLUMN_ENTRY(PROJECTS_COLUMN_ENTRY_NAME, m_rec.szName)
			COLUMN_ENTRY(PROJECTS_COLUMN_ENTRY_DESCRIPTION, m_rec.szDescription)
			COLUMN_ENTRY(PROJECTS_COLUMN_ENTRY_MANAGER_ID, m_rec.lProjectManagerId)
			COLUMN_ENTRY(PROJECTS_COLUMN_ENTRY_STATE, m_rec.nState)
			COLUMN_ENTRY(PROJECTS_COLUMN_ENTRY_TOTAL_EFFORT, m_rec.lTotalEffort)
		END_ACCESSOR()
	END_ACCESSOR_MAP()
public:
	PROJECTS& GetRec() { return m_rec; }
	void SetRec(const PROJECTS& rec) { m_rec = rec; }

protected:
	PROJECTS m_rec;
};

