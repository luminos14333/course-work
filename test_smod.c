#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main()
{
    FILE *fp;
    char buffer[64];
    fp = fopen("/proc/smodparam", "r");
    if (fp == NULL)
    {
        perror("Error opening /proc/smodparam");
        return 1;
    }
    if (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        printf("Current pproc value: %s", buffer);
    }
    fclose(fp);
    fp = fopen("/proc/smodparam", "w");
    if (fp == NULL)
    {
        perror("Error writing to /proc/smodparam");
        return 1;
    }
    fprintf(fp, "testval\n");
    fclose(fp);
    fp = fopen("/proc/smodparam", "r");
    if (fp == NULL)
    {
        perror("Error reopening /proc/smodparam");
        return 1;
    }
    if (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        printf("Updated pproc value: %s", buffer);
    }
    fclose(fp);
    fp = fopen("/sys/kernel/smod/psys", "r");
    if (fp == NULL)
    {
        perror("Error opening /sys/kernel/smod/psys");
        return 1;
    }
    if (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        printf("Current psys value: %s", buffer);
    }
    fclose(fp);
    fp = fopen("/sys/kernel/smod/psys", "w");
    if (fp == NULL)
    {
        perror("Error writing to /sys/kernel/smod/psys");
        return 1;
    }
    32 fprintf(fp, "777\n");
    fclose(fp);
    fp = fopen("/sys/kernel/smod/psys", "r");
    if (fp == NULL)
    {
        perror("Error reopening /sys/kernel/smod/psys");
        return 1;
    }
    if (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        printf("Updated psys value: %s", buffer);
    }
    fclose(fp);
    return 0;
}