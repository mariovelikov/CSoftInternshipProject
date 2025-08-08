#pragma once
#include "DLLExport.h"
#include "STRUCTURES.h"

/////////////////////////////////////////////////////////////////////////////
// CTasksDocument

class DocumentDLL_EXP CTasksDocument : public CDocument
{
	// Macros
	// ----------------
	DECLARE_DYNCREATE(CTasksDocument)

	// Constructor / Destructor
	// ----------------

public:
	CTasksDocument() noexcept;
	virtual ~CTasksDocument();

	// Methods
// ----------------
public:

	CTasksTypedPtrArray& GetAllProjects();
	bool AddTask(TASKS& oRecProject);
	bool UpdateTask(TASKS& oRecProject);
	bool DeleteTask(const long lId);

	// Overrides
	// ----------------
	BOOL OnNewDocument() override;


	// Members
	// ----------------
private:
	CTasksTypedPtrArray m_oTasksArray;
};

