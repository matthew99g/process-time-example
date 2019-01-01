#include "timelib.h"

#include <sys/times.h>
#include <time.h>

// System CPU time is time spent in KERNEL mode
// User CPU time is time spend in USER mode

static void
displayProcessTime(const char *msg)
{
    sTimes stimes;
    clock_t clockTime;
    static long clockTicks = 0;

    if (msg != NULL)
        printf("%s", msg);

    if (clockTicks == 0) {
        clockTicks = sysconf(_SC_CLK_TCK);
        if(clockTicks == -1)
            errnoExit("sysconf");
    }

    stimes = getTimes();

    clockTime = clock();
    if(clockTime == -1)
        errnoExit("clock");

    printf("\t\tclock() returns: %ld clocks-per-sec (%.8f secs)\n",
            (long) clockTime, (double) clockTime / CLOCKS_PER_SEC);

    printf("\t\ttimes() yields: user CPU=%.8f; system CPU: %.8f\n",
            stimes.usrTime,
            stimes.sysTime);

    printf("\t\tTotal time yields: Real CPU=%.8f\n",
            stimes.realTime);

    fflush(stdout);
}

int
main(const int argc, const char *argv[])
{
    int numCalls, j;

    printf("CLOCKS_PER_SEC=%ld sysconf(_SC_CLK_TCK)=%ld\n\n",
            (long) CLOCKS_PER_SEC, sysconf(_SC_CLK_TCK));

    displayProcessTime("At program start:\n");

    numCalls = (argc > 1) ? getInt(argv[1], GN_GT_O, "num-calls") : 100000000;
    for(j = 0; j < numCalls; j++)
        (void) getppid();

    displayProcessTime("After getppid() loop:\n");

    exit(EXIT_SUCCESS);
}