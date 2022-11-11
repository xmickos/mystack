#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <math.h>
#include "mystEk.h"

void errors_decoding(FILE* logfile){
    int count = 0;
    if(errors != 0){
        fprintf(logfile, "Errors:\n");
        if((errors & stk_null) != 0) {
            fprintf(logfile, "  %d) stk is nullptr!\n", count);
            count++;
            }
        if((errors & data_null) != 0){
            fprintf(logfile, " %d)  stk->data is nullptr!\n", count);
            count++;
        }
        if((errors & size_neg) != 0){
            fprintf(logfile, " %d)  stk->size = 0!\n", count);
            count++;
        }
        if((errors & cap_neg) != 0){
            fprintf(logfile, " %d)  stk->capacity = 0!\n", count);
            count++;
        }
        if((errors & cap_less_size) != 0){
            fprintf(logfile, " %d)  stk->size > stk->capacity!\n", count);
            count++;
        }
        if((errors & wrong_poison) != 0){
            fprintf(logfile, " %d)  poisoned element is not poison!\n", count);
            count++;
        }
        if((errors & first_canaree_dead) != 0){
            fprintf(logfile, " %d)  first canaree`s dead!\n", count);
            count++;
        }
        if((errors & second_canaree_dead) != 0){
            fprintf(logfile, " %d)  second canaree`s dead!\n", count);
        }
        fprintf(logfile, "Total errors count: %d\n", count);
    }else{
        fprintf(logfile, "\nNo errors finded!\n");
    }
}

int mystackResize(mystack* stk, int direction, FILE* logfile){
    // double scale_coefficient = 2;

    if(stk->data == nullptr){
        fprintf(logfile, "LINE %d: FUNC: /%s/: (!) stk->data is nullptr!\n", __LINE__, __FUNCTION__);
        errors = errors | data_null;
        exit(-1);
    }

    Verificator(stk, logfile, errors);

    if(!(direction == 0 || direction == 1)){
        exit(-1);
    }

    int scale_coefficient = 2;

    if(direction){
        stk->data = (Elem_t*)realloc(stk->data - 1, stk->capacity*sizeof(Elem_t)*(scale_coefficient + 2)) + 1;

        if(stk->data - 1 == nullptr){
            fprintf(logfile, "FUNC: /%s/, LINE %d: stk->data - 1 is nullptr after the realloc!\n", __FUNCTION__, __LINE__);
            errors = errors | data_null;
            exit(-1);
        }

        stk->capacity = stk->capacity * scale_coefficient;
        stk->data[-1] = FirstCanaree;
        stk->data[stk->capacity] = SecondCanaree;
        
        
        for(int i = stk->size; i < stk->capacity; i++){
            stk->data[i] = POISON;
        }

    }
    else{
        if(stk->capacity > 2 * scale_coefficient * stk->size) 
        {
            stk->data = (Elem_t*)realloc(stk->data - 1, sizeof(Elem_t)*(stk->capacity/scale_coefficient + 2)) + 1;
            stk->capacity = stk->capacity/scale_coefficient;
            
            if(stk->data == nullptr){
                fprintf(logfile, "FUNC: /%s/, LINE %d: stk->data is nullptr after the realloc!\n", __FUNCTION__, __LINE__);
                errors = errors | data_null;
                FinalStackOutput(stk, logfile);
                exit(-1);
            }

            if(stk->data - 1 == nullptr){
                fprintf(logfile, "FUNC: /%s/, LINE %d: stk->data[-1] is nullptr after the realloc!\n", __FUNCTION__, __LINE__);
                FinalStackOutput(stk, logfile);
                errors = errors | data_null;
                exit(-1);
            }

            if(stk->data + stk->capacity == nullptr){
                fprintf(logfile, "FUNC: /%s/, LINE %d: stk->data[stk->capacity] is nullptr after the realloc!\n", __FUNCTION__, __LINE__);
                FinalStackOutput(stk, logfile);
                errors = errors | data_null;
                exit(-1);
            }

            stk->data[-1] = FirstCanaree;
            stk->data[stk->capacity] = SecondCanaree;
        }
    }
    return 0;
}


