#include "pch.h"
#include "CTasksAppService.h"
#include "CTasksTable.h"


bool CTasksAppService::GetAllTasks(CTasksTypedPtrArray& oTasksTypedPtrArray) const
{
	CTasksTable oTasksTable;

	if (!oTasksTable.SelectAll(oTasksTypedPtrArray))
	{
		return false;
	}
	return true;
};

bool CTasksAppService::AddTask(TASKS& oReccord) const
{
	CTasksTable oTasksTable;
	
	if (!oTasksTable.Insert(oReccord))
	{
		return false;
	}
	return true;
};

bool CTasksAppService::UpdateTask(TASKS& oRecord) const
{
	CTasksTable oTasksTable;

	if (!oTasksTable.UpdateWhereID(oRecord.lId, oRecord))
	{
		return false;
	}
	return true;
};

bool CTasksAppService::DeleteTask(const long lID) const
{
	CTasksTable oTasksTable;

	if (!oTasksTable.DeleteWhereID(lID))
	{
		return false;
	}
	return true;
};