#include "pch.h"
#include "CProjectsAppService.h"
#include "CProjectsTable.h"
#include "CUsersTable.h"
#include "CTasksTable.h"
#include "STRUCTURES.h"


bool CProjectsAppService::GetAllProjects(CProjectsViewItemTypedPtrArray& oProjectsViewItemArray) const
{
	CDataSource oDataSource = CDataSourceConnection::GetInstance().GetDataSource();
	CSession* pSession = new CSession();

	HRESULT hResult = pSession->Open(oDataSource);
	if (FAILED(hResult)) {
		return false;
	}

	if (FAILED(hResult))
	{
		pSession->Close();
		return false;
	}

	CProjectsTable oProjetsTable(pSession);
	CUsersTable oUsersTable(pSession);

	CUsersTypedPtrArray oUsersTypedPtrArray;
	if (!oUsersTable.SelectAll(oUsersTypedPtrArray))
	{
		pSession->Close();
		return false;
	}

	CProjectsTypedPtrArray oProjectsTypedPtrArray;
	if (!oProjetsTable.SelectAll(oProjectsTypedPtrArray))
	{
		pSession->Close();
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
				_tcscpy_s(pViewItem->szProjectManagerName, _countof(pViewItem->szProjectManagerName), pUser->szName);
				break;
			}
		}
		oProjectsViewItemArray.Add(pViewItem);
	}

	pSession->Close();
	return true;
};

bool CProjectsAppService::GetProjectDetails(PROJECT_DETAILS& oProjectDetails) const
{
	CDataSource oDataSource = CDataSourceConnection::GetInstance().GetDataSource();
	CSession* pSession = new CSession();

	HRESULT hResult = pSession->Open(oDataSource);
	if (FAILED(hResult)) {
		CString errorMessage;
		errorMessage.Format(_T("Open session failed. HRESULT: %d"), hResult);
		TRACE(errorMessage);

		pSession->Close();
		return false;
	}

	CTasksTable oTasksTable(pSession);
	CTasksTypedPtrArray oTasks; 
	if (!oTasksTable.SelectAll(oTasks)) {
		pSession->Close();
		return false;
	}

	for (int i = 0; i < oTasks.GetCount(); i++) 
	{
		if (oTasks.GetAt(i)->lProjectId == oProjectDetails.recProject.lId) 
		{
			TASKS oTask = *oTasks[i];
			TASKS* pTask = new TASKS();
			*pTask = oTask;
			oProjectDetails.oTasksTypedPtrArray.Add(pTask);
		}
	}

	pSession->Close();
	return true;
}

bool CProjectsAppService::AddProject(PROJECTS& oRecord, CTasksTypedPtrArray& oTasksArray) const
{
	CDataSource oDataSource = CDataSourceConnection::GetInstance().GetDataSource();
	CSession* pSession = new CSession();

	HRESULT hResult = pSession->Open(oDataSource);
	if (FAILED(hResult)) {
		CString errorMessage;
		errorMessage.Format(_T("Open session failed. HRESULT: %d"), hResult);
		TRACE(errorMessage);

		pSession->Close();
		return false;
	}

	if (FAILED(pSession->StartTransaction()))
	{
		pSession->Close();
		return false;
	};

	CProjectsTable oProjetsTable(pSession);
	CTasksTable oTasksTable(pSession);

	// FIRST WE ADD PROJECT
	if (!oProjetsTable.Insert(oRecord))
	{
		pSession->Abort();
		pSession->Close();
		return false;
	}

	// LOOP ALL TASKS AND ADD TO PROJECT
	for (int i = 0; i < oTasksArray.GetCount(); i++) {
		oTasksArray[i]->lProjectId = oRecord.lId;
		if (!oTasksTable.Insert(*oTasksArray.GetAt(i)))
		{
			pSession->Abort();
			pSession->Close();
			return false;
		};
	}

	if (FAILED(pSession->Commit()))
	{
		pSession->Close();
		return false;
	};

	pSession->Close();
	return true;
};

bool CProjectsAppService::UpdateProject(PROJECT_DETAILS& oRecord) const // 
{
	CDataSource oDataSource = CDataSourceConnection::GetInstance().GetDataSource();
	CSession* pSession = new CSession();

	HRESULT hResult = pSession->Open(oDataSource);
	if (FAILED(hResult)) {
		pSession->Close();
		return false;
	}

	if (FAILED(pSession->StartTransaction()))
	{
		pSession->Close();
		return false;
	};

	CTasksTable oTasksTable(pSession);

	//Delete tasks
	for (int i = 0; i < oRecord.m_oTaskIdsToDelete.GetCount(); i++)
	{
		if (!oTasksTable.DeleteWhereID(oRecord.m_oTaskIdsToDelete.GetAt(i)))
		{
			pSession->Abort();
			pSession->Close();
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
	short sProjecState = SprxFinished;
	for (int i = 0; i < oRecord.oTasksTypedPtrArray.GetCount(); i++) {
		if (oRecord.oTasksTypedPtrArray.GetAt(i)->sState == SprxActive || oRecord.oTasksTypedPtrArray.GetAt(i)->sState == SprxInProgress)
		{
			sProjecState = SprxActive;
		}

		if (!oTasksTable.UpdateWhereID(oRecord.oTasksTypedPtrArray.GetAt(i)->lId, *oRecord.oTasksTypedPtrArray[i]))
		{
			pSession->Abort();
			pSession->Close();
			return false;
		}
	}

	CProjectsTable oProjetsTable(pSession);
	
	//Update project
	oRecord.recProject.sState = sProjecState;
	if (!oProjetsTable.UpdateWhereID(oRecord.recProject.lId, oRecord.recProject))
	{
		pSession->Abort();
		pSession->Close(); 
		return false;
	}

	if (FAILED(pSession->Commit()))
	{
		pSession->Close();
		return false;
	};

	pSession->Close();
	return true;
};

bool CProjectsAppService::DeleteProject(const PROJECT_DETAILS& oProjectDetails) const
{
	CDataSource oDataSource = CDataSourceConnection::GetInstance().GetDataSource();
	CSession* pSession = new CSession();

	HRESULT hResult = pSession->Open(oDataSource);
	if (FAILED(hResult)) {
		pSession->Close();
		return false;
	}

	if (FAILED(pSession->StartTransaction()))
	{
		pSession->Close();
		return false;
	};

	CTasksTable oTasksTable(pSession);
	
	//delete all tasks associated with the project
	for (int i = 0; i < oProjectDetails.oTasksTypedPtrArray.GetCount(); i++)
	{
		if (!oTasksTable.DeleteWhereID(oProjectDetails.oTasksTypedPtrArray.GetAt(i)->lId))
		{
			pSession->Abort();
			pSession->Close();
			return false;
		}
	}

	CProjectsTable oProjetsTable(pSession);
	if (!oProjetsTable.DeleteWhereID(oProjectDetails.recProject.lId))
	{
		pSession->Abort();
		pSession->Close();
		return false;
	}

	if (FAILED(pSession->Commit()))
	{
		pSession->Close();
		return false;
	};

	pSession->Close();
	return true;
};