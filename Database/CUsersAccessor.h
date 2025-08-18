#pragma once
#include "DLLExport.h"
#include <atldbcli.h>
#include <STRUCTURES.h>
#include "CGenericAccessor.h"

// Indexes
#define USERS_ACCESSORS_COUNT 2

#define USERS_IDENTITY_ACCESSOR_INDEX 0
#define USERS_DATA_ACCESSOR_INDEX 1

#define USERS_COLUMN_ENTRY_ID 1
#define USERS_COLUMN_ENTRY_UPDATE_COUNTER 2
#define USERS_COLUMN_ENTRY_NAME 3
#define USERS_COLUMN_ENTRY_EMAIL 4
#define USERS_COLUMN_ENTRY_JOB_TITLE 5
#define USERS_COLUMN_ENTRY_PASSWORD 6

/// <summary> Accessor class for Users</summary>
template<>
class CGenericAccessor<USERS>
{
protected:
	BEGIN_ACCESSOR_MAP(CGenericAccessor<USERS>, USERS_ACCESSORS_COUNT)
		BEGIN_ACCESSOR(USERS_IDENTITY_ACCESSOR_INDEX, true)
			COLUMN_ENTRY(USERS_COLUMN_ENTRY_ID, m_rec.lId)
		END_ACCESSOR()
		BEGIN_ACCESSOR(USERS_DATA_ACCESSOR_INDEX, true)
			COLUMN_ENTRY(USERS_COLUMN_ENTRY_UPDATE_COUNTER, m_rec.lUpdateCounter)
			COLUMN_ENTRY(USERS_COLUMN_ENTRY_NAME, m_rec.szName)
			COLUMN_ENTRY(USERS_COLUMN_ENTRY_EMAIL, m_rec.szEmail)
			COLUMN_ENTRY(USERS_COLUMN_ENTRY_JOB_TITLE, m_rec.szJobTitle)
			COLUMN_ENTRY(USERS_COLUMN_ENTRY_PASSWORD, m_rec.szPassword)
		END_ACCESSOR()
	END_ACCESSOR_MAP()

public:
	USERS& GetRec() { return m_rec; }
	void SetRec(const USERS& rec) { m_rec = rec; }

protected:
	USERS m_rec;
};
