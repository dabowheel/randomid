#include <aqua/aqua.h>
#include <gmp.h>
#include "../lib/randomid.h"

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