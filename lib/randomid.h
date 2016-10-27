#include <gmp.h>

typedef struct rlink {
    unsigned int r;
    struct rlink *next;
} *rlink;

rlink rlCreate();
rlink rlPush(rlink head, rlink l);
void rlPrint(rlink head);


typedef struct charmap {
    unsigned int size;
    char *map;
} *charmap;

charmap cmCreate(unsigned int size);
void cmDestroy(charmap map);
charmap Create10CharMap();
charmap Create16CharMap();
charmap Create62CharMap();
charmap Create94CharMap();
a_string GetRandomBytes(unsigned int size);
void bytes2mpz(a_string s, mpz_t n);
void cmPrint(charmap map);
rlink convert(mpz_t n, unsigned int base);
a_string FormatNumber(rlink head, charmap map);
a_string randomID(unsigned int size, charmap map, a_string *errorptr);
void rlDestroy(rlink head);
