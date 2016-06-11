%{
/*
	Set Conjunto := { Ele1, Ele2, eneN };
	Sets;
	ShowSets;
	ShowSet Conjunto;
	Union Cn, ...;
	Intersect Cn, ...;
	SetUnion Dest, Cn, ...;
	SetIntersect Dest, Cn, ...;
	Dest<-Conjunto;
	Delete Conjunto;
*/
#include "data.h"
#include "set.h"

struct Data* stack;
struct Set* context;

void InitStack(void);
void InitContext(void);
void AssignSet(char* name);
void CopySet(char *dest, char *origin);
void UniteSets(void);
void IntersectSets(void);
void SaveSetUnion(void);
void SaveSetIntersect(void);


%}

%locations
%define parse.error verbose
%pure_parser

%union  {
    char* sval;
    int tok;
}

%token <sval> SET SETS SHOWSETS SHOWSET UNION INTERSECT SETUNION SETINTERSECT DELETE
%token <sval> OP_COPY OP_ASSIGN BRACE_OPEN BRACE_CLOSE EOS COMMA
%token <sval> UNKNOWN KEYWORD

%start Program

%%

Program:
    Instruction
    | Program Instruction
    ;

Instruction: 
    SetCmd
    | SetsCmd
    | ShowSetsCmd
    | ShowSetCmd
    | UnionCmd
    | IntersectCmd 
    | SetUnionCmd 
    | SetIntersectCmd
    | DeleteCmd
    | AssignOperation
    ;
    
SetCmd:
    SET KEYWORD OP_ASSIGN BRACE_OPEN List BRACE_CLOSE EOS   { AssignSet($2); }
    ;

List:
    KEYWORD                 { InitStack(); AddData(stack, $1); };
    | List COMMA KEYWORD    { AddData(stack, $3); };
    ;
    
SetsCmd:
    SETS EOS    { printf("Set list: \n"); PrintSetList(context); }
    ;
    
ShowSetsCmd:
    SHOWSETS EOS    { printf("Set content list\n"); PrintSetVerboseList(context); }
    ;

ShowSetCmd:
    SHOWSET KEYWORD EOS     { PrintSetContent(SearchSet(context, $2));; }
    ;

UnionCmd:
    UNION List EOS      { UniteSets(); }
    ;
    
IntersectCmd:
    INTERSECT List EOS      { IntersectSets(); }
    ;

SetUnionCmd:
    SETUNION List EOS       { SaveSetUnion(); }
    ;

SetIntersectCmd:
    SETINTERSECT List EOS       { SaveSetIntersect(); }
    ;

DeleteCmd:
    DELETE KEYWORD EOS          { printf("Deleting set %s\n", $2); DeleteSet(context, $2); }
    ;

AssignOperation:
    KEYWORD OP_COPY KEYWORD EOS { CopySet($1, $3); };
    ;

%%

int yyerror(char *s) {
    printf("yyerror : %s\n",s);
}

int main(int argc, char* argv[]) {
    yyin = fopen(argv[1], "r");
    /*printf("$ ");*/
    yyparse();
    FlushSets(context);
    FlushData(stack);
    free(context);
    free(stack);
}

void InitStack(void)    {
    if(stack == NULL)
        stack = NewDataList();
    else
        FlushData(stack);
}

void InitContext(void)  {
    if(context == NULL)
        context = NewSetList();
}

void AssignSet(char* name)  {
    InitContext();
    AddSet(context, stack, name);
    printf("Added Set %s\n", name);
}

void CopySet(char *dest, char *origin)  {
    struct Set *oriSet;
    oriSet = SearchSet(context, origin);
    
    if(oriSet == NULL)
        yyerror("Origin set is non existent");
    else
        AddSet(context, oriSet->data, dest);   
        
    printf("Set %s copied to %s\n", origin, dest);
}

void UniteSets(void)    {
    struct Set *buf;
    struct Data *sp, *result, *newRes;
    
    result = NewDataList();
    printf("Uniting sets ");
    for(sp = stack->next; sp != NULL; sp = sp->next)    {
        buf = SearchSet(context, sp->contents);
        if(buf == NULL) {
            yyerror("\nUnknown set used in Union operation\n");
            return;
        }
        printf("%s ", buf->name);
        newRes = UniteData(result, buf->data);
        DestroyDataList(result);
        result = newRes;
    }
    printf("\nResult := { ");
    PrintDataList(result);
    printf("}\n");
}

void IntersectSets(void)  {
    struct Set *buf;
    struct Data *sp, *result, *newRes;
    
    result = NewDataList();
    printf("Intersecting sets ");
    
    sp = stack->next;
    buf = SearchSet(context, sp->contents);
    if(buf == NULL) {
        yyerror("\nUnknown set used in Intersect operation\n");
        return;
    }
    printf("%s ", buf->name);
    CopyData(result, buf->data);
    
    for(sp = sp->next; sp != NULL; sp = sp->next)    {
        buf = SearchSet(context, sp->contents);
        if(buf == NULL) {
            yyerror("\nUnknown set used in Intersect operation\n");
            return;
        }
        printf("%s ", buf->name);
        newRes = IntersectData(result, buf->data);
        DestroyDataList(result);
        result = newRes;
    }
    printf("\nResult := { ");
    PrintDataList(result);
    printf("}\n");
}

void SaveSetUnion(void) {
    struct Set *buf;
    struct Data *sp, *result, *newRes, *first;
    char* name;
    
    result = NewDataList();
    printf("Uniting sets ");
    
    first = stack->next;
    name = strndup(first->contents, strlen(first->contents));
    
    if( SearchSet(context, name) != NULL )
        first = stack;
    
    for(sp = first->next; sp != NULL; sp = sp->next)    {
        buf = SearchSet(context, sp->contents);
        if(buf == NULL) {
            yyerror("\nUnknown set used in Union operation\n");
            return;
        }
        printf("%s ", buf->name);
        newRes = UniteData(result, buf->data);
        DestroyDataList(result);
        result = newRes;
    }
    
    AddSet(context, result, name);
    printf("\n%s := { ", name);
    PrintDataList(result);
    printf("}\n");
}

void SaveSetIntersect(void) {
    struct Set *buf;
    struct Data *sp, *result, *newRes;
    char *name;
    int firstElemExists;
    
    result = NewDataList();
    printf("Intersecting sets ");
    
    /* Get first element */
    sp = stack->next;
    buf = SearchSet(context, sp->contents);
    if(buf == NULL) {
        name = strndup(sp->contents, strlen(sp->contents) );
        firstElemExists = 0;
    }
    else    {
        CopyData(result, buf->data);
        name = strndup(buf->name, strlen(buf->name) );
        printf("%s ", buf->name);
        firstElemExists = 1;
    }
    
    /* Get second element */
    if(!firstElemExists)    {
        sp = sp->next;
        buf = SearchSet(context, sp->contents);
        if(buf == NULL) {
            yyerror("\nUnknown set used in Intersect operation\n");
            return;
        }
        CopyData(result, buf->data);
        printf("%s ", buf->name);
    }
    
    for(sp = sp->next; sp != NULL; sp = sp->next)    {
        buf = SearchSet(context, sp->contents);
        if(buf == NULL) {
            yyerror("\nUnknown set used in Intersect operation\n");
            return;
        }
        printf("%s ", buf->name);
        newRes = IntersectData(result, buf->data);
        DestroyDataList(result);
        result = newRes;
    }
    
    AddSet(context, result, name);
    
    printf("\n%s := { ", name);
    PrintDataList(result);
    printf("}\n");
}
