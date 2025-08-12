#pragma once
#include "DLLExport.h"
#include "STRUCTURES.h"

/////////////////////////////////////////////////////////////////////////////
// CProjectsDocument
class DocumentDLL_EXP CProjectsDocument : public CDocument
{
	// Macros
	// ----------------
	DECLARE_DYNCREATE(CProjectsDocument)
	// Constructor / Destructor
	// ----------------

public:
	CProjectsDocument() noexcept;
	virtual ~CProjectsDocument();

	// Methods
// ----------------
public:

	CProjectsViewItemTypedPtrArray& GetAllProjects();
	CUsersTypedPtrArray& GetAllUsers();
	
	bool GetProjectDetails(PROJECT_DETAILS& oProjectDetails);
	bool AddProject(PROJECTS& oRecProject, CTasksTypedPtrArray& oTasks);
	bool UpdateProject(PROJECTS& oRecProject);
	bool DeleteProject(PROJECT_DETAILS& oProjectDetails);
	// Overrides
	// ----------------
	BOOL OnNewDocument() override;


	// Members
	// ----------------
private:
	CProjectsViewItemTypedPtrArray m_oProjectsViewItemArray;
	PROJECT_DETAILS m_oProjectDetails;
	CProjectsTypedPtrArray m_oProjectsArray;
	CUsersTypedPtrArray m_oUsersArray;
};

