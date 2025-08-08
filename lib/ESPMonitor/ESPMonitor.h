#ifndef ESPMONITOR_H
#define ESPMONITOR_H

#include <Arduino.h>
#include <cstddef>
class ESPMonitor {
public:
    void begin();
    void printStatus();
    uint32_t getFreeHeap();
    size_t getFreeFlash();
    float getCPUUsage(); // Advanced
    uint32_t getUptime();
    uint32_t getStackWatermark();
};

#endif
