#include <iostream>

#include "DS130x_I2C.h"

namespace DS130X_I2C {

CDS130X_I2C::CDS130X_I2C(const unsigned char p_slaveAddress, const PinName p_sda, const PinName p_scl, const CDS130X_I2C::OscillatorMode p_oscillatorMode, const bool p_outputLevel, const int p_frequency) : I2C(p_sda, p_scl), _dayOfWeek("SunMonTueWedThuFriSat") {
    DEBUG_ENTER("CDS130X_I2C::CDS130X_I2C: %02x - %x - %d", p_slaveAddress, p_oscillatorMode, p_frequency)

    _slaveAddress = p_slaveAddress;
    frequency(p_frequency); // Set the frequency of the I2C interface
    _oscillatorMode = p_oscillatorMode;
    _outputLevel = p_outputLevel;

    DEBUG_LEAVE("CDS130X_I2C::CDS130X_I2C")
}

CDS130X_I2C::~CDS130X_I2C() {
    DEBUG_ENTER("~CDS130X_I2C")

    DEBUG_LEAVE("~CDS130X_I2C")
}

bool CDS130X_I2C::Initialize() {
    DEBUG_ENTER("CDS130X_I2C::Initialize")

    // 1. Set control register
    unsigned char controlRegisterValue = 0x00;
    switch (_oscillatorMode) { // See Datasheet - Clause CONTROL REGISTER
        case CDS130X_I2C::One_Hz:
            controlRegisterValue = 0x10;
            break;
        case CDS130X_I2C::Four_KHz:
            controlRegisterValue = 0x11;
            break;
        case CDS130X_I2C::Height_KHz:
            controlRegisterValue = 0x12;
            break;
        case CDS130X_I2C::ThirtyTwo_KHz:
            controlRegisterValue = 0x13;
            break;
        case CDS130X_I2C::Output:
            controlRegisterValue = (_outputLevel == true) ? 0x80 : 0x00;
            break;
    } // End of 'switch' statement
    DEBUG("CDS130X_I2C::Initialize: controlRegisterValue = 0x%02x", controlRegisterValue)
    // Write the control register
    if (!Write(CDS130X_I2C::ControlRegisterAddress, controlRegisterValue)) {
        DEBUG_ERROR("CDS130X_I2C::Initialize: I2C write operation failed")
        return false;
    } // else continue

    // 2. Set the date format: Hours<6> set to 0 for 24-hour mode - See datasheet - Table 2. Timekeeper Registers
    DEBUG("CDS130X_I2C::Initialize: Set date format")
    // 1. Read hours
    unsigned char hoursRegister = 0xff;
    if (Read(CDS130X_I2C::HoursAddress, &hoursRegister)) {
        DEBUG("CDS130X_I2C::Initialize: hours:%x", hoursRegister)
        // 2. Set bit 6 to 0
        hoursRegister &= 0xbf; // 1011 1111
        // 3. Write new value
        if (!Write(CDS130X_I2C::HoursAddress, hoursRegister)) {
            DEBUG_ERROR("CDS130X_I2C::Initialize: Failed to set date format")
        }
    } else {
        DEBUG_ERROR("CDS130X_I2C::Initialize: Failed to set date format")
    }

    // 3. Set CH bit is Seconds register - See Datasheet - Clause CLOCK AND CALENDAR
    bool result = ControlClock(true);

    DEBUG_LEAVE("CDS130X_I2C::Initialize: %x", result)
    return result;
} // End of method CDS130X_I2C::Initialize

bool CDS130X_I2C::RestartClock() {
    return ControlClock(true);
} // End of method CDS130X_I2C::RestartClock

bool CDS130X_I2C::HaltClock() {
    return ControlClock(false);
} // End of method CDS130X_I2C::HaltClock

bool CDS130X_I2C::ControlClock(bool p_mode) {
    DEBUG_ENTER("CDS130X_I2C::ControlClock: 0x%02x", p_mode)

    // 1. Read seconds
    unsigned char secondsRegister = 0x00;
    if (Read(CDS130X_I2C::SecondsAddress, &secondsRegister)) {
        DEBUG("CDS130X_I2C::ControlClock: seconds register = 0x%02x", secondsRegister)
        // 2. Set bit
        if (!p_mode) {
            secondsRegister |= 0x80; // Set CH bit to halt oscilator
        } else {
            secondsRegister &= 0x7f; // Unset CH bit to restart oscilator
        }
        DEBUG("CDS130X_I2C::ControlClock: seconds register (new value) = 0x%02x - mode: %x", secondsRegister, p_mode)
        // 3. Write new value
        if (Write(CDS130X_I2C::SecondsAddress, secondsRegister)) {
            DEBUG_LEAVE("CDS130X_I2C::ControlClock (true)")
            return true;
        }

        DEBUG_LEAVE("CDS130X_I2C::ControlClock (false)")
        return false;
    }

    DEBUG_LEAVE("CDS130X_I2C::ControlClock (false)")
    return false;
} // End of method CDS130X_I2C::ControlClock

bool CDS130X_I2C::Read(const RegisterEnum p_address, unsigned char * p_byte, const CDS130X_I2C::RegisterFormatEnum p_format) {
    DEBUG_ENTER("CDS130X_I2C::Read: %02x - %x", p_address, p_format)

    // 1. Read seconds
    char i2cBuffer[1];
    i2cBuffer[0] = (char)(unsigned char)p_address;
    // Send I2C start + memory address
    if (write(_slaveAddress, i2cBuffer, 1, true) == 0) {
        // 2. Read data + I2C stop
        int result = read(_slaveAddress, (char *)p_byte, 1); // TODO Use result
        wait(0.02);

        // 3. Format convertion
        DEBUG_ENTER("CDS130X_I2C::Read: check %02x - %02x", p_format, Binary)
        if (p_format == CDS130X_I2C::Binary) {
            switch ((RegisterEnum)p_address) {
                case CDS130X_I2C::SecondsAddress:
                    //No break;
                case CDS130X_I2C::MinutesAddress:
                    *p_byte = ConvertBCDToHex(*p_byte & 0x7f); // Convert hex to BCD - See datasheet - Table 2. Timekeeper Registers
                    break;
                case CDS130X_I2C::HoursAddress:
                    //No break;
                case CDS130X_I2C::DayAddress:
                    *p_byte = ConvertBCDToHex(*p_byte & 0x3f); // Convert hex to BCD - See datasheet - Table 2. Timekeeper Registers
                    break;
                case CDS130X_I2C::DayOfWeekAddress:
                    *p_byte = ConvertBCDToHex(*p_byte & 0x07); // Convert hex to BCD - See datasheet - Table 2. Timekeeper Registers
                    break;
                case CDS130X_I2C::MonthAddress:
                    *p_byte = ConvertBCDToHex(*p_byte & 0x1f); // Convert hex to BCD - See datasheet - Table 2. Timekeeper Registers
                    break;
                case CDS130X_I2C::YearAddress:
                    *p_byte = ConvertBCDToHex(*p_byte); // Convert hex to BCD - See datasheet - Table 2. Timekeeper Registers
                    break;
            } // End of 'switch' statement
        } // else nothing to do

        DEBUG_LEAVE("CDS130X_I2C::Read %d/%02x", (int)*p_byte, *p_byte)
        return true;
    }

    DEBUG_LEAVE("CDS130X_I2C::Read (false)")
    return false;
} // End of method CDS130X_I2C::Read

bool CDS130X_I2C::Write(const RegisterEnum p_address, const unsigned char p_byte, const CDS130X_I2C::RegisterFormatEnum p_format) {
    DEBUG_ENTER("CDS130X_I2C::Write: %02x - %02x - %x", p_address, (int)p_byte, p_format)

    // 1. Format convertion
    unsigned char value = p_byte;
    if (p_format == CDS130X_I2C::Binary) {
        switch ((RegisterEnum)p_address) {
            case CDS130X_I2C::SecondsAddress:
                //No break;
            case CDS130X_I2C::MinutesAddress:
                value = ConvertHexToBCD(p_byte) & 0x7f; // Convert hex to BCD - See datasheet - Table 2. Timekeeper Registers
                break;
            case CDS130X_I2C::HoursAddress: // Force Hours<6> set to 0 for 24-hour mode - See datasheet - Table 2. Timekeeper Registers
                //No break;
            case CDS130X_I2C::DayAddress:
                value = ConvertHexToBCD(p_byte) & 0x3f; // Convert hex to BCD - See datasheet - Table 2. Timekeeper Registers
                break;
            case CDS130X_I2C::DayOfWeekAddress:
                value = ConvertHexToBCD(p_byte) & 0x07; // Convert hex to BCD - See datasheet - Table 2. Timekeeper Registers
                break;
            case CDS130X_I2C::MonthAddress:
                value = ConvertHexToBCD(p_byte) & 0x1f; // Convert hex to BCD - See datasheet - Table 2. Timekeeper Registers
                break;
            case CDS130X_I2C::YearAddress:
                value = ConvertHexToBCD(p_byte); // Convert hex to BCD - See datasheet - Table 2. Timekeeper Registers
                break;
        } // End of 'switch' statement
        DEBUG("CDS130X_I2C::Write: Convert binary %d into bcd %02x", (int)p_byte, value)
    } // else nothing to do

    // 2. Read seconds
    char i2cBuffer[2];
    i2cBuffer[0] = (char)(unsigned char)p_address;
    i2cBuffer[1] = value;
    // Send I2C start + memory address
    if (write(_slaveAddress, i2cBuffer, 2) == 0) {
        wait(0.02);
        DEBUG_LEAVE("CDS130X_I2C::Write (true)")
        return true;
    }

    DEBUG_LEAVE("CDS130X_I2C::Write (false)")
    return false;
} // End of method CDS130X_I2C::Write

bool CDS130X_I2C::SetTime(const std::string p_utcTime) {
    DEBUG_ENTER("CDS130X_I2C::SetTime: %s - %d", p_utcTime.c_str(), p_utcTime.length())

    // Sanity checks
    if (p_utcTime.length() < 23) {
        DEBUG_ERROR("CDS130X_I2C::SetTime: Wrong parameters")
        return false;
    }
    // Fill struct tm;
    struct tm t = {0};
    char wday[4] = {0};
    sscanf(p_utcTime.c_str(), "%s %2d %2d %2d:%2d:%2d %4d", /* Www MM dd hh:mm:ss yyyy, e.g. Thu 02 10 07:13:29 2011 */
          wday,
          &t.tm_mon,
          &t.tm_mday,
          &t.tm_hour,
          &t.tm_min,
          &t.tm_sec,
          &t.tm_year);
    DEBUG("CDS130X_I2C::SetTime: wday=%s - %d - %d - %d - %d - %d", wday, t.tm_mon, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec)

    t.tm_wday = (_dayOfWeek.find(wday) / 3) + 1;
    DEBUG("CDS130X_I2C::SetTime: wday=%s - _dayOfWeek:%s - tm_wday=%d", wday, _dayOfWeek.c_str(), t.tm_wday)

    Write(CDS130X_I2C::SecondsAddress, t.tm_sec, CDS130X_I2C::Binary);
    Write(CDS130X_I2C::MinutesAddress, t.tm_min, CDS130X_I2C::Binary);
    Write(CDS130X_I2C::HoursAddress, t.tm_hour, CDS130X_I2C::Binary);
    Write(CDS130X_I2C::DayOfWeekAddress, t.tm_wday, CDS130X_I2C::Binary);
    Write(CDS130X_I2C::DayAddress, t.tm_mday, CDS130X_I2C::Binary);
    Write(CDS130X_I2C::MonthAddress, t.tm_mon, CDS130X_I2C::Binary);
    Write(CDS130X_I2C::YearAddress, t.tm_year - 1900, CDS130X_I2C::Binary); // Years since 1900: 2011 becomes 111

    DEBUG_LEAVE("CDS130X_I2C::SetTime")
    return true;
} // End of method CDS130X_I2C::SetTime

struct tm CDS130X_I2C::GetTime() {
    DEBUG_ENTER("CDS130X_I2C::GetTime")

