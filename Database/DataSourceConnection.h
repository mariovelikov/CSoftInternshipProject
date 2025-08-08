#pragma once
#include "DLLExport.h"
#include <atldbcli.h>
using namespace std;

/// <summary>
/// Represents a singleton connection to a data source.
/// </summary>
class DatabaseDLL_EXP CDataSourceConnection
{
public:
	int nNumberOfConnections = 0;
	static CDataSourceConnection& GetInstance();

	CDataSource& GetDataSource();
	HRESULT CloseDataSource();
protected:
private:
	// Constructor and Destructor
	CDataSourceConnection() {}
	~CDataSourceConnection() {};

	//prevent copying
	CDataSourceConnection(const CDataSourceConnection&) = delete;
	CDataSourceConnection& operator=(const CDataSourceConnection&) = delete;

	// Methods to get the data source and property set
	CDBPropSet GetPropSet();

	CDataSource m_oDataSource;
};