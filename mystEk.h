#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <math.h>

#define POISON -47391

#define FirstCanaree 0xDEFEC8ED
#define SecondCanaree 0xCAFED00D

#define stk_null 0b00000001
#define data_null 0b00000010
#define size_neg 0b00000100
#define cap_neg 0b00001000
#define cap_less_size 0b00010000
#define wrong_poison 0b00100000
#define first_canaree_dead 0b01000000
#define second_canaree_dead 0b10000000


char errors = 0;

typedef double Elem_t;

struct mystack
{
    Elem_t* data;
    size_t size;
    size_t capacity;
};

#define FinalStackOutput(stk, logfile)  int k = 0, i = 0;               \
                                                                        \
    fprintf(logfile, "LINE %d: DUMP:\nSize = %lu,\nCapacity = %lu,\ndata: \n",__LINE__, stk->size, stk->capacity);  \
                                                                        \
    if(stk->data[-1] == (double)FirstCanaree){                          \
        fprintf(logfile, "  data[-1] = FirstCanaree\n");                \
    }else{                                                              \
        fprintf(logfile, "  data[-1] = %f != FirstCanaree == %x (!)\n", stk->data[-1], FirstCanaree);   \
    }                                                                   \
                                                                        \
    while(k < stk->capacity && i < stk->capacity){                      \
        if(stk->data[k] != POISON){                                     \
            for(;stk->data[k] != POISON && k < stk->capacity && i < stk->capacity; k++){    \
                fprintf(logfile, "  data[%u] = %f\n", k, stk->data[k]); \
            }                                                           \
        }else{                                                          \
            i = k;                                                      \
                                                                        \
            while(stk->data[k] == POISON && k < stk->capacity && i < stk->capacity) i++;        \
            fprintf(logfile, "  data[%d:%d] = POISON\n", k, i - 1);     \
        }                                                               \
    }                                                                   \
                                                                        \
    if(stk->data[stk->capacity] == (double)SecondCanaree){              \
        fprintf(logfile, "  data[stk->capacity] = SecondCanaree\n");    \
    }else{                                                              \
        fprintf(logfile, "  data[stk->capacity] = %f != SecondCanareen", stk->data[stk->size]); \
    }                                                                   \
    fprintf(logfile, "\n");                                             \



#define CanareeCheck(stk, logfile, errors) if(stk->data[-1] != FirstCanaree){   \
    fprintf(logfile, "\nFUNC: /%s/\n  LINE %d: \n  FirstCanaree`s dead! stk->data[-1] == %f\n", __FUNCTION__, __LINE__, stk->data[-1]);     \
    errors = errors | first_canaree_dead;                               \
    FinalStackOutput(stk, logfile);                                     \
    errors_decoding(logfile);                                           \
    exit(-1);                                                           \
}                                                                       \
if(stk->data[stk->capacity] != SecondCanaree){                          \
    fprintf(logfile, "\nFUNC: /%s/\n  LINE %d: \n  SecondCanaree`s dead! stk->data[stk->capacity(%lu)] == %f\n", __FUNCTION__, __LINE__,stk->capacity, stk->data[stk->capacity]);  \
    errors = errors | second_canaree_dead;                              \
    exit(-1);                                                           \
}                                                                       \

#define Verificator(stk, logfile, errors)     if(logfile == nullptr){   \
    printf("LINE %d: logfile is nullptr!\n", __LINE__);                 \
    exit(-1);                                                           \
}                                                                       \
if(stk == nullptr){                                                     \
    fprintf(logfile, "LINE %d: FUNC: /%s/: (!) stk is nullptr!\n", __LINE__, __FUNCTION__);       \
    errors = errors | stk_null;                                         \
    errors_decoding(logfile);                                           \
    exit(-1);                                                           \
}                                                                       \
if(stk->capacity == 0){                                                 \
    fprintf(logfile, "LINE %d: FUNC: /%s/: (!) capacity == 0!\n", __LINE__, __FUNCTION__);\
    errors = errors | cap_neg;                                          \
    errors_decoding(logfile);                                           \
    exit(-1);                                                           \
}                                                                       \
if(stk->data[0] != POISON && stk->size == 0){                                                      \
    fprintf(logfile, "LINE %d: FUNC: /%s/: (!) size = 0!\n", __LINE__, __FUNCTION__);        \
    FinalStackOutput(stk, logfile);                                     \
    errors = errors | size_neg;                                         \
    errors_decoding(logfile);                                           \
    exit(-1);                                                           \
}                                                                       \
if(stk->capacity < stk->size){                                          \
    fprintf(logfile, "LINE %d: FUNC: /%s/: (!) capacity < size!\ncapacity = %lu, size = %lu\n", __LINE__,__FUNCTION__, stk->capacity, stk->size);    \
    FinalStackOutput(stk, logfile);                                     \
    errors = errors | cap_less_size;                                    \
    errors_decoding(logfile);                                           \
    exit(-1);                                                           \
}                                                                       \
for(int lol = stk->size; lol < stk->capacity; lol++){                   \
    if(stk->data[lol] != POISON){                                       \
        printf("YA TUTA!!!\n");                                         \
        errors = errors | wrong_poison;                                 \
        FinalStackOutput(stk, logfile);                                 \
        errors_decoding(logfile);                                       \
        exit(-1);                                                       \
    }                                                                   \
}                                                                       \


typedef double Elem_t;

struct mystack;

void errors_decoding(FILE* logfile);

int mystackResize(mystack* stk, int direction, FILE* logfile);

int mystackPush(mystack* stk, Elem_t value, FILE* logfile);

int mystackPop(mystack* stk, FILE* logfile);

int mystackCtor(mystack* stk, int size, FILE* logfile);

int mystackDtor(mystack* stk, FILE* logfile);