    struct tm t = {0};
    unsigned char value;
    // Setup time structure from RTC
    Read(CDS130X_I2C::SecondsAddress, &value, CDS130X_I2C::Binary);
    t.tm_sec = (int)value;
    Read(CDS130X_I2C::MinutesAddress, &value, CDS130X_I2C::Binary);
    t.tm_min = (int)value;
    Read(CDS130X_I2C::HoursAddress, &value, CDS130X_I2C::Binary);
    t.tm_hour = (int)value;
    Read(CDS130X_I2C::DayOfWeekAddress, &value, CDS130X_I2C::Binary);
    t.tm_wday = (int)value - 1;
    Read(CDS130X_I2C::DayAddress, &value, CDS130X_I2C::Binary);
    t.tm_mday = (int)value;
    Read(CDS130X_I2C::MonthAddress, &value, CDS130X_I2C::Binary);
    t.tm_mon = (int)value;
    Read(CDS130X_I2C::YearAddress, &value, CDS130X_I2C::Binary);
    t.tm_year = (int)value; // Years since 1900: 111 means 2011

    DEBUG_LEAVE("CDS130X_I2C::GetTime")
    return t;
} // End of method CDS130X_I2C::GetTime

bool CDS130X_I2C::EraseMemoryArea(const unsigned char p_startAddress, const int p_count, const unsigned char p_pattern) {
    DEBUG_ENTER("CDS130X_I2C::EraseMemoryArea): 0x%02x - %d - 0x%02x", p_startAddress, p_count, p_pattern)

    std::vector<unsigned char> eraseBuffer(p_count, p_pattern);
    return WriteMemory(p_startAddress, eraseBuffer, false);
}

bool CDS130X_I2C::WriteMemory(const unsigned char p_address, const unsigned char p_byte) {
    DEBUG_ENTER("CDS130X_I2C::WriteMemory (byte): Memory address: 0x%02x", p_address)

    // 1.Prepare buffer
    char i2cBuffer[2]; // Memory address + one byte of data
    // 1.1. Memory address
    i2cBuffer[0] = CDS130X_I2C::BaseMemoryAddress + p_address;
    DEBUG("CDS130X_I2C::WriteMemory (byte): pI2CBuffer[0]: 0x%02x", i2cBuffer[0])
    // 1.2. Datas
    i2cBuffer[1] = p_byte;
    DEBUG("CDS130X_I2C::WriteMemory (byte): value=0x%02x", i2cBuffer[1])

    // 2. Send I2C start + I2C address + Memory Address + Datas + I2C stop
    int result = write(_slaveAddress, i2cBuffer, 2);
    wait(0.02);

    DEBUG_LEAVE("CDS130X_I2C::WriteMemory (byte) %x", (bool)(result == 0))
    return (bool)(result == 0);
} // End of method CDS130X_I2C::WriteMemory

bool CDS130X_I2C::WriteMemory(const unsigned char p_address, const short p_short, const CDS130X_I2C::Mode p_mode) {
    DEBUG_ENTER("CDS130X_I2C::WriteMemory (short): Memory address:0x%02x, p_short:%04x, Mode:%d", p_address, p_short, p_mode)

    // 1.Prepare buffer
    char i2cBuffer[3]; // Memory address + one short (2 bytes)
    // 1.1. Memory address
    i2cBuffer[0] = CDS130X_I2C::BaseMemoryAddress + p_address;
    DEBUG("CDS130X_I2C::WriteMemory (short): pI2CBuffer[0]: 0x%02x", i2cBuffer[0])
    // 1.2. Datas
    if (p_mode == BigEndian) {
        i2cBuffer[1] = (unsigned char)(p_short >> 8);
        i2cBuffer[2] = (unsigned char)((unsigned char)p_short & 0xff);
    } else {
        i2cBuffer[1] = (unsigned char)((unsigned char)p_short & 0xff);
        i2cBuffer[2] = (unsigned char)(p_short >> 8);
    }
    DEBUG("CDS130X_I2C::WriteMemory (short): value=0x%02x%02x", i2cBuffer[1], i2cBuffer[2])

    // 2. Send I2C start + I2C address + Memory Address + Datas + I2C stop
    int result = write(_slaveAddress, i2cBuffer, 3);
    wait(0.02);

    DEBUG_LEAVE("CDS130X_I2C::WriteMemory (short) %x", (bool)(result == 0))
    return (bool)(result == 0);
} // End of method CDS130X_I2C::WriteMemory

bool CDS130X_I2C::WriteMemory(const unsigned char p_address, const int p_int, const CDS130X_I2C::Mode p_mode) {
    DEBUG_ENTER("CDS130X_I2C::WriteMemory (int): Memory address:0x%02x, p_int:%d, Mode:%d", p_address, p_int, p_mode)

    // 1.Prepare buffer
    char i2cBuffer[5]; // Memory address + one integer (4 bytes)
    // 1.1. Memory address
    i2cBuffer[0] = CDS130X_I2C::BaseMemoryAddress + p_address;
    DEBUG("CDS130X_I2C::WriteMemory (int): pI2CBuffer[0]: 0x%02x", i2cBuffer[0])
    // 1.2. Datas
    if (p_mode == BigEndian) {
        i2cBuffer[1] = (unsigned char)(p_int >> 24);
        i2cBuffer[2] = (unsigned char)(p_int >> 16);
        i2cBuffer[3] = (unsigned char)(p_int >> 8);
        i2cBuffer[4] = (unsigned char)((unsigned char)p_int & 0xff);
    } else {
        i2cBuffer[1] = (unsigned char)((unsigned char)p_int & 0xff);
        i2cBuffer[2] = (unsigned char)(p_int >> 8);
        i2cBuffer[3] = (unsigned char)(p_int >> 16);
        i2cBuffer[4] = (unsigned char)(p_int >> 24);
    }
    DEBUG("CDS130X_I2C::WriteMemory (int): value=0x%02x%02x%02x%02x", i2cBuffer[1], i2cBuffer[2], i2cBuffer[3], i2cBuffer[4])

    // 2. Send I2C start + I2C address + Memory Address + Datas + I2C stop
    int result = write(_slaveAddress, i2cBuffer, 5);
    wait(0.02);

    DEBUG_LEAVE("CDS130X_I2C::WriteMemory (int) %x", (bool)(result == 0))
    return (bool)(result == 0);
} // End of method CDS130X_I2C::WriteMemory

bool CDS130X_I2C::WriteMemory(const unsigned char p_address, const std::string & p_string, const bool p_storeLength, const int p_length2write) {
    DEBUG_ENTER("CDS130X_I2C::WriteMemory (std::string)")
    return WriteMemory(p_address, p_string.c_str(), p_storeLength, p_length2write);
} // End of method CDS130X_I2C::WriteMemory

bool CDS130X_I2C::WriteMemory(const unsigned char p_address, const std::vector<unsigned char> & p_datas, const bool p_storeLength, const int p_length2write) {
    DEBUG_ENTER("CDS130X_I2C::WriteMemory (std::vector)")

    int length = (p_length2write == -1) ? p_datas.size() : p_length2write;
    unsigned char array[length];
    std::copy(p_datas.begin(), p_datas.end(), array);
    bool result = WriteMemory(p_address, array, p_storeLength, length);
    wait(0.02);

    DEBUG_LEAVE("CDS130X_I2C::WriteMemory (std::vector): %d", result)
    return result;
} // End of method CDS130X_I2C::WriteMemory

bool CDS130X_I2C::WriteMemory(const unsigned char p_address, const char *p_datas, const bool p_storeLength, const int p_length2write) {
    DEBUG_ENTER("CDS130X_I2C::WriteMemory (char *): Memory address: 0x%02x - %x - %d", p_address, p_storeLength, p_length2write)

    // 1.Prepare buffer
    int length = (p_length2write == -1) ? strlen(p_datas) : p_length2write;
    if (p_storeLength) {
        length += 1; // Add one byte for the length
    }
    DEBUG("CDS130X_I2C::WriteMemory (char *): length:%d", length)

    char i2cBuffer[1 + length];
    // 1.1. Memory address
    i2cBuffer[0] = CDS130X_I2C::BaseMemoryAddress + p_address;
    DEBUG("CDS130X_I2C::WriteMemory (char *): pI2CBuffer[0]: 0x%02x", i2cBuffer[0])
    // 1.2. Datas
    if (p_storeLength) {
        // Fill the length
        i2cBuffer[1] = (unsigned char)length;
        for (int i = 0; i < length; i++) {
            i2cBuffer[2 + i] = *(p_datas + i);
        }
    } else { // The length was not stored
        for (int i = 0; i < length; i++) {
            i2cBuffer[1 + i] = *(p_datas + i);
        }
    }

    // 2. Send I2C start + I2C address + Memory Address + Datas + I2C stop
    int result = write(_slaveAddress, i2cBuffer, 1 + length);
    wait(0.02);

    DEBUG_LEAVE("CDS130X_I2C::WriteMemory (char *) %x", (bool)(result == 0))
    return (bool)(result == 0);
} // End of method CDS130X_I2C::WriteMemory

bool CDS130X_I2C::WriteMemory(const unsigned char p_address, const unsigned char *p_datas, const bool p_storeLength, const int p_length2write) {
    DEBUG_ENTER("CDS130X_I2C::WriteMemory (byte *): Memory address: 0x%02x", p_address, p_storeLength, p_length2write)
    return WriteMemory(p_address, (const char *)p_datas, p_storeLength, p_length2write);
} // End of method CDS130X_I2C::WriteMemory

bool CDS130X_I2C::ReadMemory(const unsigned char p_address, unsigned char * p_byte) {
    DEBUG_ENTER("CDS130X_I2C::ReadMemory (byte): Memory address:0x%02x", p_address)

    // 1.Prepare buffer
    char i2cBuffer[1];
    // 1.1. Memory address
    i2cBuffer[0] = CDS130X_I2C::BaseMemoryAddress + p_address;
    DEBUG("CDS130X_I2C::ReadMemory (byte): pI2CBuffer[0]: 0x%02x", i2cBuffer[0])

    // 2. Send I2C start + memory address
    if (write(_slaveAddress, i2cBuffer, 1, true) == 0) {
        // 2. Read data + I2C stop
        int result = read(_slaveAddress, (char *)p_byte, 1);
        wait(0.02);

        DEBUG_LEAVE("CDS130X_I2C::ReadMemory (byte): %x", (bool)(result == 0))
        return (bool)(result == 0);
    }

    DEBUG_LEAVE("CDS130X_I2C::ReadMemory (byte) (false)")
    return false;
} // End of method CDS130X_I2C::ReadMemory

bool CDS130X_I2C::ReadMemory(const unsigned char p_address, short *p_short, const CDS130X_I2C::Mode p_mode) {
    DEBUG_ENTER("CDS130X_I2C::ReadMemory (short): Memory address:0x%02x, Mode:%d", p_address, p_mode)

    // 1.Prepare buffer
    char i2cBuffer[2];
    // 1.1. Memory address
    i2cBuffer[0] = CDS130X_I2C::BaseMemoryAddress + p_address;
    DEBUG("CDS130X_I2C::ReadMemory (short): pI2CBuffer[0]: 0x%02x", i2cBuffer[0])

    // 2. Send I2C start + memory address
    if (write(_slaveAddress, i2cBuffer, 1, true) == 0) {
        // 2. Read data + I2C stop
        int result = read(_slaveAddress, i2cBuffer, 2);
        wait(0.02);
        if (p_mode ==  BigEndian) {
            *p_short = (short)(i2cBuffer[0] << 8 | i2cBuffer[1]);
        } else {
            *p_short = (short)(i2cBuffer[1] << 8 | i2cBuffer[0]);
        }

        DEBUG_LEAVE("CDS130X_I2C::ReadMemory (short): %x", (bool)(result == 0))
        return (bool)(result == 0);
    }

    DEBUG_LEAVE("CDS130X_I2C::ReadMemory (short) (false)")
    return false;
} // End of method CDS130X_I2C::ReadMemory

bool CDS130X_I2C::ReadMemory(const unsigned char p_address, int *p_int, const CDS130X_I2C::Mode p_mode) {
    DEBUG_ENTER("CDS130X_I2C::ReadMemory (int): Memory address:0x%02x, Mode:%d", p_address, p_mode)

    // 1.Prepare buffer
    char i2cBuffer[4];
    // 1.1. Memory address
    i2cBuffer[0] = CDS130X_I2C::BaseMemoryAddress + p_address;
    DEBUG("CDS130X_I2C::ReadMemory (int): pI2CBuffer[0]: 0x%02x", i2cBuffer[0])

    // 2. Send I2C start + memory address
    if (write(_slaveAddress, i2cBuffer, 1, true) == 0) {
        // 2. Read data + I2C stop
        int result = read(_slaveAddress, i2cBuffer, 4);
        wait(0.02);
        if (p_mode ==  BigEndian) {
            *p_int = (int)(i2cBuffer[0] << 24 | i2cBuffer[1] << 16 | i2cBuffer[2] << 8 | i2cBuffer[3]);
        } else {
            *p_int = (int)(i2cBuffer[3] << 24 | i2cBuffer[2] << 16 | i2cBuffer[1] << 8 | i2cBuffer[0]);
        }

        DEBUG_LEAVE("CDS130X_I2C::ReadMemory (int): %x", (bool)(result == 0))
        return (bool)(result == 0);
    }

    DEBUG_LEAVE("CDS130X_I2C::ReadMemory (int) (false)")
    return false;
} // End of method CDS130X_I2C::ReadMemory

bool CDS130X_I2C::ReadMemory(const unsigned char p_address, std::vector<unsigned char> & p_datas, const bool p_readLengthFirst, const int p_length2write) {
    DEBUG_ENTER("CDS130X_I2C::ReadMemory (vector): Memory address:0x%02x, readLength:%01x, Length:%d", p_address, p_readLengthFirst, p_length2write)

    // 1.Prepare buffer
    unsigned char address = CDS130X_I2C::BaseMemoryAddress + p_address;
    unsigned char length;
    if (p_readLengthFirst) {
        ReadMemory(address, (unsigned char *)&length); // Read the length in big endian mode
        address += 1; // Skip the length value
        length -= 1; // length is the size of (string length + string)
    } else {
        if (p_length2write == -1) {
            length = p_datas.size();
        } else {
            length = p_length2write;
        }
    }

    // 2. Memory address
    char i2cBuffer[1];
    i2cBuffer[0] = address;
    DEBUG("CDS130X_I2C::ReadMemory (vector): pI2CBuffer[0]: 0x%02x", i2cBuffer[0])

    // 3. Send I2C start + memory address
    if (write(_slaveAddress, i2cBuffer, 1, true) == 0) {
        // 4. read data + I2C stop
        unsigned char buffer[length];
        int result = read(_slaveAddress, (char *)buffer, length);
        wait(0.02);
        if (result == 0) {
            p_datas.assign(buffer, buffer + length);

            DEBUG_LEAVE("CDS130X_I2C::ReadMemory (vector): %x", (bool)(result == 0))
            return (bool)(result == 0);
        }
    }

    DEBUG_LEAVE("CDS130X_I2C::ReadMemory (vector) (false)")
    return false;
} // End of method CDS130X_I2C::ReadMemory

bool CDS130X_I2C::ReadMemory(const unsigned char p_address, std::string & p_string, const bool p_readLengthFirst, const int p_length2write) {
    DEBUG_ENTER("CDS130X_I2C::ReadMemory (string): Memory address:0x%02x, readLength:%x, Length:%d", p_address, p_readLengthFirst, p_length2write)

    // 1.Prepare buffer
    unsigned char address = CDS130X_I2C::BaseMemoryAddress + p_address;
    unsigned char length;
    if (p_readLengthFirst) { // The string was stored with its length
        if (!ReadMemory(p_address, (unsigned char *)&length)) { // Read the length in big endian mode
            DEBUG_ERROR("CDS130X_I2C::ReadMemory (string): Failed to read length")
            return false;
        }
        wait(0.02);
        address += 1; // Skip the length value size
        length -= 1; // length is the size of (string length + string)
    } else { // The string length is provided by p_length2write parameter
        if (p_length2write == -1) {
            length = p_string.size();
        } else {
            length = p_length2write;
            p_string.resize(p_length2write);
        }
    }
    DEBUG("CDS130X_I2C::ReadMemory (string): Length=%d", length)

    // 2. Memory address
    char i2cBuffer[1];
    i2cBuffer[0] = address;
    DEBUG("CDS130X_I2C::ReadMemory (string): pI2CBuffer[0]: 0x%02x", i2cBuffer[0])

    // 3. Send I2C start + memory address
    if (write(_slaveAddress, i2cBuffer, 1, true) == 0) {
        // 4. Read data + I2C stop
        char buffer[length];
        int result = read(_slaveAddress, (char *)buffer, length);
        DEBUG_LEAVE("CDS130X_I2C::ReadMemory (string): %d", result)
        if (result == 0) {
            p_string.assign(buffer, length);

            return true;
        }
    }

    DEBUG_LEAVE("CDS130X_I2C::ReadMemory (string) (false)")
    return false;
} // End of method CDS130X_I2C::ReadMemory

#if defined(__DEBUG)
void CDS130X_I2C::DumpMemoryArea(const unsigned char p_address, const int p_count) {
    DEBUG_ENTER("CDS130X_I2C::DumpMemoryArea: %02x - %d", p_address, p_count)

    DEBUG("CDS130X_I2C::DumpMemoryArea: Reading datas...");
    std::vector<unsigned char> datas(p_count);
    if (!ReadMemory(p_address, datas, false)) { // Read bytes, including the lenght indication, buffer size is not set before the call
#ifdef __DEBUG
        //DEBUG(_FATAL("CDS130X_I2C::DumpMemoryArea: read failed")
#else // __DEBUG
        std::cout << "CDS130X_I2C::DumpMemoryArea: read failed\r" << std::endl
#endif // __DEBUG
    } else {
        std::cout << "CDS130X_I2C::DumpMemoryArea: Read bytes:\r" << std::endl;
        HEXADUMP(&datas[0], p_count);
        std::cout << "\r" << std::endl;
    }
} // End of method CDS130X_I2C::DumpMemoryArea
#endif // _DEBUG

} // End of namespace DS130X_I2C
