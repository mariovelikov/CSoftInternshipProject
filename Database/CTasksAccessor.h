#pragma once
#include "DLLExport.h"
#include <atldbcli.h>
#include <STRUCTURES.h>
#include "CGenericAccessor.h"

// Tasks Indexes
#define TASKS_ACCESSORS_COUNT 2

#define TASKS_IDENTITY_ACCESSOR_INDEX 0
#define TASKS_DATA_ACCESSOR_INDEX 1

#define TASKS_COLUMN_ENTRY_ID 1
#define TASKS_COLUMN_ENTRY_UPDATE_COUNTER 2
#define TASKS_COLUMN_ENTRY_NAME 3
#define TASKS_COLUMN_ENTRY_DESCRIPTION 4
#define TASKS_COLUMN_ENTRY_PROJECT_ID 5
#define TASKS_COLUMN_ENTRY_USER_ID 6
#define TASKS_COLUMN_ENTRY_STATE 7
#define TASKS_COLUMN_ENTRY_TOTAL_EFFORT 8

/// <summary>
/// A template specialization of CGenericAccessor for TASKS, providing access to TASKS record fields and mapping them to database columns.
/// </summary>
template<>
class CGenericAccessor<TASKS>
{
protected:
	BEGIN_ACCESSOR_MAP(CGenericAccessor<TASKS>, TASKS_ACCESSORS_COUNT)
		BEGIN_ACCESSOR(TASKS_IDENTITY_ACCESSOR_INDEX, true)
			COLUMN_ENTRY(TASKS_COLUMN_ENTRY_ID, m_rec.lId)
		END_ACCESSOR()
		BEGIN_ACCESSOR(TASKS_DATA_ACCESSOR_INDEX, true)
			COLUMN_ENTRY(TASKS_COLUMN_ENTRY_UPDATE_COUNTER, m_rec.lUpdateCounter)
			COLUMN_ENTRY(TASKS_COLUMN_ENTRY_NAME, m_rec.szName)
			COLUMN_ENTRY(TASKS_COLUMN_ENTRY_DESCRIPTION, m_rec.szDescription)
			COLUMN_ENTRY(TASKS_COLUMN_ENTRY_PROJECT_ID, m_rec.lProjectId)
			COLUMN_ENTRY(TASKS_COLUMN_ENTRY_USER_ID, m_rec.lUserId)
			COLUMN_ENTRY(TASKS_COLUMN_ENTRY_STATE, m_rec.nState)
			COLUMN_ENTRY(TASKS_COLUMN_ENTRY_TOTAL_EFFORT, m_rec.nTotalEffort)
		END_ACCESSOR()
	END_ACCESSOR_MAP()

public:
	TASKS& GetRec() { return m_rec; }
	void SetRec(const TASKS& rec) { m_rec = rec; }

protected:
	TASKS m_rec;
};

typedef CGenericAccessor<TASKS> CTasksAccessor;