#ifndef CPROJECT_CYCLES_H
#define CPROJECT_CYCLES_H

#include "system_types.h"
#include <intrin.h>

void initializeCycleInfo();
dword getTSCFrequency();
void incCycles();
void resetCycles();
uint64_t getCycles();
void setCycleDuration(double);


typedef struct {
    uint64_t numCycles;
    uint64_t startTimePoint;
    uint64_t cycleDuration;
    uint64_t TSCFrequency;
} CycleInfo;

#endif //CPROJECT_CYCLES_H
