#include <stdio.h>
#include "pvm3.h"

int main() {
    int my_tid;
    int number;
    int doubled_number;


    my_tid = pvm_mytid();


    pvm_recv(-1, 1);
    pvm_upkint(&number, 1, 1);

    // Double the number
    doubled_number = number * 2;

  
    pvm_initsend(PvmDataDefault);
    pvm_pkint(&doubled_number, 1, 1);
    pvm_send(pvm_parent(), 4); 

    pvm_exit();
    return 0;
}
