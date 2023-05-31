// TaskSchedulerInterface.cpp: implementation of the CTaskSchedulerInterface class.
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "TaskSchedulerInterface.h"
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CTaskSchedulerInterface::CTaskSchedulerInterface()
{
}
//////////////////////////////////////////////////////////////////////
CTaskSchedulerInterface::~CTaskSchedulerInterface()
{
}
//////////////////////////////////////////////////////////////////////
int CTaskSchedulerInterface::Init()
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr))
		return TSI_ERR_COINITIALIZEEX_FAILED;

	//  Set general COM security levels.
	hr = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_PKT_PRIVACY, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, 0, NULL);
	if (FAILED(hr))
	{
		CoUninitialize();
		return TSI_ERR_COINITIALIZESECURITY_FAILED;
	}

	return TSI_OK;
}
//////////////////////////////////////////////////////////////////////
void CTaskSchedulerInterface::Uninit()
{
	CoUninitialize();
}
//////////////////////////////////////////////////////////////////////
int CTaskSchedulerInterface::AddTask(LPCSTR lpTask, LPCSTR lpAuthor, LPCSTR lpPath, LPCSTR lpArg)
{
	WCHAR wszTask[MAX_PATH]; 
	WCHAR wszPath[MAX_PATH];
	WCHAR wszArg[MAX_PATH];
	WCHAR wszAuthor[MAX_PATH];
 
	MultiByteToWideChar(CP_ACP, 0, lpTask, -1, wszTask, MAX_PATH); 
	MultiByteToWideChar(CP_ACP, 0, lpPath, -1, wszPath, MAX_PATH); 
	MultiByteToWideChar(CP_ACP, 0, lpArg, -1, wszArg, MAX_PATH); 
	MultiByteToWideChar(CP_ACP, 0, lpAuthor, -1, wszAuthor, MAX_PATH); 

    //  Create an instance of the Task Service. 
    ITaskService *pService = NULL;
    HRESULT hr = CoCreateInstance(CLSID_TaskScheduler, NULL, CLSCTX_INPROC_SERVER, IID_ITaskService, (void**)&pService);  
    if (FAILED(hr))
    {
		return TSI_ERR_ITASKSERVICE_CREATE_FAILED;
    }
        
    //  Connect to the task service.
    hr = pService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
    if (FAILED(hr))
    {
        pService->Release();
        return TSI_ERR_ITASKSERVICE_CONNECT_FAILED;
    }

    //  Get the pointer to the root task folder.  
    //  This folder will hold the new task that is registered.
    ITaskFolder *pRootFolder = NULL;
    hr = pService->GetFolder(_bstr_t(L"\\") , &pRootFolder);
    if (FAILED(hr))
    {
        pService->Release();
        return TSI_ERR_ITASKSERVICE_GETFOLDER_FAILED;
    }
    
    //  If the same task exists, remove it.
    pRootFolder->DeleteTask(_bstr_t(wszTask), 0);
    
    //  Create the task builder object to create the task.
    ITaskDefinition *pTask = NULL;
    hr = pService->NewTask(0, &pTask);

	// COM clean up.  Pointer is no longer used.
    pService->Release(); 

    if (FAILED(hr))
    {
		pRootFolder->Release();
		return TSI_ERR_ITASKDEFINITION_CREATE_FAILED;
	}
 
    //  Get the registration info for setting the identification.
    IRegistrationInfo *pRegInfo= NULL;
    hr = pTask->get_RegistrationInfo(&pRegInfo);
    if (FAILED(hr))
    {
        pRootFolder->Release();
        pTask->Release();
        return TSI_ERR_CANOT_GET_IREGISTRATION_INFO;
    }
    
    hr = pRegInfo->put_Author(wszAuthor);
    pRegInfo->Release();
    if (FAILED(hr))
    {
        pRootFolder->Release();
        pTask->Release();
        return TSI_ERR_CANOT_SET_IREGISTRATION_INFO;
    }

    //  Create the settings for the task
    ITaskSettings *pSettings = NULL;
    hr = pTask->get_Settings(&pSettings);
    if (FAILED(hr))
    {
        pRootFolder->Release();
        pTask->Release();
        return TSI_ERR_CANNOT_GET_ITASKSETTINGS;
    }
    
    //  Set setting values for the task. 

    hr = pSettings->put_StartWhenAvailable(VARIANT_BOOL(true));
    if (FAILED(hr))
    {
	    pSettings->Release();
        pRootFolder->Release();
        pTask->Release();
        return TSI_ERR_CANNOT_SET_ITASKSETTINGS;
    }   

	//Disable execution time limit

	hr = pSettings->put_ExecutionTimeLimit(L"PT0S");
    if (FAILED(hr))
    {
	    pSettings->Release();
        pRootFolder->Release();
        pTask->Release();
        return TSI_ERR_CANNOT_SET_ITASKSETTINGS;
    }   

	//Allow start if on batteries

	hr = pSettings->put_DisallowStartIfOnBatteries(VARIANT_BOOL(false));
    if (FAILED(hr))
    {
	    pSettings->Release();
        pRootFolder->Release();
        pTask->Release();
        return TSI_ERR_CANNOT_SET_ITASKSETTINGS;
    }   

	//Disallow stop if going on batteries

	hr = pSettings->put_StopIfGoingOnBatteries(VARIANT_BOOL(false));
    if (FAILED(hr))
    {
	    pSettings->Release();
        pRootFolder->Release();
        pTask->Release();
        return TSI_ERR_CANNOT_SET_ITASKSETTINGS;
    }   

	//Create idle settings

	IIdleSettings *pIdleSettings = NULL;
    hr = pSettings->get_IdleSettings(&pIdleSettings);
    if (FAILED(hr))
    {
		pSettings->Release();
        pRootFolder->Release();
        pTask->Release();
        return TSI_ERR_CANNOT_GET_IIDLESETTINGS;
    }

	//Disallow stop on idle end

	hr = pIdleSettings->put_StopOnIdleEnd(VARIANT_BOOL(false));
    if (FAILED(hr))
    {
		pIdleSettings->Release();
	    pSettings->Release();
        pRootFolder->Release();
        pTask->Release();
        return TSI_ERR_CANNOT_SET_IIDLESETTINGS;
	} 

	pIdleSettings->Release();

	pSettings->Release();

    //  Get the trigger collection to insert the boot trigger.
    ITriggerCollection *pTriggerCollection = NULL;
    hr = pTask->get_Triggers(&pTriggerCollection);
    if (FAILED(hr))
    {
        pRootFolder->Release();
        pTask->Release();
        return TSI_ERR_CANNOT_GET_ITRIGGERCOLLECTION;
    }

    //  Add the log on trigger to the task.
    ITrigger *pTrigger = NULL;
    hr = pTriggerCollection->Create(TASK_TRIGGER_LOGON, &pTrigger); 
    pTriggerCollection->Release();
    if (FAILED(hr))
    {
        pRootFolder->Release();
        pTask->Release();
        return TSI_ERR_ITRIGGER_CREATE_FAILED;
    }

    pTrigger->Release();

	IPrincipal* pPrincipal = NULL;

    hr = pTask->get_Principal(&pPrincipal);
    if (FAILED(hr))
    {
        pRootFolder->Release();
        pTask->Release();
        return TSI_ERR_CANNOT_GET_IPRINCIPAL;
    }

    hr = pPrincipal->put_RunLevel(TASK_RUNLEVEL_HIGHEST);
	pPrincipal->Release();
    if (FAILED(hr))
    {
        pRootFolder->Release();
        pTask->Release();
        return TSI_ERR_IPRINCIPAL_PUT_RUNLEVEL_FAILED;
    }

    //  Add an Action to the task. This task will execute Notepad.exe.     
    IActionCollection *pActionCollection = NULL;

    //  Get the task action collection pointer.
    hr = pTask->get_Actions( &pActionCollection );
    if (FAILED(hr))
    {
        pRootFolder->Release();
        pTask->Release();
        return TSI_ERR_CANNOT_GET_IACTIONCOLLECTION;
    }
        
    //  Create the action, specifying it as an executable action.
    IAction *pAction = NULL;
    hr = pActionCollection->Create(TASK_ACTION_EXEC, &pAction);
    pActionCollection->Release();
    if (FAILED(hr))
    {
        pRootFolder->Release();
        pTask->Release();
        return TSI_ERR_IACTION_CREATE_FAILED;
    }

    IExecAction *pExecAction = NULL;
    hr = pAction->QueryInterface(IID_IExecAction, (void**)&pExecAction);
    pAction->Release();
    if (FAILED(hr))
    {
        pRootFolder->Release();
        pTask->Release();
        return TSI_ERR_IEXECACTION_QUERYINTERFACE_FAILED;
    }

    //  Set the path of the executable
    hr = pExecAction->put_Path(_bstr_t(wszPath)); 
    if (FAILED(hr))
    {
	    pExecAction->Release(); 
        pRootFolder->Release();
        pTask->Release();
        return TSI_ERR_IEXECACTION_PUT_PATH_FAILED;
    }  

	//  Set arguments
	hr = pExecAction->put_Arguments(_bstr_t(wszArg)); 
    if (FAILED(hr))
    {
	    pExecAction->Release(); 
        pRootFolder->Release();
        pTask->Release();
        return TSI_ERR_IEXECACTION_PUT_ARGUMENTS_FAILED;
    }  

    pExecAction->Release(); 

    //  Save the task in the root folder.
    IRegisteredTask *pRegisteredTask = NULL;

	VARIANT varPassword;
    varPassword.vt = VT_EMPTY;
    hr = pRootFolder->RegisterTaskDefinition(_bstr_t(wszTask), pTask, TASK_CREATE_OR_UPDATE, _variant_t(L""), varPassword, TASK_LOGON_INTERACTIVE_TOKEN, _variant_t(L""), &pRegisteredTask);
    if( FAILED(hr) )
    {
        pRootFolder->Release();
        pTask->Release();
        return TSI_ERR_REGISTERTASKDEFINITION_FAILED;
    }
    
    pRootFolder->Release();
    pTask->Release();
    pRegisteredTask->Release();
    return TSI_OK;
}
//////////////////////////////////////////////////////////////////////
int CTaskSchedulerInterface::DeleteTask(LPCSTR lpTask)
{
	WCHAR wszTask[MAX_PATH]; 
 	MultiByteToWideChar(CP_ACP, 0, lpTask, -1, wszTask, MAX_PATH); 

    //  Create an instance of the Task Service. 
    ITaskService *pService = NULL;
    HRESULT hr = CoCreateInstance(CLSID_TaskScheduler, NULL, CLSCTX_INPROC_SERVER, IID_ITaskService, (void**)&pService);  
    if (FAILED(hr))
    {
		return TSI_ERR_ITASKSERVICE_CREATE_FAILED;
    }
        
    //  Connect to the task service.
    hr = pService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
    if (FAILED(hr))
    {
        pService->Release();
        return TSI_ERR_ITASKSERVICE_CONNECT_FAILED;
    }

    //  Get the pointer to the root task folder.  
    //  This folder will hold the new task that is registered.
    ITaskFolder *pRootFolder = NULL;
    hr = pService->GetFolder(_bstr_t(L"\\") , &pRootFolder);
    if (FAILED(hr))
    {
        pService->Release();
        return TSI_ERR_ITASKSERVICE_GETFOLDER_FAILED;
    }
    
    //  If the same task exists, remove it.
    pRootFolder->DeleteTask(_bstr_t(wszTask), 0);

    pService->Release(); 
    pRootFolder->Release();
    return TSI_OK;
}
//////////////////////////////////////////////////////////////////////
int CTaskSchedulerInterface::IsTaskExist(LPCSTR lpTask)
{
	WCHAR wszTask[MAX_PATH]; 
 	MultiByteToWideChar(CP_ACP, 0, lpTask, -1, wszTask, MAX_PATH); 

	//  Create an instance of the Task Service. 
	ITaskService *pService = NULL;
	HRESULT hr = CoCreateInstance(CLSID_TaskScheduler, NULL, CLSCTX_INPROC_SERVER, IID_ITaskService, (void**)&pService);  
	if (FAILED(hr))
	{
		return TSI_ERR_ITASKSERVICE_CREATE_FAILED;
	}

        
    //  Connect to the task service.
    hr = pService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
    if (FAILED(hr))
    {
        pService->Release();
        return TSI_ERR_ITASKSERVICE_CONNECT_FAILED;
    }

    //  Get the pointer to the root task folder.  
    //  This folder will hold the new task that is registered.
    ITaskFolder *pRootFolder = NULL;
    hr = pService->GetFolder(_bstr_t(L"\\") , &pRootFolder);
    if (FAILED(hr))
    {
        pService->Release();
        return TSI_ERR_ITASKSERVICE_GETFOLDER_FAILED;
    }

	// COM clean up.  Pointer is no longer used.
    pService->Release(); 

    //  Get the registered tasks in the folder.
    IRegisteredTaskCollection* pTaskCollection = NULL;
    hr = pRootFolder->GetTasks(NULL, &pTaskCollection);

	// COM clean up.  Pointer is no longer used.
    pRootFolder->Release();

    if (FAILED(hr))
    {
        return TSI_ERR_IREGISTEREDTASKCOLLECTION_CREATE_FAILED;
    }

	LONG numTasks = 0;
    hr = pTaskCollection->get_Count(&numTasks);

    if (!numTasks)
     {
        pTaskCollection->Release();
        return TSI_ERR_NO_TASKS;
     }

	for(int i=0; i<numTasks; i++)
    {
        IRegisteredTask* pRegisteredTask = NULL;
        hr = pTaskCollection->get_Item(_variant_t(i+1), &pRegisteredTask);
        
        if (SUCCEEDED(hr))
        {
            BSTR taskName = NULL;
            hr = pRegisteredTask->get_Name(&taskName);

            if( SUCCEEDED(hr) )
            {
				if (!wcscmp(wszTask, taskName))
				{
	                SysFreeString(taskName);

		            pRegisteredTask->Release();
					pTaskCollection->Release();
					return TSI_OK;
				}

                SysFreeString(taskName);
            }

            pRegisteredTask->Release();
        }
    }

	pTaskCollection->Release();
    return TSI_ERR_TASKS_DOESNT_EXIST;
}
//////////////////////////////////////////////////////////////////////