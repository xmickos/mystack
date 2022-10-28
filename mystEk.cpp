#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <math.h>
#include </Users/xmickos/Desktop/govno.jopa.kosmos/mystacklol/mystEkfuncs.cpp>

int main(){
    FILE* logfile = fopen("logfile.txt", "w");
    mystack stk1 = {};

    mystackCtor(&stk1, 5, logfile);


    for(int i = 0; i < 120; i++){
        mystackPush(&stk1, i, logfile);
    }


    mystackPop(&stk1, logfile);
    mystackPop(&stk1, logfile);

    for(int i = 0; i < 81; i++){
        mystackPop(&stk1, logfile);
    }
    

    mystackPush(&stk1, 9, logfile);


    mystackPush(&stk1, -2, logfile);
    mystackPush(&stk1, -2, logfile);
    mystackPush(&stk1, -2, logfile);


    fprintf(logfile, "\nFinal stack is:\n");  

    FinalStackOutput(&stk1, logfile);    
    

    mystackDtor(&stk1, logfile);

    errors_decoding(logfile);

    fclose(logfile);
    return 0;
}

/*          Возможные ошибки стека:
*   stk == nullptr                                       = 2^0 
*   stk -> data == nullptr                               = 2^1
*   stk -> size <= 0                                     = 2^2
*   stk -> capacity <= 0                                 = 2^3
*   stk -> size < stk -> capacity                        = 2^4
*   в элементах идущих после size лежит не POISON        = 2^5
*   первая канарейка мерта                               = 2^6
*   вторая канарейка мертва                              = 2^7
*
*/
