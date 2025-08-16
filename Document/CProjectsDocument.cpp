#include "pch.h"
#include "CProjectsDocument.h"
#include "CProjectsAppService.h"
#include "CUsersAppService.h"
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

bool CProjectsDocument::GetProjectDetails(PROJECT_DETAILS& oProjectDetails)
{
	CProjectsAppService oProjectsAppService;
	if (!oProjectsAppService.GetProjectDetails(oProjectDetails))
	{
		return false;
	}
	return true;
};

bool CProjectsDocument::AddProject(PROJECTS& oRecProject, CTasksTypedPtrArray& oTasks)
{
	CProjectsAppService oProjectsAppService;
	if (!oProjectsAppService.AddProject(oRecProject, oTasks))
	{
		return false;
	}

	PROJECTS* pProject = new PROJECTS(oRecProject);
	m_oProjectsArray.Add(pProject);
	
	PROJECTS_VIEW_ITEM* pViewItem = new PROJECTS_VIEW_ITEM();
	pViewItem->recProject = *pProject;

	_tcscpy_s(pViewItem->szProjectManagerName, _countof(pViewItem->szProjectManagerName), m_oUsersMap[pProject->lProjectManagerId]->szName);

	m_oProjectsViewItemArray.Add(pViewItem);
	UpdateAllViews(nullptr, (LPARAM)ViewAdd, (CObject*)pViewItem);

	return true;
}

bool CProjectsDocument::UpdateProject(PROJECT_DETAILS& oRecProject, PROJECTS_VIEW_ITEM *pProjectViewItem, CString strProjectManagerName)
{
	CProjectsAppService oProjectsAppService;
	if (!oProjectsAppService.UpdateProject(oRecProject))
	{
		return false;
	}

	_tcscpy_s(pProjectViewItem->szProjectManagerName, _countof(pProjectViewItem->szProjectManagerName), strProjectManagerName);

	for (int i = 0; i < m_oProjectsViewItemArray.GetCount(); ++i)
	{
		if (m_oProjectsViewItemArray[i]->recProject.lId == oRecProject.recProject.lId)
		{
			m_oProjectsViewItemArray[i]->recProject = oRecProject.recProject;
			UpdateAllViews(nullptr, (LPARAM)ViewUpdate, (CObject*)m_oProjectsViewItemArray[i]);
			return true;
		}
	}
	return false;
}

bool CProjectsDocument::DeleteProject(PROJECT_DETAILS& oProjectDetails)
{
	CProjectsAppService oProjectsAppService;
	if (!oProjectsAppService.DeleteProject(oProjectDetails))
	{
		return false;
	}

	for (int i = 0; i < m_oProjectsViewItemArray.GetCount(); ++i)
	{
		if (m_oProjectsViewItemArray[i]->recProject.lId == oProjectDetails.recProject.lId)
		{
			m_oProjectsViewItemArray.RemoveAt(i);
			UpdateAllViews(nullptr, (LPARAM)ViewDelete, nullptr);
			return true;
		}
	}
}

CUsersMap& CProjectsDocument::GetAllUsers()
{
	if (m_oUsersMap.IsEmpty())
	{
		CUsersAppService oProjectsAppService;
		if (!oProjectsAppService.GetAllUsers(m_oUsersMap))
		{
			TRACE(_T("Failed to retrieve users from the database.\n"));
			return m_oUsersMap;
		}
		return m_oUsersMap;
	}
	else {
		return m_oUsersMap;
	}
}

// Overrides
// ----------------
BOOL CProjectsDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}