#include <stdio.h>
#include <openssl/rand.h>
#include <gmp.h>
#include <aqua/aqua.h>

typedef struct rlink {
    unsigned int r;
    struct rlink *next;
} *rlink;

rlink rlCreate()
{
    rlink l = a_malloc(sizeof(struct rlink));
    l->next = NULL;
    return l;
}

rlink rlPush(rlink head, rlink l)
{
    l->next = head;
    return l;
}

void rlPrint(rlink head)
{
    for (rlink l = head; l; l = l->next) {
        printf("%u ", l->r);
    }
    printf("\n");
}

typedef struct charmap {
    unsigned int size;
    char *map;
} *charmap;

charmap cmCreate(unsigned int size)
{
    charmap map;

    map = a_malloc(sizeof(struct charmap));
    map->size = size;
    map->map = a_malloc(size);
    return map;
}

void cmDestroy(charmap map)
{
    free(map->map);
    free(map);
}

charmap Create16CharMap()
{
    charmap map = cmCreate(16);
    int i = 0;

    for (int j = 0; j < 10; j++, i++) {
        map->map[i] = j + '0';
    }

    for (int j = 0; j < 6; j++, i++) {
        map->map[i] = j + 'A';
    }

    return map;
}

charmap Create62CharMap()
{
    charmap map = cmCreate(62);
    int i = 0;

    for (int j = 0; j < 10; j++, i++) {
        map->map[i] = j + '0';
    }

    for (int j = 0; j < 26; j++, i++) {
        map->map[i] = j + 'a';
    }

    for (int j = 0; j < 26; j++, i++) {
        map->map[i] = j + 'A';
    }

    return map;
}

charmap Create94CharMap()
{
    charmap map = cmCreate(94);
    int i = 0;

    for (int j = 33; j <= 126; j++, i++) {
        map->map[i] = j;
    }

    return map;
}

a_string GetRandomBytes(unsigned int size)
{
    a_string_builder b = a_sbldcreate();
    int ret;

    a_sbldaddlen(b, size);
    ret = RAND_bytes((unsigned char *)b->data, 100);
    if (ret != 1) {
        return NULL;
    }

    return a_sbld2s(b);
}

void bytes2mpz(a_string s, mpz_t n)
{
    unsigned int place = 0;

    for (int i = s->len - 1; i >= 0; i--) {
        mpz_t single;
        mpz_init(single);
        mpz_set_ui(single, (unsigned char)s->data[i]);
        for(int j = 0; j < place; j++) {
            mpz_mul_ui(single, single, 256);
        }
        mpz_add(n, n, single);
        place++;
    }
}

void cmPrint(charmap map)
{
    for (int i = 0; i < map->size; i++) {
        printf("%c ", map->map[i]);
    }
    printf("\n");
}

rlink convert(mpz_t n, unsigned int base)
{
    mpz_t q;
    mpz_t r;
    unsigned int r2;
    rlink head = NULL;
    rlink l = NULL;

    mpz_init(q);
    mpz_set(q, n);
    mpz_init(r);

    while (mpz_cmp_ui(q, 0) > 0) {
        r2 = mpz_fdiv_qr_ui(q, r, q, base);
        l = rlCreate();
        l->r = r2;
        head = rlPush(head, l);
    }

    return head;
}

a_string FormatNumber(rlink head, charmap map)
{
    a_string_builder b = a_sbldcreate();

    for (rlink l = head; l; l = l->next) {
        a_sbldaddchar(b, map->map[l->r]);
    }

    return a_sbld2s(b);
}

int main(int argc, char *argv[])
{
    a_string s;
    mpz_t n;
    int size;
    rlink head;
    charmap map;
    a_string num;

    const char usage[] = "Usage: convert <size>\n";
    if (argc != 2) {
        fprintf(stderr, usage);
        return 1;
    }

    size = atoi(argv[1]);
    if (size < 1) {
        fprintf(stderr, "Size must be a positive integer\n");
        return 1;
    }

    s = GetRandomBytes(size);
    if (!s) {
        fprintf(stderr, "Could not get random bytes\n");
        return 1;
    }
    for (int i = 0; i < s->len; i++)
        printf("%u ", (unsigned char)s->data[i]);
    printf("\n");

    mpz_init(n);
    bytes2mpz(s, n);
    gmp_printf("num10 = %Zu\n", n);

    /* base 16 */
    map = Create16CharMap();
    cmPrint(map);
    head = convert(n, map->size);    
    rlPrint(head);
    num = FormatNumber(head, map);
    printf("num16 = %s\n", num->data);
    cmDestroy(map);
    a_sdestroy(num);

    /* base 62 */
    map = Create62CharMap();
    cmPrint(map);
    head = convert(n, map->size);    
    rlPrint(head);
    num = FormatNumber(head, map);
    printf("num62 = %s\n", num->data);
    cmDestroy(map);
    a_sdestroy(num);

    /* base 94 */
    map = Create94CharMap();
    cmPrint(map);
    head = convert(n, map->size);
    num = FormatNumber(head, map);
    printf("num94 = %s\n", num->data);
    cmDestroy(map);
    a_sdestroy(num);

    a_sdestroy(s);
    return 0;
}