#include "pch.h"
#include "CProjectsAppService.h"
#include "CProjectsTable.h"
#include "CUsersTable.h"
#include "CTasksTable.h"
#include "STRUCTURES.h"


bool CProjectsAppService::GetAllProjects(CProjectsViewItemsTypedPtrArray& oProjectsViewItemsArray) const
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
		PROECTS_VIEW_ITEM* pViewItem = new PROECTS_VIEW_ITEM();

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
				//pViewItem->szProjectManagerName = pUser->szName;
				break;
			}
		}
		oProjectsViewItemsArray.Add(pViewItem);
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