/*
#include <Windows.h>
#include "Multi_Thread.h"

#include <string>


unsigned __stdcall Thread_TaskGenerator(LPVOID pParam)
{
	ThreadParam_Task *pThrdParam;
	HL_PriTaskQ *pTask_Q;
	int myRole;

	int maxRound;

	Task new_task;
	string task_title;
	char tempStr[16];	
	int task_id = 0;
	int task_priority = 0;
	int task_gen_count = 0;
	int target_task_gen;
	int myAddr = -1;


	ThreadStatusMonitor *pThrdMon;

	pThrdParam = (ThreadParam_Task *)pParam;
	myRole = pThrdParam->role;
	myAddr = pThrdParam->myAddr;
	pTask_Q = pThrdParam->pTask_Q;
	pThrdMon = pThrdParam->pThrdMon;
	maxRound = pThrdParam->maxRound;
	target_task_gen = pThrdParam->targetTaskGen;

	for (int round = 0; round < maxRound; round++)
	{
		if (task_gen_count >= target_task_gen)
		{
			if (*pThrdMon->pFlagThreadTerminate == TERMINATE)
				break;
			else
			{
				Sleep(800);
				continue;
			}
		}

		task_id = task_gen_count;
		sprintf_s(tempStr, "%02d", task_id);
		task_title = "task_" + string(tempStr);

		task_priority = rand()% MAX_TASK_PRIORITY;

		new_task.setTaskTitle(task_title);	
		new_task.setTaskGenAddr(myAddr);
		new_task.setTaskPriority(task_priority);
		new_task.setTaskHandlerAddr(-1);
		
	
		pTask_Q->enQueue_Task(new_task);
		EnterCriticalSection(&pThrdMon->cs_thrd_mon);
		pThrdMon->taskGenerated[pThrdMon->totalTaskGenerated] = new_task;
		pThrdMon->numTaskGenerated++;
		pThrdMon->totalTaskGenerated++;
		LeaveCriticalSection(&pThrdMon->cs_thrd_mon);

		task_gen_count++;
		Sleep(10);
	}
	_endthreadex(THREAD_EXIT_CODE);
	return 0;
}
*/