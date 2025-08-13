#pragma once
#include "STRUCTURES.h"
#include "DLLExport.h"

class ApplicationDLL_EXP CProjectsAppService
{
public:
	bool GetAllProjects(CProjectsViewItemTypedPtrArray& oProjectsViewItemTypedPtrArray) const;
	bool GetProjectDetails(PROJECT_DETAILS& oProjectDetails) const;
	bool AddProject(PROJECTS& oRecord, CTasksTypedPtrArray& oTasksArray) const;
	bool UpdateProject(PROJECT_DETAILS& oRecord) const;
	bool DeleteProject(const PROJECT_DETAILS& oProjectDetails) const;
};