#include <aqua/aqua.h>
#include <gmp.h>
#include "../lib/randomid.h"

int main(int argc, char *argv[])
{
    int size;
    int base;
    charmap map;
    a_string num;
    a_string error;

    const char usage[] = "Usage: randomid <size> <base>\n"
        " size is the number of random bytes to generate; it must be a positive integer.\n"
        " base must be 10, 16, 62, or 94.\n";

    if (argc != 3) {
        fprintf(stderr, usage);
        return 1;
    }

    size = atoi(argv[1]);
    if (size < 1) {
        fprintf(stderr, "Size must be a positive integer\n");
        return 1;
    }

    base = atoi(argv[2]);
    switch (base) {
        case 10:
            map = Create10CharMap();
            break;
        case 16:
            map = Create16CharMap();
            break;
        case 62:
            map = Create62CharMap();
            break;
        case 94:
            map = Create94CharMap();
            break;
        default:
            fprintf(stderr, "base must be 10, 16, 62 or 94\n");
            return 1;
    }

    num = randomID(size, map, &error);
    if (!num) {
        fprintf(stderr, "%s\n", error->data);
        a_sdestroy(error);
        return 1;
    }
    printf("%s\n", num->data);

    cmDestroy(map);
    a_sdestroy(num);

    return 0;
}