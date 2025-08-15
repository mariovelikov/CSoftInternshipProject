#pragma once
#include <tchar.h>


#define NAME_LENGTH 64
#define EMAIL_LENGTH 128
#define JOBS_TITLE_LENGTH 128

enum SprxTasksState
{
	SprxPending = 1,
	SprxInProgress = 2,
	SprxEnded = 3
};

enum SprxProjectsState
{
	SprxActive = 1,
	SprxFinished = 2
};

/// <summary> Struct Users </summary>
struct USERS
{
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

	/// <summary> Constructor </summary>
	USERS()
	{
		SecureZeroMemory(this, sizeof(*this));
	}
};


#define PROJECTS_NAME_LENGTH 64
#define PROJECTS_DESCRIPTION_LENGTH 256
#define PROJECTS_TOTAL_EFFORT_DEFAULT 0

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

	/// <summary> A variable that stores state of project. </summary>
	short sState = SprxActive;

	/// <summary>A variable that stores total effort for all project. </summary>
	long lTotalEffort = PROJECTS_TOTAL_EFFORT_DEFAULT;

	/// <summary> Constructor </summary>
	PROJECTS()
	{
		SecureZeroMemory(this, sizeof(*this));
	}
};

#define TASKS_NAME_LENGTH 64
#define TASKS_DESCRIPTION_LENGTH 256

/// <summary> Struct Tasks </summary>
struct TASKS 
{
	/// <summary> A variable that stores a task identifier as a long integer. </summary>
	long lId;

	/// <summary> A variable that stores the update counter. </summary>
	long lUpdateCounter;

	/// <summary> Declares a character array for storing a name. </summary>
	TCHAR szName[TASKS_NAME_LENGTH];

	/// <summary> A character array for storing a description string. </summary>
	TCHAR szDescription[TASKS_DESCRIPTION_LENGTH];

	/// <summary> A variable that stores a project identifier as a long integer. </summary>
	long lProjectId;

	/// <summary> Declares a variable to store a user ID as a long integer. </summary>
	long lUserId;

	/// <summary> Declares and initializes an integer variable representing the state of a project. </summary>
	short sState = SprxPending;

	/// <summary> Stores the total effort value as int. </summary>
	long lEffort;

	/// <summary> Constructor </summary>
	TASKS()
	{
		SecureZeroMemory(this, sizeof(*this));
	}
};

/// <summary> Represents a view item containing project information and the project manager's name. </summary>
struct PROJECTS_VIEW_ITEM
{
	/// <summary> A variable that stores a project record. </summary>
	PROJECTS recProject;

	/// <summary> Project manager's name associated with the project. </summary>
	TCHAR szProjectManagerName[NAME_LENGTH];

	PROJECTS_VIEW_ITEM()
	{
		SecureZeroMemory(this, sizeof(*this));
	}
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

class CUsersMap: public CMap<int, int, USERS*, USERS*&>
{
public:
	CUsersMap() {}

	~CUsersMap()
	{
		ClearMap();
	}

private:
	void ClearMap()
	{
		POSITION pos = GetStartPosition();
		while (pos != nullptr)
		{
			int key;
			USERS* pUser;
			GetNextAssoc(pos, key, pUser);

			delete pUser; 
		}

		RemoveAll();
	}
};


typedef CStructTypedPtrArray<USERS> CUsersTypedPtrArray;
typedef CStructTypedPtrArray<PROJECTS> CProjectsTypedPtrArray;
typedef CStructTypedPtrArray<TASKS> CTasksTypedPtrArray;
typedef CStructTypedPtrArray<PROJECTS_VIEW_ITEM> CProjectsViewItemTypedPtrArray;

struct PROJECT_DETAILS
{
	PROJECTS recProject;
	CTasksTypedPtrArray oTasksTypedPtrArray;
	CArray<long, long> m_oTaskIdsToDelete;
};
