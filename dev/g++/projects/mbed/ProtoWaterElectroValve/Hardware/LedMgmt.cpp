#include "LedMgmt.h"

LedMgmt::LedMgmt(PinName p_ledPort) : _ledPort(p_ledPort) {
}

void LedMgmt::On() {
    _ledPort.write(1);
}

void LedMgmt::Off() {
    _ledPort.write(0);
}

void LedMgmt::flash(float msec) {
    _ledPort.write(1);
    wait(msec);
    _ledPort.write(0);
}

LedMgmt& LedMgmt::operator !=(const LedMgmt& p_ledMgmt) {
     _ledPort = !_ledPort;
    return *this;
}
