#include "pch.h"
#include "CTasksAppService.h"
#include "CTasksTable.h"


bool CTasksAppService::GetAllTasks(CTasksTypedPtrArray& oTasksTypedPtrArray) const
{
	CDataSource oDataSource = CDataSourceConnection::GetInstance().GetDataSource();
	CTasksTable oTasksTable;

	if (!oTasksTable.SelectAll(oTasksTypedPtrArray))
	{
		return false;
	}
	return true;
};

bool CTasksAppService::AddTask(TASKS& oReccord) const
{
	CDataSource oDataSource = CDataSourceConnection::GetInstance().GetDataSource();
	CSession oSession;

	HRESULT hResult = oSession.Open(oDataSource);
	if (FAILED(hResult)) {
		return false;
	}

	CTasksTable oTasksTable(&oSession);
	
	if (!oTasksTable.Insert(oReccord))
	{
		oSession.Close();
		return false;
	}
	oSession.Close();
	return true;
};

bool CTasksAppService::UpdateTask(TASKS& oRecord) const
{
	CDataSource oDataSource = CDataSourceConnection::GetInstance().GetDataSource();
	CSession oSession;

	HRESULT hResult = oSession.Open(oDataSource);
	if (FAILED(hResult)) {
		return false;
	}

	CTasksTable oTasksTable(&oSession);

	if (!oTasksTable.UpdateWhereID(oRecord.lId, oRecord))
	{
		oSession.Close();
		return false;
	}
	oSession.Close();
	return true;
};

bool CTasksAppService::DeleteTask(const long lID) const
{
	CDataSource oDataSource = CDataSourceConnection::GetInstance().GetDataSource();
	CSession oSession;

	HRESULT hResult = oSession.Open(oDataSource);
	if (FAILED(hResult)) {
		return false;
	}

	CTasksTable oTasksTable(&oSession);

	if (!oTasksTable.DeleteWhereID(lID))
	{
		oSession.Close();
		return false;
	}
	oSession.Close();
	return true;
};