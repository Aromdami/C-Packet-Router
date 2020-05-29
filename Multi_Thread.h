#ifndef MULTI_THREAD_H
#define MULTI_THREAD_H
#include <Windows.h>
#include <process.h>
#include <thread>

#include "SimParam.h"
/*
#define THREAD_EXIT_CODE 0

enum ROLE { TASK_GENERATOR, TASK_HANDLER };
enum THREAD_FLAG { INITIALIZE, RUN, TERMINATE };


typedef struct ThreadStatusMonitor
{
	CRITICAL_SECTION cs_thrd_mon;

	int numTaskGenerated;
	int numTaskProcessed;

	int totalTaskGenerated;
	int totalTaskProcessed;

	Task taskGenerated[TOTAL_NUM_TASKS];
	Task taskProcessed[TOTAL_NUM_TASKS]; 
	THREAD_FLAG *pFlagThreadTerminate;
}ThreadStatusMonitor;

typedef struct ThreadParam
{
	CRITICAL_SECTION *pCS_main;
	CRITICAL_SECTION *pCS_thrd_mon;

	HL_PriTaskQ *pTask_Q;
	ROLE role;

	int myAddr;
	int maxRound;
	int targetTaskGen;
	int num_tasks;

	ThreadStatusMonitor *pThrdMon;
}ThreadParam_Task;

unsigned __stdcall Thread_TaskHandler(LPVOID pParam);
unsigned __stdcall Thread_TaskGenerator(LPVOID pParam);
*/
#endif