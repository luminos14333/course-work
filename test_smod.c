/* smod_test.c
 * Test program for smod.ko kernel module
 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    FILE *f;
    int pproc_val;
    short new_psys;

    /* 1. Read 'pproc' from /proc */
    f = fopen("/proc/smodparam", "r");
    if (!f) {
        perror("Error opening /proc/smodparam");
        return EXIT_FAILURE;
    }
    if (fscanf(f, "%d", &pproc_val) != 1) {
        fprintf(stderr, "Failed to read pproc value\n");
        fclose(f);
        return EXIT_FAILURE;
    }
    fclose(f);
    printf("Current pproc value: %d\n", pproc_val);

    /* 2. Prompt user for new 'psys' value */
    printf("Enter new psys value (short): ");
    if (scanf("%hd", &new_psys) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return EXIT_FAILURE;
    }

    /* 3. Write new 'psys' to sysfs */
    f = fopen("/sys/module/smod/parameters/psys", "w");
    if (!f) {
        perror("Error opening /sys/module/smod/parameters/psys");
        return EXIT_FAILURE;
    }
    fprintf(f, "%hd\n", new_psys);
    fclose(f);

    printf("New psys value set: %hd\n", new_psys);
    return EXIT_SUCCESS;
}
