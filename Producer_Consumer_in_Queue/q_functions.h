#pragma once

#ifndef _Q_FUNCTIONS_H_
#define _Q_FUNCTIONS_H_

struct Student
{
	char name[20]; // new; made the MAX 20 
	int stno;
	int age;
};

struct Node
{
	Student st;
	Node* next;
	Node* prev;
};

struct Queue
{
	Node* top;
	Node* down;
	int count;
};

struct threadParams
{
	int seed;
	Queue q;
};

void getStudent(Queue* q);
void insert(Queue* q, Student st);
Student remove(Queue* q);
void showQ(Queue* q);
void clearQ(Queue* q);
void initializeQ(Queue* q);
bool is_empty(Queue* q);
bool is_full(Queue* q);
void fakeStudMaker(Queue* q, int n);
int countSt(Queue* q);
void delay(long double msecs);
long double random();
void wait();

#endif