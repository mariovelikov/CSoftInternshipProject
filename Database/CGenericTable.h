#pragma once

#include "pch.h"
#include "STRUCTURES.h"
#include <comdef.h>
#include "DataSourceConnection.h"
#include "CProjectsAccessor.h"
#include "CTasksAccessor.h"
#include "CUsersAccessor.h"

#define DATA_ACCESSOR_INDEX 1

template <typename T>
class CGenericTable
{
public:
	CGenericTable(CString strTableName, CSession* oSession = nullptr) : m_strTableName(strTableName), m_oSession(*oSession){};
	bool SelectAll(CStructTypedPtrArray<T>& oRecordsArray)
	{
		// Create SQL query to select all users
		CString strQuery;
		strQuery.Format(_T("SELECT * FROM %s WITH(NOLOCK)"), m_strTableName);

		HRESULT hResult = m_oCommand.Open(m_oSession, strQuery);

		if (FAILED(hResult))
		{
			PrintError(hResult, _T("SQL query failed"));

			m_oCommand.Close();
			return false;
		}

		// Read the data
		while ((hResult = m_oCommand.MoveNext()) == S_OK)
		{
			T* pRecord = new T(m_oCommand.GetRec());
			oRecordsArray.Add(pRecord);
		}

		if (hResult != DB_S_ENDOFROWSET) {
			m_oCommand.Close();
			return false;
		}

		m_oCommand.Close();
		return true;
	};

	bool SelectWhereID(const long lID, T& oRecord)
	{
		// Construct the query
		CString strQuery;
		strQuery.Format(_T("SELECT * FROM %s WITH(NOLOCK) WHERE ID = %ld"), m_strTableName,  lID);
		HRESULT hResult = m_oCommand.Open(m_oSession, strQuery);

		if (FAILED(hResult))
		{
			PrintError(hResult, _T("SQL Query failed"));

			return false;
		}

		if (m_oCommand.MoveNext() == S_OK)
		{
			oRecord = m_oCommand.GetRec();
			m_oCommand.Close();
			return true;
		}

		m_oCommand.Close();
		return false;
	};
	
	bool UpdateWhereID(const long lID, T& oRecord)
	{
		CString strQuery;
		strQuery.Format(_T("SELECT * FROM %s WITH(UPDLOCK) WHERE ID = %ld"),m_strTableName, lID);

		CDBPropSet oUpdateDBPropSet = GetDBPropSet();

		HRESULT hResult = m_oCommand.Open(m_oSession, strQuery, &oUpdateDBPropSet);
		if (FAILED(hResult)) {
			PrintError(hResult, _T("Open command failed"));
			m_oCommand.Close();
			return false;
		}

		hResult = m_oCommand.MoveFirst();
		if (FAILED(hResult))
		{
			PrintError(hResult, _T("Error reading data"));
			m_oCommand.Close();
			return false;
		}

		if (oRecord.lUpdateCounter != m_oCommand.GetRec().lUpdateCounter) {
			PrintError(hResult, _T("Error record update counter changed id"));

			m_oCommand.Close();
			return false;
		}

		T& recCurrentUser = m_oCommand.GetRec();
		oRecord.lUpdateCounter += 1;
		recCurrentUser = oRecord;

		hResult = m_oCommand.SetData(DATA_ACCESSOR_INDEX);

		if (FAILED(hResult)) {
			PrintError(hResult, _T("Command open failed"));
			m_oCommand.Close();
			return false;
		}
		
		m_oCommand.Close();
		return true;
	};

	bool Insert(T& oRecord)
	{
		// Prepare the command to insert a new user
		CDBPropSet oInsertDBPropSet = GetDBPropSet();

		CString strQuery;
		strQuery.Format(_T("SELECT * FROM %s WHERE 1 <> 1"), m_strTableName);

		HRESULT hResult = m_oCommand.Open(m_oSession, strQuery, &oInsertDBPropSet);
		if (FAILED(hResult))
		{
			_com_error err(hResult);
			PrintError(hResult, _T("Command open failed"));

			m_oCommand.Close();
			return false;
		}

		T tempRecUser = oRecord;
		m_oCommand.SetRec(tempRecUser);

		hResult = m_oCommand.Insert(DATA_ACCESSOR_INDEX);
		if (FAILED(hResult))
		{
			PrintError(hResult, _T("Insert failed"));
			m_oCommand.Close();
			return false;
		}

		hResult = m_oCommand.MoveFirst();
		if (FAILED(hResult))
		{
			PrintError(hResult, _T("Update failed"));

			m_oCommand.Close();
			return false;
		}

		oRecord.lId = m_oCommand.GetRec().lId;
		m_oCommand.Close();
		return true;
	};

	bool DeleteWhereID(const long lID)
	{
		CString strQuery;
		strQuery.Format(_T("SELECT * FROM %s WHERE ID = %ld"), m_strTableName, lID);

		CDBPropSet oDeleteDBPropSet = GetDBPropSet();

		HRESULT hResult = m_oCommand.Open(m_oSession, strQuery, &oDeleteDBPropSet);
		if (FAILED(hResult)) {
			PrintError(hResult, _T("Transaction failed"));
			m_oCommand.Close();

			return false;
		}

		hResult = m_oCommand.MoveFirst();
		if (hResult != S_OK) {
			PrintError(hResult, _T("Error reading data or no User with this ID"));

			m_oCommand.Close();
			return false;
		}

		hResult = m_oCommand.Delete();
		if (FAILED(hResult)) {
			PrintError(hResult, _T("Delete failed"));

			m_oCommand.Close();
			return false;
		}
		
		m_oCommand.Close();
		return true;
	};


private:
	void PrintError(HRESULT hResult, LPCTSTR lpszMessage)
	{
		_com_error err(hResult);
		TRACE(_T("\n \n \n%s, error message: %s\n \n \n"), lpszMessage, err.ErrorMessage());
	}

	CDBPropSet GetDBPropSet()
	{
		CDBPropSet oPropSet(DBPROPSET_ROWSET);
		oPropSet.AddProperty(DBPROP_CANFETCHBACKWARDS, true);
		oPropSet.AddProperty(DBPROP_IRowsetScroll, true);
		oPropSet.AddProperty(DBPROP_IRowsetChange, true);
		oPropSet.AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE);

		return oPropSet;
	}

private:
	CString m_strTableName;
	CSession& m_oSession;
	/// <summary> Execute SQL and Hadle results</summary>
	CCommand<CAccessor<CGenericAccessor<T>>> m_oCommand;
};