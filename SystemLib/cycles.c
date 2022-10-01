#include "cycles.h"

CycleInfo ci;

void initializeCycleInfo() {
    ci.numCycles = 0;
    ci.startTimePoint = 0;
    ci.cycleDuration = 0;
    ci.TSCFrequency = getTSCFrequency();
}

dword getTSCFrequency() {
    int cpuInfo[4];
    __cpuid(cpuInfo, 0x16);
    return cpuInfo[0] > 500 ? cpuInfo[0] : 2400;  //just a hack so the program will still run if*/
    return 0;
    //eax does not contain TSC frequency. It only will
    //on newer intell processors....
}

void incCycles() {
    ++ci.numCycles;
    while((__builtin_ia32_rdtsc() - ci.startTimePoint) < ci.cycleDuration);
    ci.startTimePoint = __builtin_ia32_rdtsc();
}

void resetCycles() {
    ci.numCycles = 0;
    ci.startTimePoint = __builtin_ia32_rdtsc();
}

uint64_t getCycles() {
    return ci.numCycles;
}

void setCycleDuration(double Mhz) {
    ci.cycleDuration = (ci.TSCFrequency /*- 70 * Mhz*/) / Mhz;
}
