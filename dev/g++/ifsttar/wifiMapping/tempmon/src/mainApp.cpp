#include "mainApp.h"

#include <iomanip>
#include <sstream> // std::ostringstream
#include <cstring> // Used std::strerror, memcpy

#include <unistd.h>

#include "beagleboneCommEx.h"

#include "beagleboneHwInit.h" // Used to initialize the wiringBbb

#include "ipcCommon.h" // IPC constants 

#include "hwconfig.h" // Hardware configuration

namespace tempmon {

  mainApp::mainApp() : _smMgr(), _temp(nullptr), _pwm(FAN_PWM), _currentTemp(0.0), _dutyCycle(0), _samplePeriod(15.0)/*15 seconds*/, _values(2) {
    std::clog << ">>> tempmon::mainApp::mainApp" << std::endl;

    // Open the shared memory
    if (_smMgr.open(TEMP_SEGMENT_ID) != 0) {
      std::ostringstream os;
      os << "mainApp::initialize: Failed to open the shared memory: " << std::strerror(errno) << std::endl;
      throw new beagleboneCommEx(os.str());
    }

    // Initialize the wiringBbb
    beagleboneHwInit::getInstance().initialize();
    
    // Setup PWM
    ::pinMode(_pwm, PWM_OUTPUT);
    ::pwmSetMode(PWM_MODE_MS);
    ::pwmWrite(_pwm, _dutyCycle); // Duty cycle = 100%

    // Setup temperature sensor
    _temp.reset(new mcp980x_i2c(0x00)); // <A2,A0>=000b - i2cdetect -y 1 returns 0x48
    _temp->setAlertTemp(FAN_TEMP_SHUTDOWN);
  }

  mainApp::~mainApp() {
    std::clog << ">>> tempmon::mainApp::~mainApp" << std::endl;
    _smMgr.close();
    _temp.reset(nullptr);
  }

  void mainApp::stop() {
    std::clog << ">>> tempmon::mainApp::stop" << std::endl;
    runnable::stop();
  }

  void mainApp::run() {
    std::clog << ">>> tempmon::mainApp::run" << std::endl;

    _running = true;
    while(_running == true) {
      // Read temp
      _currentTemp = _temp->readTemp();
      //      std::clog << "tempmon::mainApp: ambiant temperature: " << std::setprecision(5) << (float)_currentTemp << std::endl;
      // Process FAN 
      processFan();
      // Store current temperature
      storeData();

      int i = 0; // TODO Enhance sleep interruption handling
      do { 
	sleep(1);
	i += 1;
      } while ((i < _samplePeriod) && (_running == true));
    } // End of 'while' statement

    std::clog << "<<< tempmon::mainApp::run" << std::endl;
  }

  void mainApp::processFan() {
    //    std::clog << ">>> tempmon::mainApp::processFan: " << (unsigned int)_dutyCycle << std::endl;

    float step = 10.0 * (_currentTemp - FAN_TEMP_SHUTDOWN) / 2.0/* change into parameter */;
    //    std::clog << "tempmon::mainApp::processFan: step=" << step << std::endl;
    if (step > 0.0) {
      _dutyCycle = (_dutyCycle + static_cast<unsigned char>(step)) % 100;
      std::clog << "tempmon::mainApp::processFan: 1/ new duty=" << (unsigned int)_dutyCycle << std::endl;
      ::pwmWrite(_pwm, _dutyCycle);
    } else if (_dutyCycle != 0) {
      _dutyCycle -= step;
      if (_dutyCycle > 100) { // Substr overflow
	_dutyCycle = 0;
      }
      std::clog << "tempmon::mainApp::processFan: 2/ new duty=" << (unsigned int)_dutyCycle << std::endl;
      ::pwmWrite(_pwm, _dutyCycle);
    }
  }

  void mainApp::storeData() {
    //    std::clog << ">>> tempmon::mainApp::storeData" << std::endl;

    // Build data array
    _values[0] = _currentTemp;
    _values[1] = static_cast<float>(_dutyCycle);
    
    // Convert float into bytes array
    float *pfloat = _values.data();
    unsigned char *buffer = reinterpret_cast<unsigned char *>(pfloat);

    // Reserve space into the buffer
    std::vector<unsigned char> result(1 + _values.size() * sizeof(float), 0x00);
    result[0] = _values.size(); // # of items
    std::copy(buffer, buffer + _values.size() * sizeof(float), result.begin() + 1); // Values    

    // Store in shared memory
    if (_smMgr.write(result, result.size()) < 0) {
      std::cerr << "tempmon::mainApp::storeData: Failed to write into the shared memory: " << std::strerror(errno) << std::endl;
    }
  }

} // End of namespace tempmon
