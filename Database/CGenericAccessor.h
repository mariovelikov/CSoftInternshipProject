#pragma once
#include "DLLExport.h"
#include "pch.h"  
#include <atldbcli.h>

/// <summary>
/// A template class that provides generic access functionality for a specified type.
/// </summary>
/// <typeparam name="T">The type for which the accessor provides access.</typeparam>
template<typename T>
class CGenericAccessor
{	
public:
	T& GetRec() { return m_rec; }
	void SetRec(const T& rec) { m_rec = rec; }

protected:
	T m_rec;
};
