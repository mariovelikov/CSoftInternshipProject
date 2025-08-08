#include "pch.h"
#include "CProjectsAppService.h"
#include "CProjectsTable.h"
#include "CUsersTable.h"
#include "CTasksTable.h"
#include "STRUCTURES.h"


bool CProjectsAppService::GetAllProjects(CProjectsViewItemTypedPtrArray& oProjectsViewItemArray) const
{
	CProjectsTable oProjetsTable;
	CUsersTable oUsersTable;

	CUsersTypedPtrArray oUsersTypedPtrArray;
	if (!oUsersTable.SelectAll(oUsersTypedPtrArray))
	{
		return false;
	}

	CProjectsTypedPtrArray oProjectsTypedPtrArray;
	if (!oProjetsTable.SelectAll(oProjectsTypedPtrArray))
	{
		return false;
	}

	for (int i = 0; i < oProjectsTypedPtrArray.GetCount(); i++)
	{
		PROJECTS* pProject = oProjectsTypedPtrArray.GetAt(i);
		if (pProject == nullptr)
		{
			continue;
		}
		PROJECTS_VIEW_ITEM* pViewItem = new PROJECTS_VIEW_ITEM();

		pViewItem->recProject = *pProject;

		for (int j = 0; j < oUsersTypedPtrArray.GetCount(); j++)
		{
			USERS* pUser = oUsersTypedPtrArray.GetAt(j);
			if (pUser == nullptr)
			{
				continue;
			}

			if (pUser->lId == pProject->lId)
			{
				_tcscpy_s(pViewItem->szProjectManagerName, (sizeof(pViewItem->szProjectManagerName) / sizeof(pViewItem->szProjectManagerName[0])), pUser->szName);
				break;
			}
		}
		oProjectsViewItemArray.Add(pViewItem);
	}

	return true;
};

bool CProjectsAppService::AddProject(PROJECTS& oRecord) const
{
	CProjectsTable oProjetsTable;
	if (!oProjetsTable.Insert(oRecord))
	{
		return false;
	}
	return true;
};

bool CProjectsAppService::UpdateProject(PROJECTS& oRecord) const // 
{
	CProjectsTable oProjetsTable;

	if (!oProjetsTable.UpdateWhereID(oRecord.lId, oRecord))
	{
		return false;
	}
	return true;
};

bool CProjectsAppService::DeleteProject(const long lID) const
{
	CSession oSession;
	oSession.StartTransaction();

	CTasksTable oTasksTable; // put in tables
	//delete all tasks associated with the project

	CProjectsTable oProjetsTable; // put in tables


	if (!oProjetsTable.DeleteWhereID(lID))
	{
		//oSession.Rollback();
		return false;
	}

	oSession.Close();
	return true;
};