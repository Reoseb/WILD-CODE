// TaskSchedulerInterface.h: interface for the CTaskSchedulerInterface class.
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#ifndef _TASKSCHEDULERINTERFACE_H_INCLUDED_
#define _TASKSCHEDULERINTERFACE_H_INCLUDED_
//////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//////////////////////////////////////////////////////////////////////
#define TSI_OK											0
#define TSI_ERR_NO_TASKS								1
#define TSI_ERR_TASKS_DOESNT_EXIST						2
#define TSI_ERR_COINITIALIZEEX_FAILED					-1
#define TSI_ERR_COINITIALIZESECURITY_FAILED				-2
#define TSI_ERR_ITASKSERVICE_CREATE_FAILED				-3
#define TSI_ERR_ITASKSERVICE_CONNECT_FAILED				-4
#define TSI_ERR_ITASKSERVICE_GETFOLDER_FAILED			-5
#define TSI_ERR_ITASKDEFINITION_CREATE_FAILED			-6
#define TSI_ERR_CANOT_GET_IREGISTRATION_INFO			-7
#define TSI_ERR_CANOT_SET_IREGISTRATION_INFO			-8
#define TSI_ERR_CANNOT_GET_ITASKSETTINGS				-9
#define TSI_ERR_CANNOT_SET_ITASKSETTINGS				-10
#define TSI_ERR_CANNOT_GET_ITRIGGERCOLLECTION			-11
#define TSI_ERR_ITRIGGER_CREATE_FAILED					-12
#define TSI_ERR_IBOOTTRIGGER_QUERYINTERFACE_FAILED		-13
#define TSI_ERR_IBOOTTRIGGER_PUT_DELAY_FAILED			-14
#define TSI_ERR_CANNOT_GET_IPRINCIPAL					-15
#define TSI_ERR_IPRINCIPAL_PUT_RUNLEVEL_FAILED			-16
#define TSI_ERR_CANNOT_GET_IACTIONCOLLECTION			-17
#define TSI_ERR_IACTION_CREATE_FAILED					-18
#define TSI_ERR_IEXECACTION_QUERYINTERFACE_FAILED		-19
#define TSI_ERR_IEXECACTION_PUT_PATH_FAILED				-20
#define TSI_ERR_IEXECACTION_PUT_ARGUMENTS_FAILED		-21
#define TSI_ERR_REGISTERTASKDEFINITION_FAILED			-22
#define TSI_ERR_IREGISTEREDTASKCOLLECTION_CREATE_FAILED	-23
#define TSI_ERR_CANNOT_GET_IIDLESETTINGS				-24
#define TSI_ERR_CANNOT_SET_IIDLESETTINGS				-25
//////////////////////////////////////////////////////////////////////
#include <taskschd.h>
//////////////////////////////////////////////////////////////////////
class CTaskSchedulerInterface  
{
public:
	int	Init();
	void Uninit();
	int	AddTask(LPCSTR lpTask, LPCSTR lpAuthor, LPCSTR lpPath, LPCSTR lpArg);
	int	DeleteTask(LPCSTR lpTask);
	int	IsTaskExist(LPCSTR lpTask);

	CTaskSchedulerInterface();
	virtual ~CTaskSchedulerInterface();
};
//////////////////////////////////////////////////////////////////////
#endif
//////////////////////////////////////////////////////////////////////
