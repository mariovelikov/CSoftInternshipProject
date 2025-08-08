#include "pch.h"
#include "CProjectsDocument.h"
#include "CProjectsAppService.h"
#include "ViewActions.h"

/////////////////////////////////////////////////////////////////////////////
// CProjectsDocument

// Macros
// ----------------
IMPLEMENT_DYNCREATE(CProjectsDocument, CDocument)

// Constructor / Destructor
// ----------------
CProjectsDocument::CProjectsDocument() noexcept
{
}

CProjectsDocument::~CProjectsDocument()
{
}

//Methods
// ----------------

CProjectsViewItemTypedPtrArray& CProjectsDocument::GetAllProjects()
{
	if (m_oProjectsViewItemArray.IsEmpty())
	{
		CProjectsAppService oProjectsAppService;
		if (!oProjectsAppService.GetAllProjects(m_oProjectsViewItemArray))
		{
			TRACE(_T("Failed to retrieve projects from the database.\n"));
			return m_oProjectsViewItemArray;
		}
		return m_oProjectsViewItemArray;
	};
};

bool CProjectsDocument::AddProject(PROJECTS& oRecProject)
{
	CProjectsAppService oProjectsAppService;
	if (!oProjectsAppService.AddProject(oRecProject))
	{
		return false;
	}

	PROJECTS* pProject = new PROJECTS(oRecProject);
	m_oProjectsArray.Add(pProject);
	UpdateAllViews(nullptr, (LPARAM)ViewAdd, (CObject*)pProject);

	return true;
}

bool CProjectsDocument::UpdateProject(PROJECTS& oRecProject)
{
	CProjectsAppService oProjectsAppService;
	if (!oProjectsAppService.UpdateProject(oRecProject))
	{
		return false;
	}

	for (int i = 0; i < m_oProjectsArray.GetCount(); ++i)
	{
		if (m_oProjectsArray[i]->lId == oRecProject.lId)
		{
			*m_oProjectsArray[i] = oRecProject;
			UpdateAllViews(nullptr, (LPARAM)ViewUpdate, (CObject*)m_oProjectsArray[i]);
			return true;
		}
	}
	return false;
}

bool CProjectsDocument::DeleteProject(const long lId)
{
	CProjectsAppService oProjectsAppService;
	if (!oProjectsAppService.DeleteProject(lId))
	{
		return false;
	}

	for (int i = 0; i < m_oProjectsArray.GetCount(); ++i)
	{
		if (m_oProjectsArray[i]->lId == lId)
		{
			m_oProjectsArray.RemoveAt(i);
			UpdateAllViews(nullptr, (LPARAM)ViewDelete, nullptr);
			return true;
		}
	}
	return false;
}

// Overrides
// ----------------
BOOL CProjectsDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}