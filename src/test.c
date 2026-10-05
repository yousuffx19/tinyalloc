#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "talloc.h"

int main(){
        int *p = (int *) talloc(3000); // |P-3000, used| 
        printf("p:%p\n", p);
        int *q = (int *) talloc(2000); // |P-3000, used|->|Q-2000, used|
        int *r = (int *) talloc(3000); // |P-3000, used|->|Q-2000, used|->|R-3000, used|
        printf("q:%p\n", q);
        printf("r:%p\n", r);
	tinyfree(q); 		       // |P-3000, used|->|Q-2000, free|->|R-3000, used|
        tinyfree(p);                   // |P-5000, free|->|R-3000, used|
        int *x = (int *) talloc(1500); // |X-1500, used|->|.->3476, free|->|R-3000, used|
        printf("x:%p\n", x);
	int *y = (int *) talloc(3000); // |X-1500, used|->|Y->3000, used|->|R-3000,used|
	printf("y:%p\n", y);
        tinyfree(r);                   // |X-1500, used|->|Y->3000, used|
        int *z = (int *) talloc(1600); // |X-1500, used|->|Y->3000, used|->|Z-1600,used|
        printf("z:%p\n", z);
	tinyfree(y);
	tinyfree(z);
	tinyfree(x);                   //fully empty
	int *a = talloc(1000);         // |A-1000,used|
	printf("a:%p\n", a);
        return 0;
}


