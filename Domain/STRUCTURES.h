#pragma once
#include <tchar.h>


#define NAME_LENGTH 64
#define EMAIL_LENGTH 128
#define JOBS_TITLE_LENGTH 128

enum StateEnum
{
	ProjectPending = 1,
	ProjectActive = 2,
	ProjectOnHold = 3,
	ProjectClosedComplete = 4,
	ProjectClosedCancel = 5
};

/// <summary> Struct Users </summary>
struct USERS
{

	// Members
	//-------------

	/// <summary> Unique Id of user </summary>
	long lId;

	/// <summary> Update counter of User, increase + 1 when we make changes on User </summary>
	long lUpdateCounter;
	
	/// <summary> Name of User </summary>
	TCHAR szName[NAME_LENGTH];

	/// <summary> Email of User </summary>
	TCHAR szEmail[EMAIL_LENGTH];

	/// <summary> Job title of User </summary>
	TCHAR szJobTitle[JOBS_TITLE_LENGTH];

	// Constructor 
	// ----------------
	/// <summary> Constructor </summary>
	USERS()
	{
		SecureZeroMemory(this, sizeof(*this));
	}
};


#define PROJECTS_NAME_LENGTH 64
#define PROJECTS_DESCRIPTION_LENGTH 256

/// <summary> Struct Projects </summary>
struct PROJECTS 
{

	/// <summary> A variable that stores a project identifier as a long integer. </summary>
	long lId;

	/// <summary> A variable that stores the update counter value. </summary>
	long lUpdateCounter;

	/// <summary> Declares a character array to store a name. </summary>
	TCHAR szName[PROJECTS_NAME_LENGTH];

	/// <summary> Declares a character array to store a project description. </summary>
	TCHAR szDescription[PROJECTS_DESCRIPTION_LENGTH];

	/// <summary> A variable that stores the project manager's identifier.</summary>
	long lProjectManagerId;

	// Constructor 
	// ----------------
	/// <summary> Constructor </summary>
	PROJECTS()
	{
		SecureZeroMemory(this, sizeof(*this));
	}
};


/// <summary> Struct Tasks </summary>
struct TASKS 
{
	/// <summary> A variable that stores a task identifier as a long integer. </summary>
	long lId;

	/// <summary> A variable that stores the update counter. </summary>
	long lUpdateCounter;

	/// <summary> Declares a character array for storing a name. </summary>
	TCHAR szName[64];

	/// <summary> A character array for storing a description string. </summary>
	TCHAR szDescription[256];

	/// <summary> A variable that stores a project identifier as a long integer. </summary>
	long lProjectId;

	/// <summary> Declares a variable to store a user ID as a long integer. </summary>
	long lUserId;

	/// <summary> Declares and initializes an integer variable representing the state of a project. </summary>
	int nState = ProjectPending;

	/// <summary> Stores the total effort value as int. </summary>
	int nTotalEffort;

	// Constructor 
	// ----------------
	/// <summary> Constructor </summary>
	TASKS()
	{
		SecureZeroMemory(this, sizeof(*this));
	}
};


struct PROECTS_VIEW_ITEM
{
	PROJECTS recProject;
	TCHAR szProjectManagerName;
};

template <typename T> 
class CStructTypedPtrArray : public CTypedPtrArray<CPtrArray, T*>
{
public:
	// Constructor
	CStructTypedPtrArray() noexcept
	{
	}

	// Destructor
	~CStructTypedPtrArray()
	{
		ClearArray();
	}
private:
	void ClearArray() {
		for (int i = 0; i < this->GetSize(); ++i)
		{
			delete this->GetAt(i);
		}
		this->SetSize(0);
	}
};

typedef CStructTypedPtrArray<USERS> CUsersTypedPtrArray;
typedef CStructTypedPtrArray<PROJECTS> CProjectsTypedPtrArray;
typedef CStructTypedPtrArray<TASKS> CTasksTypedPtrArray;
typedef CStructTypedPtrArray<PROECTS_VIEW_ITEM> CProjectsViewItemsTypedPtrArray;

struct PROJECT_DETAILS
{
	PROJECTS recProject;
	CTasksTypedPtrArray oTasksTypedPtrArray;
};
