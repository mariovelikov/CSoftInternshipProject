#pragma once
#include "DLLExport.h"
#include "STRUCTURES.h"

class ApplicationDLL_EXP CTasksAppService
{
public:
	bool GetAllTasks(CTasksTypedPtrArray& oTasksTypedPtrArray) const;
	bool AddTask(TASKS& oReccord) const;
	bool UpdateTask(TASKS& oRecord) const;
	bool DeleteTask(const long lID) const;
};

