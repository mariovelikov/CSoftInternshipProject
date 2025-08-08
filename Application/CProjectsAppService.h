#pragma once
#include "STRUCTURES.h"
#include "DLLExport.h"

class ApplicationDLL_EXP CProjectsAppService
{
public:
	bool GetAllProjects(CProjectsViewItemTypedPtrArray& oProjectsViewItemTypedPtrArray) const;
	bool AddProject(PROJECTS& oRecord) const; // PROJECT_DETAILS
	bool UpdateProject(PROJECTS& oRecord) const; // PROJECT_DETAILS
	bool DeleteProject(const long lID) const;
};