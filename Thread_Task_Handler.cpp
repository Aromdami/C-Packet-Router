/*
#include <Windows.h>
#include "Multi_Thread.h"

unsigned __stdcall Thread_TaskHandler(LPVOID pParam)
{
	ThreadParam_Task *pThrdParam;
	HL_PriTaskQ *pTask_Q;
	int myRole;
	int myAddr;

	THREAD_FLAG *pFlagThreadTerminate;
	int maxRound;

	Task  task;
	int task_gen_count = 0;
	int num_pkt_processed = 0;
	int targetTaskGen;

	ThreadStatusMonitor *pThrdMon;

	pThrdParam = (ThreadParam_Task *)pParam;
	myRole = pThrdParam->role;
	myAddr = pThrdParam->myAddr;
	pTask_Q = pThrdParam->pTask_Q;
	pThrdMon = pThrdParam->pThrdMon;
	maxRound = pThrdParam->maxRound;
	targetTaskGen = pThrdParam->targetTaskGen;

	for (int round = 0; round < maxRound; round++)
	{
		if (*pThrdMon->pFlagThreadTerminate == TERMINATE)
			break;
		task = pTask_Q->deQueue_Task();

		if (task.getTaskTitle() != "")
		{
			EnterCriticalSection(&pThrdMon->cs_thrd_mon);
			task.setTaskHandlerAddr(myAddr);
			pThrdMon->taskProcessed[pThrdMon->totalTaskProcessed] = task;
			
			pThrdMon->numTaskProcessed++;
			pThrdMon->totalTaskProcessed++;

			LeaveCriticalSection(&pThrdMon->cs_thrd_mon);
		}
		Sleep(300 + rand() % 500);
	}
	_endthreadex(THREAD_EXIT_CODE);
	return 0;
}
*/