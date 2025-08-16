#include "pch.h"
#include "CTasksAppService.h"
#include "CTasksTable.h"


bool CTasksAppService::GetAllTasks(CTasksTypedPtrArray& oTasksTypedPtrArray) const
{
	CDataSource oDataSource = CDataSourceConnection::GetInstance().GetDataSource();
	CSession* pSession = new CSession();

	HRESULT hResult = pSession->Open(oDataSource);
	if (FAILED(hResult)) {
		return false;
	}

	CTasksTable oTasksTable(pSession);

	if (!oTasksTable.SelectAll(oTasksTypedPtrArray))
	{
		pSession->Close();
		return false;
	}
	pSession->Close();
	return true;
};

bool CTasksAppService::AddTask(TASKS& oReccord) const
{
	CDataSource oDataSource = CDataSourceConnection::GetInstance().GetDataSource();
	CSession* pSession = new CSession();

	HRESULT hResult = pSession->Open(oDataSource);
	if (FAILED(hResult)) {
		return false;
	}

	CTasksTable oTasksTable(pSession);
	
	if (!oTasksTable.Insert(oReccord))
	{
		pSession->Close();
		return false;
	}
	pSession->Close();
	return true;
};

bool CTasksAppService::UpdateTask(TASKS& oRecord) const
{
	CDataSource oDataSource = CDataSourceConnection::GetInstance().GetDataSource();
	CSession* pSession = new CSession();

	HRESULT hResult = pSession->Open(oDataSource);
	if (FAILED(hResult)) {
		return false;
	}

	CTasksTable oTasksTable(pSession);

	if (!oTasksTable.UpdateWhereID(oRecord.lId, oRecord))
	{
		pSession->Close();
		return false;
	}
	pSession->Close();
	return true;
};

bool CTasksAppService::DeleteTask(const long lID) const
{
	CDataSource oDataSource = CDataSourceConnection::GetInstance().GetDataSource();
	CSession* pSession = new CSession();

	HRESULT hResult = pSession->Open(oDataSource);
	if (FAILED(hResult)) {
		return false;
	}

	CTasksTable oTasksTable(pSession);

	if (!oTasksTable.DeleteWhereID(lID))
	{
		pSession->Close();
		return false;
	}
	pSession->Close();
	return true;
};