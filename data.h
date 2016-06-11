#ifndef DATA_H

#define DATA_H

#include<string.h>
#include<stdlib.h>
#include<assert.h>

struct Data {
    char* contents;
    struct Data* past;
    struct Data* next;
};

int DataCheck(struct Data *n)  
{
    if(n == NULL)
        assert(1);
    else if(n->next == NULL)
        return 0;
    else
        return 1;
}

struct Data* SearchData(struct Data* n, char* str)
{
    struct Data* p;
    
    if(!DataCheck(n))
        return NULL;
    
    for(p = n->next; p != NULL; p = p->next)    {
        int l = strlen(str);
        if( strncmp(p->contents, str, l) == 0)  {
            return p;
        }
    }
    return NULL;
}

struct Data* NewDataList()  {
    struct Data* new;
    new = malloc( sizeof(struct Data) );
    new->contents = NULL;
    new->past = new;
    new->next = NULL;
    return new;
}

void PrintData(struct Data *n)  {
    struct Data* s;
    
    if(!DataCheck(n))
        return;
        
    for(s = n; s != NULL; s = s->next)
        printf("DATA ADDR 0x%x PAST 0x%x NEXT 0x%x TXT %s\n", s, s->past, s->next, s->contents);
    printf("\n");
}

void PrintDataList(struct Data *n)  {
    struct Data* s;
    
    if(!DataCheck(n))
        return;
    
    for(s = n->next; s != NULL; s = s->next)    {
        if(s->next != NULL)
            printf("%s, ", s->contents);
        else
            printf("%s ", s->contents);
    }
    /* DEBUG 
    printf("\n\n");
    /* DEBUG_END */
}

void AddData(struct Data* n, char* yytext)    {
    if(yytext == NULL)
        assert(1);

    if(SearchData(n, yytext) != NULL)
        return;

    int lenght = strlen(yytext);
    struct Data *new, *last;
    new = malloc(sizeof(struct Data));
    new->contents = strndup(yytext, lenght);
     
    for(last = n; last->next != NULL; last = last->next);
    last->next = new;
    new->past = last;
    new->next = NULL;
}

void CopyData(struct Data* dest, struct Data *source) {
    struct Data *buf;
    
    if(!DataCheck(source))
        return;
    
    if(dest == NULL)
        return;
        
    /* Add to existing dest */
    for(buf = source->next; buf != NULL; buf = buf->next )
        AddData(dest, buf->contents);
    
    /* DEBUG
    PrintData(dest);
    /* DEBUG_END */
}

struct Data* DuplicateData(struct Data* source)  {
    struct Data *dest, *buf;
    dest = NewDataList();
    
    /* Add to dest */
    for(buf = source->next; buf != NULL; buf = buf->next )
        AddData(dest, buf->contents);
    
    /* DEBUG
    PrintData(dest);
    /* DEBUG_END */
    
    return dest;
    
}

struct Data* IntersectData(struct Data* d1, struct Data* d2)    {
    struct Data* res;
    res = NewDataList();
    
    if(!DataCheck(d1) || !DataCheck(d2))
        return res;
        
    struct Data *p;
    for(p = d1->next; p != NULL; p = p->next)   {
        if(SearchData(d2, p->contents) != NULL)
            AddData(res, p->contents);
    }
    
    return res;
}

struct Data* UniteData(struct Data* d1, struct Data* d2)    {
    struct Data *res, *p;
    res = NewDataList();
    for(p = d1->next; p != NULL; p = p->next)
        AddData(res, p->contents);
    for(p = d2->next; p != NULL; p = p->next)
        AddData(res, p->contents);
    return res;
}

int GetDataLenght(struct Data *n)  {
    struct Data* s;
    int i;
    
    if(!DataCheck(n))
        return 0;
    
    for(s = n->next, i = 0; s->next != NULL; s = s->next, ++i);
    return i;
}

void FlushData(struct Data* n) {
    if(!DataCheck(n))
        return;
    
    /* DEBUG 
    printf("\nPrinting Data list...\n");
    PrintData(n);
    printf("\nPrinted %i Data...\n", GetDataLenght(n) );
    printf("\nEliminating Data...\n");
    /* DEBUG_END */
    
    struct Data *pointer, *buf;
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
    printf("\nData after elimination...\n");
    PrintData(n);
    printf("\n\n");
    /* DEBUG_END */
}

void DestroyDataList(struct Data* n) {
    if(!DataCheck(n))
        return;
    
    FlushData(n);
    free(n);
    n = NULL;
}

#endif
