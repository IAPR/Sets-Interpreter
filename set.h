#ifndef SET_H

#define SET_H

#include<string.h>
#include<stdlib.h>
#include<assert.h>

#include "data.h"

struct Set {
    struct Data* data;
    char* name;
    struct Set* past;
    struct Set* next;
};

int SetCheck(struct Set *n)  
{
    if(n == NULL)
        assert(1);
    else if(n->next == NULL)
        return 0;
    else
        return 1;
}

struct Set* NewSetList()  {
    struct Set* new;
    new = malloc( sizeof(struct Set) );
    new->data = NULL;
    new->name = NULL;
    new->past = new;
    new->next = NULL;
    return new;
}

void PrintSetsDebugInfo(struct Set *n)	{
    struct Set* s;
    
    if(!SetCheck(n))
        return;
    
    for(s = n; s != NULL; s = s->next)	{
        printf("SET ADDR 0x%x PAST 0x%x NEXT 0x%x NAME %s\n", s, s->past, s->next, s->name);
        if(s->data != NULL)
            PrintData(s->data);
	}
}

void PrintSetList(struct Set *n)	{
	struct Set *buf;
	
	if(!SetCheck(n))
		return;
		
	for(buf = n->next; buf != NULL; buf = buf->next)	{
		if(buf->next != NULL)
			printf("%s, ", buf->name);
		else
			printf("%s\n", buf->name);
	}
}

void PrintSetVerboseList(struct Set *n)	{
	struct Set *buf;
	
	if(!SetCheck(n))
		return;
		
	for(buf = n->next; buf != NULL; buf = buf->next)	{
		PrintSetContent(buf);
	}
}

void PrintSetContent(struct Set *n)	{
	if(n == NULL || n->past == n)
		return;
		
	printf("%s := { ", n->name);
	PrintDataList(n->data);
	printf("}\n");
}

struct Set* SearchSet(struct Set* n, char* str)
{
    struct Set* p;
    
    if(!SetCheck(n))
        return NULL;
    
    for(p = n->next; p != NULL; p = p->next)    {
        int l = strlen(str);
        if( strncmp(p->name, str, l) == 0)  {
            return p;
        }
    }
    return NULL;
}

int GetSetLenght(struct Set *n)  {
    struct Set* s;
    int i;
    
    if(n->next == NULL)
        return 0;
    
    for(s = n->next, i = 0; s->next != NULL; s = s->next, ++i);
    return i;
}

void AddSet(struct Set* n, struct Data* dataArray, char* yytext)    {
    SetCheck(n);
    
    if(yytext == NULL)
        assert(1);
        
    if(SearchSet(n, yytext) != NULL)    {
        DeleteSet(n, yytext);
    }

    int lenght = strlen(yytext);
    struct Set *new, *last;
    new = malloc(sizeof(struct Set));
    
    new->data = NewDataList();
    if(dataArray != NULL)
        CopyData(new->data, dataArray);
    
    new->name = strndup(yytext, lenght);
     
    for(last = n; last->next != NULL; last = last->next);
    last->next = new;
    new->past = last;
    new->next = NULL;
}

void DeleteSet(struct Set *n, char *name)   {
    struct Set *past, *next, *cur;
    cur = SearchSet(n, name);
    
    if(cur == NULL)
        return;
    
    past = cur->past;
    next = cur->next;
    if(next != NULL)    {    
        past->next = next;
        next->past = past;
    }
    else
        past->next = NULL;
    free(cur);
}

void FlushSets(struct Set* n) {
    if(!SetCheck(n))
		return;
    
    /* DEBUG 
    printf("\nPrinting node list...\n");
    PrintSetsDebugInfo(n);
    printf("\nPrinted %i nodes...\n", GetSetLenght(n) );
    printf("\nEliminating nodes...\n");
    /* DEBUG_END */
    
    struct Set *pointer, *buf;
    pointer = n->next;
    while(pointer->next != NULL)    {
        buf = pointer;
        pointer = pointer->next;
        free(buf);
    }
    free(pointer);
    n->next = NULL;
    n->past = n;
    
    /* DEBUG 
    printf("\nSets after elimination\n");
    PrintSetsDebugInfo(n);
    /* DEBUG_END */
}

#endif
