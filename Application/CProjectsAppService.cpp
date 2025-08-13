#include "pch.h"
#include "CProjectsAppService.h"
#include "CProjectsTable.h"
#include "CUsersTable.h"
#include "CTasksTable.h"
#include "STRUCTURES.h"


bool CProjectsAppService::GetAllProjects(CProjectsViewItemTypedPtrArray& oProjectsViewItemArray) const
{
	CDataSourceConnection& oDataSourceConnection = CDataSourceConnection::GetInstance();
	CDataSource oDataSource = oDataSourceConnection.GetDataSource();
	CSession oSession;

	HRESULT hResult = oSession.Open(oDataSource);
	if (FAILED(hResult)) {
		//PrintError(hResult, _T("Open session failed"));

		return false;
	}

	if (FAILED(hResult))
	{
		oSession.Close();
		return false;
	}

	CProjectsTable oProjetsTable(oSession);
	CUsersTable oUsersTable(oSession);

	CUsersTypedPtrArray oUsersTypedPtrArray;
	if (!oUsersTable.SelectAll(oUsersTypedPtrArray))
	{
		oSession.Close();
		return false;
	}

	CProjectsTypedPtrArray oProjectsTypedPtrArray;
	if (!oProjetsTable.SelectAll(oProjectsTypedPtrArray))
	{
		oSession.Close();
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

			if (pUser->lId == pProject->lProjectManagerId)
			{
				_tcscpy_s(pViewItem->szProjectManagerName, (sizeof(pViewItem->szProjectManagerName) / sizeof(pViewItem->szProjectManagerName[0])), pUser->szName);
				break;
			}
		}
		oProjectsViewItemArray.Add(pViewItem);
	}

	oSession.Close();
	return true;
};

bool CProjectsAppService::GetProjectDetails(PROJECT_DETAILS& oProjectDetails) const
{
	CDataSourceConnection& oDataSourceConnection = CDataSourceConnection::GetInstance();
	CDataSource oDataSource = oDataSourceConnection.GetDataSource();
	CSession oSession;

	HRESULT hResult = oSession.Open(oDataSource);
	if (FAILED(hResult)) {
		CString errorMessage;
		errorMessage.Format(_T("Open session failed. HRESULT: %d"), hResult);
		TRACE(errorMessage);

		oSession.Close();
		return false;
	}

	CTasksTable oTasksTable(oSession);
	CTasksTypedPtrArray oTasks; 
	if (!oTasksTable.SelectAll(oTasks)) {
		oSession.Close();
		return false;
	}

	for (int i = 0; i < oTasks.GetCount(); i++) 
	{
		CString name = oTasks[i]->szName;

		if (oTasks.GetAt(i)->lProjectId == oProjectDetails.recProject.lId) 
		{
			TASKS oTask = *oTasks[i];
			TASKS* pTask = new TASKS();
			*pTask = oTask;
			oProjectDetails.oTasksTypedPtrArray.Add(pTask);
		}
	}

	oSession.Close();
	return true;
}

bool CProjectsAppService::AddProject(PROJECTS& oRecord, CTasksTypedPtrArray& oTasksArray) const
{
	CDataSource oDataSource = CDataSourceConnection::GetInstance().GetDataSource();
	CSession oSession;

	HRESULT hResult = oSession.Open(oDataSource);
	if (FAILED(hResult)) {
		CString errorMessage;
		errorMessage.Format(_T("Open session failed. HRESULT: %d"), hResult);
		TRACE(errorMessage);

		oSession.Close();
		return false;
	}

	oSession.StartTransaction();
	CProjectsTable oProjetsTable(oSession);
	CTasksTable oTasksTable(oSession);

	// FIRST WE ADD PROJECT
	if (!oProjetsTable.Insert(oRecord))
	{
		oSession.Abort();
		oSession.Close();
		return false;
	}

	// LOOP ALL TASKS AND ADD TO PROJECT
	for (int i = 0; i < oTasksArray.GetCount(); i++) {
		oTasksArray[i]->lProjectId = oRecord.lId;
		if (!oTasksTable.Insert(*oTasksArray.GetAt(i)))
		{
			oSession.Abort();
			oSession.Close();
			return false;
		};
	}

	oSession.Commit();
	oSession.Close();
	return true;
	
};

bool CProjectsAppService::UpdateProject(PROJECT_DETAILS& oRecord) const // 
{
	CDataSourceConnection& oDataSourceConnection = CDataSourceConnection::GetInstance();
	CDataSource oDataSource = oDataSourceConnection.GetDataSource();
	CSession oSession;

	HRESULT hResult = oSession.Open(oDataSource);
	if (FAILED(hResult)) {
		//PrintError(hResult, _T("Open session failed"));
		oSession.Close();
		return false;
	}

	oSession.StartTransaction();

	CTasksTable oTasksTable(oSession);

	//Delete tasks
	for (int i = 0; i < oRecord.m_oTaskIdsToDelete.GetCount(); i++)
	{
		if (!oTasksTable.DeleteWhereID(oRecord.m_oTaskIdsToDelete.GetAt(i)))
		{
			oSession.Abort();
			oSession.Close();
			return false;
		}
	}

	// Add tasks
	for (int i = 0; i < oRecord.oTasksTypedPtrArray.GetCount(); i++)
	{
		if (oRecord.oTasksTypedPtrArray.GetAt(i)->lId != 0)
		{
			continue;
		}

		TASKS& oTask = *oRecord.oTasksTypedPtrArray[i];
		oTasksTable.Insert(oTask);
	}

	// Update tasks 
	for (int i = 0; i < oRecord.oTasksTypedPtrArray.GetCount(); i++) {
		if (!oTasksTable.UpdateWhereID(oRecord.oTasksTypedPtrArray.GetAt(i)->lId, *oRecord.oTasksTypedPtrArray[i]))
		{
			oSession.Abort();
			oSession.Close();
			return false;
		}
	}

	CProjectsTable oProjetsTable(oSession);
	
	//Update project
	if (!oProjetsTable.UpdateWhereID(oRecord.recProject.lId, oRecord.recProject))
	{
		oSession.Abort();
		oSession.Close(); 
		return false;
	}

	oSession.Commit();
	oSession.Close();
	return true;
};

bool CProjectsAppService::DeleteProject(const PROJECT_DETAILS& oProjectDetails) const
{
	CDataSource oDataSource = CDataSourceConnection::GetInstance().GetDataSource();
	CSession oSession;

	HRESULT hResult = oSession.Open(oDataSource);
	if (FAILED(hResult)) {
		oSession.Close();
		return false;
	}

	oSession.StartTransaction();
	CTasksTable oTasksTable(oSession);
	
	//delete all tasks associated with the project
	for (int i = 0; i < oProjectDetails.oTasksTypedPtrArray.GetCount(); i++)
	{
		if (!oTasksTable.DeleteWhereID(oProjectDetails.oTasksTypedPtrArray.GetAt(i)->lId))
		{
			oSession.Abort();
			oSession.Close();
			return false;
		}
	}

	CProjectsTable oProjetsTable(oSession);
	if (!oProjetsTable.DeleteWhereID(oProjectDetails.recProject.lId))
	{
		oSession.Abort();
		oSession.Close();
		return false;
	}

	oSession.Commit();
	oSession.Close();
	return true;
};