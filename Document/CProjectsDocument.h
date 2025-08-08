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
	bool AddProject(PROJECTS& oRecProject);
	bool UpdateProject(PROJECTS& oRecProject);
	bool DeleteProject(const long lId);

	// Overrides
	// ----------------
	BOOL OnNewDocument() override;


	// Members
	// ----------------
private:
	CProjectsViewItemTypedPtrArray m_oProjectsViewItemArray;
	CProjectsTypedPtrArray m_oProjectsArray;
};

