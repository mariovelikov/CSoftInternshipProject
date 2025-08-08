#include "pch.h"
#include <mutex>

#include "DataSourceConnection.h"
using namespace std;

CDataSourceConnection& CDataSourceConnection::GetInstance()
{
    static CDataSourceConnection m_oDataSourceInstance;
    return m_oDataSourceInstance;
}

CDataSource& CDataSourceConnection::GetDataSource()
{
    if (m_oDataSource.m_spInit == NULL) 
    {
 	    CDBPropSet oDBPropSet = GetPropSet();
        HRESULT hResult = m_oDataSource.Open(_T("MSOLEDBSQL"), &oDBPropSet);
    
        if (FAILED(hResult)) {
            TRACE(_T("\n \n \n \n Open connection failed %s"), hResult);
        }
    }
    return m_oDataSource;
}

HRESULT CDataSourceConnection::CloseDataSource()
{
    try {
        m_oDataSource.Close();
        return S_OK;
    }
    catch (...) {
        return E_FAIL;
    }
}

CDBPropSet CDataSourceConnection::GetPropSet()
{
    CDBPropSet oDBPropSet(DBPROPSET_DBINIT);
    oDBPropSet.AddProperty(DBPROP_INIT_DATASOURCE, _T("(localdb)\\MSSQLLocalDB"));
    oDBPropSet.AddProperty(DBPROP_INIT_CATALOG, _T("ProjectManagementDB"));
    oDBPropSet.AddProperty(DBPROP_AUTH_PERSIST_SENSITIVE_AUTHINFO, false);
    oDBPropSet.AddProperty(DBPROP_INIT_LCID, 1033L);
    oDBPropSet.AddProperty(DBPROP_INIT_PROMPT, static_cast<short>(DBPROMPT_NOPROMPT));
    oDBPropSet.AddProperty(DBPROP_AUTH_USERID, _T("sa"));
    oDBPropSet.AddProperty(DBPROP_AUTH_PASSWORD, _T("SQLpassword123!"));

    return oDBPropSet;
}

