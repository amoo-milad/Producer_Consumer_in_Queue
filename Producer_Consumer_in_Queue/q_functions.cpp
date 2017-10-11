// all the necessary functions in the queue.
#include "stdafx.h"
#include <stdlib.h>
#include <windows.h>
#include "q_functions.h"

const int MAX = 20;

void getStudent(Queue* q)
{
	Student std;
	printf("Enter students name:");
	scanf("%s", std.name);
	printf("Enter students IDno:");
	scanf_s("%d", &std.stno);
	printf("Enter students age:");
	scanf_s("%d", &std.age);

	insert(q, std);
}

void insert(Queue* q, Student st)
{
	WaitForSingleObject(q->hNotFull, INFINITE);
	
	WaitForSingleObject(q->hMutex, INFINITE);

	//Sleep;
	{	// insert 

		Node* node;
		if (is_empty(q))
		{
			node = q->down;
		}
		else
		{
			node = (Node*)malloc(sizeof Node);
			node->next = NULL;
			node->prev = q->top;
			q->top->next = node;
		}

		node->st = st;

		q->top = node; // same as before:  q->top->next;
		q->count++;
	}

	SetEvent(q->hNotEmpty);
	
	if (is_full(q)) // is full. MAX is 20
		ResetEvent(q->hNotFull);

	ReleaseMutex(q->hMutex);
}

Student remove(Queue* q)
{
	Student std;
	WaitForSingleObject(q->hNotEmpty, INFINITE);
	//			Sleep;

	WaitForSingleObject(q->hMutex, INFINITE);

	//	if (!is_empty(q)) // Which is not ever! ... the below:
	{
		Node* node = q->down;
		q->down = q->down->next;
		//	q->down->prev = NULL;	// its unnecessary and makes some problems too.
		q->count--;
		if (is_empty(q))
			initializeQ(q); // i'm all aware that it's a shitty code! :/

		ReleaseMutex(q->hMutex);

		std = node->st;
		free(node);
	}
	
	SetEvent(q->hNotFull);

	if (is_empty(q))
		ResetEvent(q->hNotEmpty);

	ReleaseMutex(q->hMutex);
	return std;
}

void showQ(Queue* q)
{
	if (is_empty(q))
	{
		printf("\nthe list is empty!");
		return;
	}

	printf("\nthe items in the given indexes are as below:");
	int i = 1;
	Node* node;

	WaitForSingleObject(q->hMutex, INFINITE);

	for (node = q->down, i; node; node = node->next, i++)
	{
		printf("\n%d. ", i);
		printf("\n %s ,", node->st.name);
		printf("\t %d ,", node->st.stno);
		printf("\t %d \n", node->st.age);
	}

	ReleaseMutex(q->hMutex);
}

void clearQ(Queue* q)
{
	if (is_empty(q))
	{
		printf("\nthe queue is already empty. ");
		return;
	}

	Node* node;

	WaitForSingleObject(q->hMutex, INFINITE);

	while (q->top != q->down)
	{
		node = q->top;
		q->top = q->top->prev;
		free(node);
	}
	free(q->down);

	initializeQ(q);

	ReleaseMutex(q->hMutex); // can it be upper (before the init_func) ?

	printf("\nthe queue is cleared. ");
}

void initializeQ(Queue* q)
{
	q->hMutex = CreateMutex(NULL, FALSE, NULL);
	WaitForSingleObject(q->hMutex, INFINITE);

	q->hNotFull = CreateEvent(NULL, TRUE, TRUE, NULL);
	q->hNotEmpty = CreateEvent(NULL, TRUE, TRUE, NULL);

	q->down = (Node*)malloc(sizeof Node);
	q->down->prev = NULL;
	q->down->next = NULL;

	q->top = q->down;
	q->count = 0;

	ReleaseMutex(q->hMutex);
}

bool is_empty(Queue* q)
{
	WaitForSingleObject(q->hMutex, INFINITE);
	bool is_it_empty = !q->count;
	ReleaseMutex(q->hMutex);

	return(is_it_empty);
}

bool is_full(Queue* q)
{
	WaitForSingleObject(q->hMutex, INFINITE);
	bool is_it_full = (q->count == MAX);
	ReleaseMutex(q->hMutex);

	return(is_it_full);
}

void fakeStudMaker(Queue* q, int n)
{
	Student st;
	for (int i = 1; i <= n; i++)
	{
		st.name[0] = 'M';
		st.name[1] = 'r';
		st.name[2] = '.';
		st.name[3] = 64 + i;	// Ascii Code of 65 == A
		st.name[4] = '\0';
		st.stno = i + 1000;
		st.age = i + 20;

		insert(q, st);
	}
}

int countSt(Queue* q)
{
	WaitForSingleObject(q->hMutex, INFINITE);
	int count = q->count;
	ReleaseMutex(q->hMutex);

	return count;
}

void delay(long double msecs)
{
	Sleep(msecs);
}

long double random()
{
	return (long double)rand() / (long double)((unsigned)RAND_MAX + 1) * 500;
}

void wait()
{
	// it attaches delay and random functions to make a small wait.
	// define 3 different params, before calling it in the 3 threads: srand(n);
	long double msecs = random();
	delay(msecs);
}

