#include "Mateksys3901L0X.h"

Mateksys3901L0X::Mateksys3901L0X() {

}

Mateksys3901L0X::~Mateksys3901L0X() {
    end();
}

bool Mateksys3901L0X::begin(HardwareSerial& serial) {
    if (isOpen()) {
      end(); 
    }
    _serial = &serial;
    _serial->begin(OPTFLOW_SERIAL_BAUDRATE);
}

void Mateksys3901L0X::end() {
    if(isOpen()) {
        _serial->end();
        _serial = nullptr;
    }
}

bool Mateksys3901L0X::isOpen() {
    return _serial ? true : false;
}

double Mateksys3901L0X::pixelToMillimeters(int32_t d) {
    return (2*d*tan(FOV/2))/IMAGE_WIDTH;
}

void Mateksys3901L0X::setCurrentFlow(int32_t dx, int32_t dy, int32_t d, double fov_mm) {
    _currentMeasurement.distance = d;
    _currentMeasurement.flowX = dx*fov_mm;
    _currentMeasurement.flowY = dy*fov_mm;
}

bool Mateksys3901L0X::waitFlow() {
    while(_serial->available()) {
        byte recByte = _serial->read();
        if(recByte == 0x24) {
            _recvPos = 0;
        }
        switch(_recvPos) {
            case 4:
                _cmdRec[0] = recByte;
                break;
            case 5:
                _cmdRec[1] = recByte;
                _cmd = (_cmdRec[1] << 8) + _cmdRec[0];
                break;
            case 9:
                _payloadX[0] = recByte;
                break;
            case 10:
                _payloadX[1] = recByte;
                break;
            case 11:
                _payloadX[2] = recByte;
                break;
            case 12:
                _payloadX[3] = recByte;
                if(_cmd == 7938) {
                    _flowX = _payloadX[0] | ((int32_t)_payloadX[1] << 8) | ((int32_t)_payloadX[2] << 16) | ((int32_t)_payloadX[3] << 24);
                } else if(_cmd == 7937) {
                    _distance = _payloadX[0] | ((int32_t)_payloadX[1] << 8) | ((int32_t)_payloadX[2] << 16) | ((int32_t)_payloadX[3] << 24);
                }
                break;
            case 13:
                _payloadY[0] = recByte;
                break;
            case 14:
                _payloadY[1] = recByte;
                break;
            case 15:
                _payloadY[2] = recByte;
                break;
            case 16:
                _payloadY[3] = recByte;
                if(_cmd == 7938) {
                    _flowY = _payloadY[0] | ((int32_t)_payloadY[1] << 8) | ((int32_t)_payloadY[2] << 16) | ((int32_t)_payloadY[3] << 24);
                }
                break;
        }

        _recvPos += 1;

        if(_cmd == 7938 && _recvPos > 17) {
            if(_distance < MIN_HEIGHT || _distance > MAX_HEIGHT) {
                _distance = 0;
            }

            double fov_mm = pixelToMillimeters(_distance);
            setCurrentFlow(_flowX, _flowY, _distance, fov_mm);
            return true;
        }
    }

    return false;
}