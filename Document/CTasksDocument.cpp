#include "pch.h"
#include "CTasksDocument.h"
#include "CTasksAppService.h"
#include "ViewActions.h"

/////////////////////////////////////////////////////////////////////////////
// CTasksDocument

// Macros
// ----------------
IMPLEMENT_DYNCREATE(CTasksDocument, CDocument)

// Constructor / Destructor
// ----------------
CTasksDocument::CTasksDocument() noexcept
{
}

CTasksDocument::~CTasksDocument()
{
}

//Methods
// ----------------

CTasksTypedPtrArray& CTasksDocument::GetAllProjects()
{
	if (m_oTasksArray.IsEmpty())
	{
		CTasksAppService oTasksAppService;
		if (!oTasksAppService.GetAllTasks(m_oTasksArray))
		{
			TRACE(_T("Failed to retrieve projects from the database.\n"));
			return m_oTasksArray;
		}
		return m_oTasksArray;
	};
};

bool CTasksDocument::AddTask(TASKS& oRecTask)
{
	CTasksAppService oTasksAppService;
	if (!oTasksAppService.AddTask(oRecTask))
	{
		return false;
	}

	TASKS* pTask = new TASKS(oRecTask);
	m_oTasksArray.Add(pTask);
	UpdateAllViews(nullptr, (LPARAM)ViewAdd, (CObject*)pTask);

	return true;
}

bool CTasksDocument::UpdateTask(TASKS& oRecTask)
{
	CTasksAppService oProjectsAppService;
	if (!oProjectsAppService.UpdateTask(oRecTask))
	{
		return false;
	}

	for (int i = 0; i < m_oTasksArray.GetCount(); ++i)
	{
		if (m_oTasksArray[i]->lId == oRecTask.lId)
		{
			*m_oTasksArray[i] = oRecTask;
			UpdateAllViews(nullptr, (LPARAM)ViewUpdate, (CObject*)m_oTasksArray[i]);
			return true;
		}
	}
	return false;
}

bool CTasksDocument::DeleteTask(const long lId)
{
	CTasksAppService oProjectsAppService;
	if (!oProjectsAppService.DeleteTask(lId))
	{
		return false;
	}

	for (int i = 0; i < m_oTasksArray.GetCount(); ++i)
	{
		if (m_oTasksArray[i]->lId == lId)
		{
			m_oTasksArray.RemoveAt(i);
			UpdateAllViews(nullptr, (LPARAM)ViewDelete, nullptr);
			return true;
		}
	}
	return false;
}

// Overrides
// ----------------
BOOL CTasksDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}