// all the necessary functions in the queue.
#include "stdafx.h"
#include <stdlib.h>
#include "q_functions.h"
#include <windows.h>

HANDLE mutex = CreateMutex(NULL, FALSE, NULL);

const int MAX = 20;

void getStudent(Queue* q)
{
	Student std;
	printf("Enter students name:");
	scanf_s("%s", std.name);
	printf("Enter students IDno:");
	scanf_s("%d", &std.stno);
	printf("Enter students age:");
	scanf_s("%d", &std.age);

	insert(q, std);
}

void insert(Queue* q, Student st)
{
	WaitForSingleObject(mutex, INFINITE);

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

	ReleaseMutex(mutex);
}

Student remove(Queue* q)
{
	WaitForSingleObject(mutex, INFINITE);

	Node* node = q->down;
	q->down = q->down->next;
	//	q->down->prev = NULL;	// its unnecessary and makes some problems too.
	q->count--;
	if (is_empty(q))
		initializeQ(q); // i'm all aware that it's a shitty code! :/

	ReleaseMutex(mutex);

	Student std = node->st;
	free(node);
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

	WaitForSingleObject(mutex, INFINITE);

	for (node = q->down, i; node; node = node->next, i++)
	{
		printf("\n%d. ", i);
		printf("\n %s ,", node->st.name);
		printf("\t %d ,", node->st.stno);
		printf("\t %d \n", node->st.age);
	}

	ReleaseMutex(mutex);
}

void clearQ(Queue* q)
{
	if (is_empty(q))
	{
		printf("\nthe queue is already empty. ");
		return;
	}

	Node* node;

	WaitForSingleObject(mutex, INFINITE);

	while (q->top != q->down)
	{
		node = q->top;
		q->top = q->top->prev;
		free(node);
	}
	free(q->down);

	initializeQ(q);

	ReleaseMutex(mutex); // can it be upper (before the init_func) ?

	printf("\nthe queue is cleared. ");
}

void initializeQ(Queue* q)
{
	WaitForSingleObject(mutex, INFINITE);

	q->down = (Node*)malloc(sizeof Node);
	q->down->prev = NULL;
	q->down->next = NULL;

	q->top = q->down;
	q->count = 0;

	ReleaseMutex(mutex);
}

bool is_empty(Queue* q)
{
	WaitForSingleObject(mutex, INFINITE);
	bool is_it_empty = !q->count;
	ReleaseMutex(mutex);

	return(is_it_empty);
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
	WaitForSingleObject(mutex, INFINITE);
	int count = q->count;
	ReleaseMutex(mutex);

	return count;
}

