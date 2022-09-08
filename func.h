#ifndef FUNC_H
#define FUNC_H

void *safeAlloc(size_t);
void *safeRealloc(void*, size_t);
FILE *safeOpen(char*, char*);
void safeClose(FILE*);
void skipNewLine(FILE**);

struct rule *checkArgs(int, char*[], struct rule*, FILE*);
void exeMake(struct rule*);

#endif