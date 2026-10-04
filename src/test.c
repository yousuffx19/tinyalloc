#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "talloc.h"

int main(){
        int *p = (int *) talloc(3000);
        printf("p:%p\n", p);
        int *q = (int *) talloc(2000);
        int *r = (int *) talloc(3000);
        printf("q:%p\n", q);
        printf("r:%p\n", r);
	tinyfree(q);
        tinyfree(p);
        int *x = (int *) talloc(1500);
        printf("x:%p\n", x);
        tinyfree(r);
        int *z = (int *) talloc(1600);
        printf("z:%p\n", z);
	tinyfree(z);
	tinyfree(x);
	int *a = talloc(1000);
	printf("a:%p\n", a);
        return 0;
}


