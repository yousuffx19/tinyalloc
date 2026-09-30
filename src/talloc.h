#ifndef talloc_h
#define talloc_h

void free_all();

void tinyfree(void *block);

void *talloc(int size);

#endif
