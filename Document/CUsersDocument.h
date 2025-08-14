#pragma once
#include "DLLExport.h"
#include "STRUCTURES.h"

/////////////////////////////////////////////////////////////////////////////
// CUsersDocument
class DocumentDLL_EXP CUsersDocument : public CDocument
{
	// Macros
	// ----------------
	DECLARE_DYNCREATE(CUsersDocument)

	// Constructor / Destructor
	// ----------------
public:
	CUsersDocument() noexcept;
	virtual ~CUsersDocument();

	// Methods
	// ----------------
public:

	CUsersMap& GetAllUsers();
	bool AddUser(USERS& oRecUser);
	bool UpdateUser(USERS& oRecUser);
	bool DeleteUser(const long lId);

	// Overrides
	// ----------------
	BOOL OnNewDocument() override;


	// Members
	// ----------------
private: 
	//CUsersTypedPtrArray m_oUsersArray;
	CUsersMap m_oUsersMap;
};
