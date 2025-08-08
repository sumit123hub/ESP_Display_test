#include "ESPMonitor.h"
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void ESPMonitor::begin() {
    Serial.begin(115200);
}

void ESPMonitor::printStatus() {
    Serial.println("Heap: " + String(getFreeHeap()));
    Serial.println("Flash: " + String(getFreeFlash()));
    Serial.println("Uptime (ms): " + String(getUptime()));
    Serial.println("Stack watermark: " + String(getStackWatermark()));
}

uint32_t ESPMonitor::getFreeHeap() {
    return ESP.getFreeHeap();
}

size_t ESPMonitor::getFreeFlash() {
    return ESP.getFreeSketchSpace();
}

uint32_t ESPMonitor::getUptime() {
    return millis();
}

uint32_t ESPMonitor::getStackWatermark() {
    return uxTaskGetStackHighWaterMark(NULL); // current task
}
