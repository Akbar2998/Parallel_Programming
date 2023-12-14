#include <stdio.h>
#include "pvm3.h"

int main() {
    int my_tid;
    int number_from_server, number_from_blue;
    int sum;

 // Joining the PVM
    my_tid = pvm_mytid();


    pvm_recv(-1, 2);
    pvm_upkint(&number_from_server, 1, 1);

    pvm_recv(-1, 4);
    pvm_upkint(&number_from_blue, 1, 1);

    // Sum the numbers
    sum = number_from_server + number_from_blue;

    // Send the result back to the server
    pvm_initsend(PvmDataDefault);
    pvm_pkint(&sum, 1, 1);
    pvm_send(pvm_parent(), 3);


    pvm_exit();
    return 0;
}
