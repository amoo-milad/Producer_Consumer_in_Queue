// Producer_Consumer_in_Queue.cpp 

#include "stdafx.h"
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "q_functions.h"

HANDLE hNotFull = CreateEvent(NULL, TRUE, FALSE, NULL);
HANDLE hNotEmpty = CreateEvent(NULL, TRUE, FALSE, NULL);

DWORD WINAPI producer(LPVOID tpn) // TestInsertToQueue
{
	//srand(1);  // <=hey, why not using this very simple thing?!
	// is some thing wrong with the line above? (writing '1' instead of passing seed '1' and defining threadParams).
	// since we use a seperate thread for this function (one thread for this one func), then its ok.
	threadParams* tp = (threadParams*)tpn;
	Queue* q = tp->q;
	srand(tp->seed);

	// not here ... HANDLE hPauseIns = CreateEvent(NULL, TRUE, FALSE, NULL);

	Student st;
	int i = 1;
	while (true)
	{
		st.stno = i + 1000;
		st.age = i + 20;
		st.name[0] = 'M';
		st.name[1] = 'r';
		st.name[2] = '.';
		st.name[3] = 64 + i;	// Ascii Code of 65 == A
		st.name[4] = '\0';

		if (is_full(q)) // is full. MAX is 20
			ResetEvent(hNotFull);
			
		WaitForSingleObject(hNotFull, INFINITE);
			//Sleep;

		insert(q, st);
		SetEvent(hNotEmpty);

		printf("\n *** Inserted ...\n");
		i++;
		wait();
	}
	return 0;
}

DWORD WINAPI consumer(LPVOID tpn) // TestRemoveFromQueue
{
	//srand(2);	// <=the same question here; is this ok? I think it is.
	threadParams* tp = (threadParams*)tpn;
	Queue* q = tp->q;
	srand(tp->seed);
	
	Student st;

	while (true)
	{
		if (is_empty(q))
			ResetEvent(hNotEmpty);

		WaitForSingleObject(hNotEmpty, INFINITE);
//			Sleep;

		if (!is_empty(q)) // Which is not ever! ... the below:
		st = remove(q);
		SetEvent(hNotFull);
//			wakeup(TestInsertToQueue);

		printf("\n *** Removed ...\n ");
		printf("Student Name: %s\n", st.name);
		printf("Student ID  : %d\n", st.stno);
		printf("Student Age : %d\n", st.age);
		wait();
	}
	return 0;
}

int main()
{
	Queue q;
	initializeQ(&q);

	threadParams tp1;
	tp1.seed = 1;
	tp1.q = &q;
	threadParams tp2;
	tp2.seed = 2;
	tp2.q = &q;

	HANDLE hInsert = CreateThread(NULL, 0, &producer, &tp1, 0, NULL);
	HANDLE hRemove = CreateThread(NULL, 0, &consumer, &tp2, 0, NULL);
	//initializeQ(&q); // Can it be here instead?! answer: NO, Not Normally...
	// because the remove thread would want to free an already empty_space!
	//	but now that i've putten an init_func call, in the remove_func (i had to!), it doesn't matter any more!
	srand(3);

	while (true)
	{
		int x;
		printf("\n\nplease enter a number, each will do the specified job:\n\n ");
		printf("1=> Inserts data to the queue\n ");
		printf("2=> Removes data from the queue\n ");
		printf("3=> Shows the data in the queue\n ");
		printf("4=> Clears the queue\n ");
		printf("5=> Make a bunch of fake students\n ");
		printf("6=> Count the students\n ");
		printf("0=> Exit\n\n\n ");
		scanf_s("%d", &x);

		switch (x)
		{
		case(1):
			getStudent(&q);
			break;   /////////////////////////////////////////////////////////////////////////

		case(2):

			if (is_empty(&q))
				printf("\n List is empty");
			else
			{
				Student stdp;
				stdp = remove(&q);
				printf("\nStudent %s poped!", stdp.name);
			}
			break;   /////////////////////////////////////////////////////////////////////////		

		case(3):
			showQ(&q);
			break;   /////////////////////////////////////////////////////////////////////////

		case(4):
			clearQ(&q);
			break;   /////////////////////////////////////////////////////////////////////////

		case 5:
			printf("\nHow many fake students do you want? ");
			int n;
			scanf_s("%d", &n);
			fakeStudMaker(&q, n);
			break;   /////////////////////////////////////////////////////////////////////////

		case 6:
			//int c = countSt(&s1);
			//printf("\nthere are '%d' students in the stack. ", c);
			printf("\nthere are '%d' students in the stack. ", countSt(&q));
			break;	 /////////////////////////////////////////////////////////////////////////

		case(0):
			//EXIT
			return 0; /////////////////////////////////////////////////////////////////////////

		default:
			printf(" Invalid Request!  Try Again...\n");
			break;	 /////////////////////////////////////////////////////////////////////////
		}
	}
	_getch();
	return 0;
}