int mystackPush(mystack* stk, Elem_t value, FILE* logfile){

    //verficator
    //push | if errors -> write in error field
    //dump
    

    Verificator(stk, logfile, errors);
    CanareeCheck(stk, logfile, errors);

    if(stk->data == nullptr){
        errors = errors | data_null;
        errors_decoding(logfile);
        exit(-1);
    }

    if(stk->size == stk->capacity){
        if(mystackResize(stk, 1, logfile) == -1){
            fprintf(logfile, "FUNC: /%s/, LINE %d: (!) FAILed to resize! Stk->size is %lu\n", __FUNCTION__, __LINE__, stk->size);
            exit(-1);
        }
    }   

    if(stk->capacity > 8 && stk->capacity > 4 * stk->size){

        if(mystackResize(stk, 0, logfile) == -1){
            fprintf(logfile, "FUNC: /%s/, LINE %d: (!) FAILed to resize!\n", __FUNCTION__, __LINE__);
            exit(-1);
        }
    }
        
    if(stk->data[stk->size] == POISON){
        stk->data[stk->size] = value;

        stk->size++;
    }else{
        errors = errors | wrong_poison;
        printf("A TEPER TUTA!\n");
        FinalStackOutput(stk, logfile);
        exit(-1);
    }

    return 0;
}


int mystackPop(mystack* stk, FILE* logfile){

    Verificator(stk, logfile, errors);

    if(stk->capacity > 8 && stk->capacity > 4 * stk->size){
        if(mystackResize(stk, 0, logfile) == -1){
            fprintf(logfile, "FUNC: /%s/, LINE %d: (!) FAILed to resize!\n", __FUNCTION__, __LINE__);
            FinalStackOutput(stk, logfile);
            exit(-1);
        }        

    }

    if(stk->data[0] == POISON){
        fprintf(logfile, "  LINE %d: Stack is empty!\n", __LINE__);
    }
    else{
        fprintf(logfile, "Popping data[%d] = %f\n", 0, stk->data[0]);
        
        for(unsigned int i = 0; i < stk->size; i++){
            stk->data[i] = stk->data[i+1];
        }
        stk->data[stk->size] = POISON;
        stk->size--;
    }

    return 0;
}

int mystackCtor(mystack* stk, int size, FILE* logfile){

    if(stk == nullptr){
        fprintf(logfile, "LINE %d: FUNC: /%s/: (!) capacity == 0!\n", __LINE__, __FUNCTION__);
        errors = errors | stk_null;
        errors_decoding(logfile);
        exit(-1);
    }

    if(size <= 0){
        fprintf(logfile, "LINE %d, FUNC: /%s/: (!) size == 0!", __LINE__, __FUNCTION__);
        errors = errors | size_neg;
        exit(-1);
    }

    if(stk->data != nullptr){
        fprintf(logfile, "FUNC: /%s/\n  LINE %d: Stack was already created! Recreating...\n", __FUNCTION__, __LINE__);
        mystackDtor(stk, logfile);
    }

    stk->data = (Elem_t*)calloc(size + 2, sizeof(Elem_t)) + 1;

    stk->capacity = size;

    for(int i = 0; i < stk->capacity; i++){
        stk->data[i] = POISON;
    }

    stk->data[-1] = FirstCanaree;
    stk->data[size] = SecondCanaree; 

    return 0;
}

int mystackDtor(mystack* stk, FILE* logfile){
    if(stk == nullptr){
        fprintf(logfile, "FUNC: /%s/\n  LINE %d: stk is nullptr!", __FUNCTION__, __LINE__);
        exit(-1);
    }else{
        for(int i = 0; i < stk->size; i++){
            stk->data[i] = POISON;
        }
        free(stk->data - 1);

        return 0;
    }
}

void MainFinalStackOutput(mystack* stk, FILE* logfile){
    FinalStackOutput(stk, logfile);
}

// murmur hash 
