#include <stdio.h>
#include <stdlib.h>
#include "pvm3.h"

int main() {
    // Numbers for the testing
    int blue_tid, red_tid;
    int number_for_blue = 5; 
    int number_for_red = 3;  
    int result;


    pvm_mytid();


    pvm_spawn("blue_client", NULL, PvmTaskDefault, "", 1, &blue_tid);
    pvm_spawn("red_client", NULL, PvmTaskDefault, "", 1, &red_tid);


    pvm_initsend(PvmDataDefault);
    pvm_pkint(&number_for_blue, 1, 1);
    pvm_send(blue_tid, 1);

    pvm_initsend(PvmDataDefault);
    pvm_pkint(&number_for_red, 1, 1);
    pvm_send(red_tid, 2);


 
    pvm_recv(red_tid, 3);
    pvm_upkint(&result, 1, 1);


    printf("Final result: %d\n", result);


    pvm_exit();
    return 0;
}
