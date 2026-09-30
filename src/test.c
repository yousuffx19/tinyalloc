#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "talloc.h"

int main(){
        int *p = (int *) talloc(2000);
        printf("p:%p\n", p);
        int *q = (int *) talloc(2000);
        int *r = (int *) talloc(3000);
        printf("q:%p\n", q);
        printf("r:%p\n", r);
        tinyfree(q);
        int *x = (int *) talloc(1500);
        printf("x:%p\n", x);
        tinyfree(x);
        int *z = (int *) talloc(1600);
        printf("z:%p\n", z);
        return 0;
}


