#pragma once

#include "Arduino.h"

#include <stdint.h>

const uint32_t OPTFLOW_SERIAL_BAUDRATE = 115200;
const uint8_t FOV = 42;
const uint8_t IMAGE_WIDTH = 35;
const uint16_t MAX_HEIGHT = 2000; 
const uint8_t MIN_HEIGHT = 80; 

struct OptFlowMeasurement
{
    int32_t distance;
    double flowX;
    double flowY;
};

class Mateksys3901L0X {
    public:
        Mateksys3901L0X();
        ~Mateksys3901L0X();

        bool begin(HardwareSerial& serial);
        void end();
        bool isOpen();

        bool waitFlow();

        const OptFlowMeasurement & getCurrentFlow() {
            return _currentMeasurement;
        }
    private:
        double pixelToMillimeters(int32_t d);
        void setCurrentFlow(int32_t dx, int32_t dy, int32_t d, double fov_mm);
    private:
        HardwareSerial* _serial;
        OptFlowMeasurement _currentMeasurement;
        uint8_t _recvPos;

        char _cmdRec[2];
        char _payloadX[4];
        char _payloadY[4];

        uint16_t _cmd;
        int32_t _distance;
        int32_t _flowX;
        int32_t _flowY;
};