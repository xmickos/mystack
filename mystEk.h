#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <math.h>

#define POISON -47391

#define FirstCanaree 0xDEFEC8ED
#define SecondCanaree 0xCAFED00D

#define stk_null 00000001
#define data_null 00000010
#define size_neg 00000100
#define cap_neg 00001000
#define cap_less_size 00010000
#define wrong_poison 00100000
#define first_canaree_dead 01000000
#define second_canaree_dead 10000000


unsigned char errors = 0;

typedef double Elem_t;

struct mystack
{
    Elem_t* data;
    size_t size;
    size_t capacity;
};

#define ASSERT(stk, logfile, errors)     if(logfile == nullptr){                \
    printf("LINE %d: logfile is nullptr!\n", __LINE__);                 \
    return -1;                                                          \
}                                                                       \
if(stk == nullptr){                                                     \
    fprintf(logfile, "LINE %d: FUNC: /%s/: (!) stk is nullptr!\n", __LINE__, __FUNCTION__);       \
    errors = errors | stk_null;                                         \
    return -1;                                                          \
}                                                                       \
if(stk->size < 0){                                                      \
    fprintf(logfile, "LINE %d: FUNC: /%s/: (!) size < 0!\nsize = %lu", __LINE__, __FUNCTION__, stk->size);        \
    errors = errors | size_neg;                                         \
    return -1;                                                          \
}                                                                       \
if(stk->capacity < stk->size){                                          \
    fprintf(logfile, "LINE %d: FUNC: /%s/: (!) capacity < size!\ncapacity = %lu, size = %lu\n", __LINE__,__FUNCTION__, stk->capacity, stk->size);    \
    errors = errors | cap_less_size;                                    \
    return -1;                                                          \
}                                                                       \
                                                                        \
fprintf(logfile, "\nFUNC: /%s/\n  LINE %d: Assert passed!\n", __FUNCTION__, __LINE__);              \


#define CanareeCheck(stk, logfile, errors) if(stk->data[-1] != FirstCanaree){   \
    fprintf(logfile, "\nFUNC: /%s/\n  LINE %d: \n  FirstCanaree`s dead! stk->data[-1] == %f\n", __FUNCTION__, __LINE__, stk->data[-1]);     \
    errors = errors | first_canaree_dead;                               \
    return -1;                                                          \
}                                                                       \
if(stk->data[stk->capacity] != SecondCanaree){                          \
    fprintf(logfile, "\nFUNC: /%s/\n  LINE %d: \n  SecondCanaree`s dead! stk->data[stk->capacity(%lu)] == %f\n", __FUNCTION__, __LINE__,stk->capacity, stk->data[stk->capacity]);  \
    errors = errors | second_canaree_dead;                              \
    return -1;                                                          \
}                                                                       \
fprintf(logfile, "  Both canarees are alive!");      \

typedef double Elem_t;

struct mystack;

void errors_decoding(FILE* logfile);

void FinalStackOutput(mystack* stk, FILE* logfile);

int mystackResize(mystack* stk, int direction, FILE* logfile);

int mystackPush(mystack* stk, Elem_t value, FILE* logfile);

int mystackPop(mystack* stk, FILE* logfile);

int mystackCtor(mystack* stk, int size, FILE* logfile);

int mystackDtor(mystack* stk, FILE* logfile);