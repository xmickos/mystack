#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <math.h>
#include </Users/xmickos/Desktop/govno.jopa.kosmos/mystacklol/mystEk.h>

void errors_decoding(FILE* logfile){
    int count = 1;
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
            fprintf(logfile, " %d)  stk->size < 0!\n", count);
            count++;
        }
        if((errors & cap_neg) != 0){
            fprintf(logfile, " %d)  stk->capacity < 0!\n", count);
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

void FinalStackOutput(mystack* stk, FILE* logfile){
    int k = 0, i = 0;

    fprintf(logfile, "LINE %d: DUMP:\nSize = %lu,\nCapacity = %lu,\ndata: \n", __LINE__, stk->size, stk->capacity);

    if(stk->data[-1] == (double)FirstCanaree){
        fprintf(logfile, "  data[-1] = FirstCanaree\n");
    }else{
        fprintf(logfile, "  data[-1] = %f != FirstCanaree == %u (!)\n", stk->data[-1], FirstCanaree);
    }

    while(k < stk->capacity && i < stk->capacity && k < 200 && i < 200){
        if(stk->data[k] != POISON){
            for(;stk->data[k] != POISON && k < stk->capacity && k < 200 && i < 200 && i < stk->capacity; k++){
                fprintf(logfile, "  data[%u] = %f\n", k, stk->data[k]);
            }
        }else{
            i = k;

            while(stk->data[k] == POISON && k < stk->capacity && k < 200 && i < 200 && i<stk->capacity) i++;

            fprintf(logfile, "  data[%d:%d] = POISON\n", k, i);
        }
    }

    if(stk->data[stk->capacity] == (double)SecondCanaree){
        fprintf(logfile, "  data[stk->capacity] = SecondCanaree\n");
    }else{
        fprintf(logfile, "  data[stk->capacity] = %f != SecondCanaree == %u, (double)SecondCanaree == %f (!)\n", stk->data[stk->size], SecondCanaree, (double)SecondCanaree);
    }
    fprintf(logfile, "\n");   
}


int mystackResize(mystack* stk, int direction, FILE* logfile){
    // double scale_coefficient = 2;

    // assert(stk);
    // assert(stk->size > stk->capacity);      //заменить на if`ы – нефатальные assert`ы
    // assert(stk->capacity <= 0);             //сделать условную компиляцию для debug мода
    // assert(stk->size <= 0);


    if(stk->data == nullptr){
        fprintf(logfile, "LINE %d: FUNC: /%s/: (!) stk->data is nullptr!\n", __LINE__, __FUNCTION__);
        errors = errors | data_null;
        return -1;
    }
    ASSERT(stk, logfile, errors);
    CanareeCheck(stk, logfile, errors);
    if(!(direction == 0 || direction == 1)){
        fprintf(logfile, "  LINE %d: (!) %s(): direction is %d\n", __LINE__, __FUNCTION__, direction);
        return -1;
    }

/*                         
*                       Зачем передавать аргумент newsize, если фун-я resize определяет новый
*                       размер в себе ? Достаточно определить увеличение/уменьшение ? 
*                       остальные параметры стека хранятся в его структуре. 
*                       Direction == 1 <=> capacity возрастёт
*                       Direction == 0 <=> capacity уменьшится
*
*                       Увеличение: size ––> size * scale_coeff(size) <=> scale_coeff зависит от size –
*                       не явл. фун-ей. (хотя..?)
*                       Уменьшение: size * 2 > capacity => capacity ––> capacity / 2 ? 
*                       Тогда свободная зона всегда существует и равна capacity / 2 - size до recalloc`a.
*
*                       А когда вызывать фун-ю на уменьшение размера стека?                        
*
*/


    int scale_coefficient = 2;

    fprintf(logfile, "\n  LINE %d: direction is %d\n", __LINE__, direction);

    if(direction){
        stk->data = (Elem_t*)realloc(stk->data - 1, stk->capacity*sizeof(Elem_t)*(scale_coefficient + 2)) + 1;
        fprintf(logfile, "FUNC: /%s/, LINE %d: capacity = %lu\n",__FUNCTION__, __LINE__, stk->capacity);

        if(stk->data - 1 == nullptr){
            fprintf(logfile, "FUNC: /%s/, LINE %d: stk->data - 1 is nullptr after the realloc!\n", __FUNCTION__, __LINE__);
            errors = errors | data_null;
            return -1;
        }

        stk->capacity = stk->capacity * scale_coefficient;
        stk->data[-1] = FirstCanaree;
        stk->data[stk->capacity] = SecondCanaree;
        
        
        for(int i = stk->size; i < stk->capacity; i++){
            stk->data[i] = POISON;
        }

        fprintf(logfile, "  LINE %d: UPPER RESIZED: stk->capacity = %lu\n", __LINE__, stk->capacity);
    }
    else{
        if(stk->capacity > 2 * scale_coefficient * stk->size) 
        {
            // StackOutput(stk, logfile);
            FinalStackOutput(stk, logfile);

            stk->data = (Elem_t*)realloc(stk->data - 1, sizeof(Elem_t)*(stk->capacity/scale_coefficient + 2)) + 1;
            stk->capacity = stk->capacity/scale_coefficient;
            
            if(stk->data == nullptr){
                fprintf(logfile, "FUNC: /%s/, LINE %d: stk->data is nullptr after the realloc!\n", __FUNCTION__, __LINE__);
                errors = errors | data_null;
                return -1;
            }
            if(stk->data - 1 == nullptr){
                fprintf(logfile, "FUNC: /%s/, LINE %d: stk->data[-1] is nullptr after the realloc!\n", __FUNCTION__, __LINE__);
                errors = errors | data_null;
                return -1;
            }
            if(stk->data + stk->capacity == nullptr){
                fprintf(logfile, "FUNC: /%s/, LINE %d: stk->data[stk->capacity] is nullptr after the realloc!\n", __FUNCTION__, __LINE__);
                errors = errors | data_null;
                return -1;
            }

            stk->data[-1] = FirstCanaree;
            stk->data[stk->capacity] = SecondCanaree;
            
            CanareeCheck(stk, logfile, errors);

            fprintf(logfile, "\n  LINE %d: DOWN RESIZED: stk->capacity = %lu, stk->size: %lu\n", __LINE__, stk->capacity, stk->size);


        }
    }
    return 0;
}


int mystackPush(mystack* stk, Elem_t value, FILE* logfile){

    if(stk->data == nullptr){
        fprintf(logfile, "FUNC: /%s/, LINE %d: stk->data is nullptr!\n", __FUNCTION__, __LINE__);
        errors = errors | data_null;
        exit(-1);
    }
    ASSERT(stk, logfile, errors);
    CanareeCheck(stk, logfile, errors);

    fprintf(logfile, "\nFUNC: /%s/\n", __FUNCTION__);
    fprintf(logfile, "  LINE %d: Pushing value = %f\n", __LINE__, value);

    if(stk->size == stk->capacity){
//                                          ОБРАБОТКА ОШИБОК – в первых строчках функции
        fprintf(logfile, "  LINE %d: *RESIZING UP*", __LINE__);
        if(mystackResize(stk, 1, logfile) == -1){
            fprintf(logfile, "FUNC: /%s/, LINE %d: (!) FAILed to resize! Stk->size is %lu\n", __FUNCTION__, __LINE__, stk->size);
            exit(-1);
        }
    }   

    if(stk->capacity > 8 && stk->capacity > 4 * stk->size){
//                                          ОБРАБОТКА ОШИБОК
        fprintf(logfile, "  LINE %d: *RESIZING DOWN*", __LINE__);

        if(mystackResize(stk, 0, logfile) == -1){
            fprintf(logfile, "FUNC: /%s/, LINE %d: (!) FAILed to resize!\n", __FUNCTION__, __LINE__);
            exit(-1);
        }

        // StackOutput(stk, logfile);
        FinalStackOutput(stk, logfile);
        CanareeCheck(stk, logfile, errors);
    }
        
    if(stk->data[stk->size] == POISON){
        fprintf(logfile, "  LINE %d: Pushed stk->data[%lu]: %s ––>", __LINE__, stk->size, "POISON");
        stk->data[stk->size/**sizeof(Elem_t)*/] = value;
        stk->size++;
    }else{
        fprintf(logfile, "  LINE %d: (!) Poisoned element is not POISON!\nPoisoned elem = stk->data[%lu] = %f\n",__LINE__ - 4, stk->size, stk->data[stk->size]);
        errors = errors | wrong_poison;
    }

    fprintf(logfile, " %f, stk->size: %lu ––> %lu\n", stk->data[(stk->size-1)], (stk->size-1), stk->size);

    return 0;
}


int mystackPop(mystack* stk, FILE* logfile){

    ASSERT(stk, logfile, errors);

    if(stk->capacity > 8 && stk->capacity > 4 * stk->size){
//                                          ОБРАБОТКА ОШИБОК
        fprintf(logfile, "  LINE %d: *RESIZING DOWN*", __LINE__);
        if(mystackResize(stk, 0, logfile) == -1){
            fprintf(logfile, "FUNC: /%s/, LINE %d: (!) FAILed to resize!\n", __FUNCTION__, __LINE__);
        }        

        CanareeCheck(stk, logfile, errors);
    }

    if(stk->data[0] == POISON){
        fprintf(logfile, "  LINE %d: Stack is empty!\n", __LINE__);
    }
    else{
        fprintf(logfile, "  LINE %d: Popping data[%d] = %f\n", __LINE__, 0, stk->data[0]);
        
        for(unsigned int i = 0; i < stk->size; i++){
            stk->data[i] = stk->data[i+1];
        }
        stk->data[stk->size] = POISON;
        stk->size--;
    }


    
    // StackOutput(stk, logfile);
    return 0;
}

int mystackCtor(mystack* stk, int size, FILE* logfile){
    if(size <= 0){
        fprintf(logfile, "LINE %d: (!) size <= 0, size == %d ", __LINE__, size);
        errors = errors | size_neg;
        return -1;
    }
    if(stk != nullptr){
        fprintf(logfile, "FUNC: /%s/\n  LINE %d: Stack was already created!\n", __FUNCTION__, __LINE__);
    }
    fprintf(logfile, "FUNC /%s/: LINE %d: size == %d\n", __FUNCTION__, __LINE__, size);
    ASSERT(stk, logfile, errors);

    stk->data = (Elem_t*)calloc(size + 2, sizeof(Elem_t)) + 1;
    stk->capacity = size;           // ЯД? 
    //ЯД: 
    for(int i = 0; i < stk->capacity; i++){
        stk->data[i] = POISON;
    }

    stk->data[-1] = FirstCanaree;
    stk->data[size] = SecondCanaree; 

    fprintf(logfile, "\n    *** Created stack ***\n\nsize = %lu,\ncapacity = %lu.\n", stk->size, stk->capacity);

    FinalStackOutput(stk, logfile);
    CanareeCheck(stk, logfile, errors);

    return 0;
}

int mystackDtor(mystack* stk, FILE* logfile){
    if(stk == nullptr){
        fprintf(logfile, "FUNC: /%s/\n  LINE %d: stk is nullptr!", __FUNCTION__, __LINE__);
        return -1;
    }else{
        for(int i = 0; i < stk->capacity; i++){
            stk->data[i] = POISON;
        }
        free(stk->data - 1);
        // free((void*)stk);
        return 0;
    }
}