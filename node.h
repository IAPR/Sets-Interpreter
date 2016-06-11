#ifndef NODE_H

#define NODE_H

#include<string.h>
#include<stdlib.h>
#include<assert.h>

struct Node {
    int tokenType;
    char* contents;
    struct Node* past;
    struct Node* next;
};

struct Node* NewStack()  {
    struct Node* new;
    new = malloc( sizeof(struct Node) );
    new->tokenType = 0;
    new->contents = NULL;
    new->past = new;
    new->next = NULL;
    return new;
}

void PrintNodes(struct Node *n)	{
    struct Node* s;
    for(s = n; s != NULL; s = s->next)
        printf("ADDR 0x%x PAST 0x%x NEXT 0x%x TOKEN 0x%x STR %s\n", s, s->past, s->next, s->tokenType, s->contents);
}

int GetNodeLenght(struct Node *n)  {
    struct Node* s;
    int i;
    
    if(n->next == NULL)
        return 0;
    
    for(s = n->next, i = 0; s->next != NULL; s = s->next, ++i);
    return i;
}

void AddNode(struct Node* n, int tokenType, char* yytext)    {
    if(yytext == NULL)
        assert(1);

    int lenght = strlen(yytext);
    struct Node *new, *last;
    new = malloc(sizeof(struct Node));
    new->tokenType = tokenType;
    new->contents = strndup(yytext, lenght);
     
    for(last = n; last->next != NULL; last = last->next);
    last->next = new;
    new->past = last;
    new->next = NULL;
}

void FlushNodes(struct Node* n) {
    if(n == NULL) 
        assert(1);
    if(n->next == NULL)
        return;
    
    /* DEBUG */
    printf("\nPrinting node list...\n");
    PrintNodes(n);
    printf("\nPrinted %i nodes...\n", GetNodeLenght(n) );
    printf("\nEliminating nodes...\n");
    /* DEBUG_END */
    
    struct Node *pointer, *buf;
    pointer = n->next;
    while(pointer->next != NULL)    {
        buf = pointer;
        pointer = pointer->next;
        free(buf);
    }
    free(pointer);
    n->next = NULL;
    n->past = n;
    
    /* DEBUG */
    printf("\nNodes after elimination\n");
    PrintNodes(n);
    /* DEBUG_END */
}

#endif
