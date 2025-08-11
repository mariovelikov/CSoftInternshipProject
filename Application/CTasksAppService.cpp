#include "pch.h"
#include "CTasksAppService.h"
#include "CTasksTable.h"


bool CTasksAppService::GetAllTasks(CTasksTypedPtrArray& oTasksTypedPtrArray) const
{
	CDataSourceConnection& oDataSourceConnection = CDataSourceConnection::GetInstance();
	CDataSource oDataSource = oDataSourceConnection.GetDataSource();
	CSession oSession;

	HRESULT hResult = oSession.Open(oDataSource);
	if (FAILED(hResult)) {
		//PrintError(hResult, _T("Open session failed"));

		return false;
	}

	CTasksTable oTasksTable(oSession);

	if (!oTasksTable.SelectAll(oTasksTypedPtrArray))
	{
		oSession.Close();
		return false;
	}
	oSession.Close();
	return true;
};

bool CTasksAppService::AddTask(TASKS& oReccord) const
{
	CDataSourceConnection& oDataSourceConnection = CDataSourceConnection::GetInstance();
	CDataSource oDataSource = oDataSourceConnection.GetDataSource();
	CSession oSession;

	HRESULT hResult = oSession.Open(oDataSource);
	if (FAILED(hResult)) {
		//PrintError(hResult, _T("Open session failed"));

		return false;
	}

	CTasksTable oTasksTable(oSession);
	
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
	CDataSourceConnection& oDataSourceConnection = CDataSourceConnection::GetInstance();
	CDataSource oDataSource = oDataSourceConnection.GetDataSource();
	CSession oSession;

	HRESULT hResult = oSession.Open(oDataSource);
	if (FAILED(hResult)) {
		//PrintError(hResult, _T("Open session failed"));

		return false;
	}

	CTasksTable oTasksTable(oSession);

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
	CDataSourceConnection& oDataSourceConnection = CDataSourceConnection::GetInstance();
	CDataSource oDataSource = oDataSourceConnection.GetDataSource();
	CSession oSession;

	HRESULT hResult = oSession.Open(oDataSource);
	if (FAILED(hResult)) {
		//PrintError(hResult, _T("Open session failed"));

		return false;
	}

	CTasksTable oTasksTable(oSession);

	if (!oTasksTable.DeleteWhereID(lID))
	{
		oSession.Close();
		return false;
	}
	oSession.Close();
	return true;
};