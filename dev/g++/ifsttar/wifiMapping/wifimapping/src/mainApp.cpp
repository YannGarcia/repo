#include <iomanip>
#include <fstream> // Used std:ifstream
#include <sstream> // Used std::ostringstream
#include <cstring> // Used std::strerror, memcpy

#include <unistd.h>

#include <sys/types.h>
#include <signal.h>

#include "hwconfig.h"

#include "mainApp.h"

#include "beagleboneCommEx.h"

#include "converter.h"

//#include "dump.h" // TODO To be removed

namespace wifimapping {

  mainApp::mainApp() : _smMgr(), _parmMgr(), _dsMq(), _guiMq(), _adcMq(), _cfgMap(), _archive(), _checkIperf(), _checkGps(), _checkTemp(), _checkRtc(), _checkTshark(), _monitorProcess(), _samplePeriod(2e6/*2 seconds*/), _lcdTimeout(0), _autoMode(true) {
    std::clog << ">>> mainApp::mainApp" << std::endl;

    // Create the shared memory
    if (_smMgr.create() != 0) {
      std::ostringstream os;
      os << "mainApp::initialize: Failed to create the shared memory: " << std::strerror(errno) << std::endl;
      throw new beagleboneCommEx(os.str());
    }

    // Open parameters part. Only this process write inside. The other process just read parameters valuue
    if (_parmMgr.open(PARAMETER_SEGMENT_ID) != 0) {
      std::cerr << "checkTemp::initialize: Failed to  " << std::strerror(errno) << std::endl;
      std::ostringstream os;
      os << "mainApp::initialize: Failed to open the shared memory: " << std::strerror(errno) << std::endl;
      throw new beagleboneCommEx(os.str());
    }
    std::vector<unsigned char> buffer = converter::getInstance().float2bytes(_samplePeriod);
    if (_parmMgr.write(buffer, buffer.size()) < 0) {
      std::cerr << "mainApp::mainApp: Failed to write into the shared memory: " << std::strerror(errno) << std::endl;
    }
    
    // Open shared memory segment for Iperf-3.x
    if (_checkIperf.initialize() != 0) {
      std::ostringstream os;
      os << "mainApp::initialize: Failed to initialize Iperf share memory: " << std::strerror(errno) << std::endl;
      throw new beagleboneCommEx(os.str());
    }

    // Open shared memory segment for GPS
    if (_checkGps.initialize() != 0) {
      std::ostringstream os;
      os << "mainApp::initialize: Failed to initialize GPS share memory: " << std::strerror(errno) << std::endl;
      throw new beagleboneCommEx(os.str());
    }

    // Open shared memory segment for Temp/FAN
    if (_checkTemp.initialize() != 0) {
      std::ostringstream os;
      os << "mainApp::initialize: Failed to initialize Temp/FAN share memory: " << std::strerror(errno) << std::endl;
      throw new beagleboneCommEx(os.str());
    }

    // Open shared memory segment for RTC
    if (_checkRtc.initialize() != 0) {
      std::ostringstream os;
      os << "mainApp::initialize: Failed to initialize RTC share memory: " << std::strerror(errno) << std::endl;
      throw new beagleboneCommEx(os.str());
    }

    // Open shared memory segment for RTC
    if (_checkTshark.initialize() != 0) {
      std::ostringstream os;
      os << "mainApp::initialize: Failed to initialize Tshark share memory: " << std::strerror(errno) << std::endl;
      throw new beagleboneCommEx(os.str());
    }

    // Create the message queue for data storage
    if (_dsMq.create(mqMgr::dsMq) != 0) {
      std::ostringstream os;
      os << "mainApp::initialize: Failed to create the message queue: " << std::strerror(errno) << std::endl;
      throw new beagleboneCommEx(os.str());
    }

    // Open the message queue for GUI communication
    if (_guiMq.open(mqMgr::guiMq) != 0) { // Already created (_dsMq.create)
      std::ostringstream os;
      os << "mainApp::initialize: Failed to open the message queue: " << std::strerror(errno) << std::endl;
      throw new beagleboneCommEx(os.str());
    }

    // Open the message queue for ADC communication
    if (_adcMq.open(mqMgr::adcMq) != 0) { // Already created (_dsMq.create)
      std::ostringstream os;
      os << "mainApp::initialize: Failed to open the message queue: " << std::strerror(errno) << std::endl;
      throw new beagleboneCommEx(os.str());
    }

    // Load properties file
    loadPropertiesFile(std::string("/home/debian/etc/ifsttar.cfg"));
  } // End of Constructor

  mainApp::~mainApp() {
    std::clog << ">>> mainApp::~mainApp" << std::endl;

    _checkIperf.uninitialize();
    _checkGps.uninitialize();
    //_checkAcc.uninitialize();
    _checkTemp.uninitialize();
    _checkRtc.uninitialize();

    _parmMgr.close();
    _smMgr.close();
    _adcMq.close();
    _guiMq.close();
    _dsMq.close();
    _monitorProcess.clear(); // unique_ptr will delete processMonitor instances
  }

  int mainApp::initialize(const bool p_autoMode) {
    _autoMode = p_autoMode;

    // GUI monitor process
    _monitorProcess.push_back(std::unique_ptr<processMonitor>(new processMonitor));
    _monitorProcess[0]->prepare(_cfgMap, std::string("guimon"));

    int i = 1; // FIXME Use porperties file to create the correct number of instances
    // iPerf 3.0 monitor process
    _monitorProcess.push_back(std::unique_ptr<processMonitor>(new processMonitor));
    _monitorProcess[i++]->prepare(_cfgMap, std::string("iperfmon"));
    // Temperature/Fan monitor process
    _monitorProcess.push_back(std::unique_ptr<processMonitor>(new processMonitor));
    _monitorProcess[i++]->prepare(_cfgMap, std::string("tempmon"));
    // RTC monitor process
    _monitorProcess.push_back(std::unique_ptr<processMonitor>(new processMonitor));
    _monitorProcess[i++]->prepare(_cfgMap, std::string("rtcmon"));
    // GPS monitor process
    _monitorProcess.push_back(std::unique_ptr<processMonitor>(new processMonitor));
    _monitorProcess[i++]->prepare(_cfgMap, std::string("gpsmon"));
    // Tshark monitor process
    _monitorProcess.push_back(std::unique_ptr<processMonitor>(new processMonitor));
    _monitorProcess[i++]->prepare(_cfgMap, std::string("tsharkmon"));
    // TODO ACC monitor process
    
    // Data aggregation process
    _monitorProcess.push_back(std::unique_ptr<processMonitor>(new processMonitor));
    _monitorProcess[i++]->prepare(_cfgMap, std::string("aggreg"));

    return 0;
  }

  void mainApp::stop() {
    std::clog << ">>> mainApp::stop" << std::endl;
    // Terminate main loop
    runnable::stop();
  }

  void mainApp::run() {
    std::clog << ">>> mainApp::run" << std::endl;

    _running = true;

    // Start 'guimon'
    _monitorProcess[0]->execute();
    sleep(2);

    // Update GUI: Clear display, set curor (0,0) and print Ready
    std::vector<unsigned char> buffer(converter::getInstance().string2bytes(std::string("CLEAR;HOME;PRINT,Ready")));
    if (_guiMq.write(buffer, buffer.size()) < 0) {
      std::cerr << "mainApp::processGui: Failed to access GUI messqge queue " << std::strerror(errno) << std::endl;
    }
    
    if (_autoMode == false) {
      buffer = converter::getInstance().string2bytes(std::string("CLEAR;HOME;PRINT,Push button 1 to start"));
      if (_guiMq.write(buffer, buffer.size()) < 0) {
	std::cerr << "mainApp::processGui: Failed to access GUI messqge queue " << std::strerror(errno) << std::endl;
      }
      buffer.clear();
      do {
	std::vector<unsigned char> buffer;
	if (_adcMq.read(buffer, GUI_MQ_SIZE) > 0) {
	  if (buffer[0] == GUI_DEVICE_PHB_ID) {
	    std::clog << "mainApp::run: SW #" << (int)buffer[1] << " was pushed" << std::endl;
	    if (buffer[1] == PUSHBUTTON_1_PUSHED) {
	      break; // Terminate the waiting loop
	    }
	  } else {
	    //	std::cerr << "mainApp::processGui: Unsupported command: " << static_cast<int>(buffer[0]) << std::endl;
	  }
	}
	usleep(200000);
      } while (_running == true); // End of 'do-while' statement
      if (!_running) { // 'claos' method was called, exit
	std::clog << "<<< mainApp::run (before started)" << std::endl;
	return;
      } // else, continue
      // Update GUI: Clear displat, set curor (0,0) and print Ready
      buffer = converter::getInstance().string2bytes(std::string("CLEAR;HOME;PRINT,Ready"));
      if (_guiMq.write(buffer, buffer.size()) < 0) {
	std::cerr << "mainApp::processGui: Failed to access GUI messqge queue " << std::strerror(errno) << std::endl;
      }
    } // else, continue

    // Start children process
    for (auto it = _monitorProcess.begin() + 1/*Skip guimon*/; it != _monitorProcess.end(); ++it) {
      //      std::clog <<"mainApp::run: executing " << (*it)->toString() << std::endl;
      (*it)->execute();
    } // End of 'for' statement

    while(_running == true) {
      processGps(); // TODO Unify processXXX using checkData(enum GPS, Iperf...)
      processIperf();
      processRtc();
      processAcc();
      processTemp();
      processTshark();
      storeData();
      processGui();
      updateSamplePeriod();
      
      if (usleep(_samplePeriod) < 0) {
	std::cerr << "wifimapping::mainApp::run: 'usleep' failed" << std::strerror(errno) << std::endl;
      }
    } // End of 'while' statement

    // Stop children process
    for (auto it =  _monitorProcess.begin(); it != _monitorProcess.end(); ++it) {
      //      std::clog <<"mainApp::run: terminating " << (*it)->toString() << std::endl;
      (*it)->terminate();
    } // End of 'for' statement

    std::clog << "<<< mainApp::run" << std::endl;
  } // End of method run

  void mainApp::processGui() { 
    // Check inputs
    std::vector<unsigned char> buffer;
    if (_adcMq.read(buffer, GUI_MQ_SIZE) > 0) {
      if (buffer[0] == GUI_DEVICE_PHB_ID) {
	std::clog << "mainApp::processGui: SW #" << (int)buffer[1] << " was pushed" << std::endl;
	if ((buffer[1] == PUSHBUTTON_1_PUSHED) && (_autoMode == false)) {
	  std::clog << "mainApp::processGui: Terminate application with SIGTERM" << std::endl;
	  kill(getpid(), SIGTERM);
	}
      } else {
	//	std::cerr << "mainApp::processGui: Unsupported command: " << static_cast<int>(buffer[0]) << std::endl;
      }
    }

    // Process output
    if (_lcdTimeout > mainApp::LcdRefreshTimeout) { // TODO Use a constant
      _lcdTimeout = 0.0;
      // Update display
      buffer = converter::getInstance().string2bytes(std::string("CLEAR;HOME"));
      _guiMq.write(buffer, buffer.size());
      std::ostringstream os;
      int line = 0;
      // Network capabilities
      if (_checkIperf.getValues().size() != 0) {
	//	buffer.assign(_checkIperf.getValues().size(), 0x00);
	os << "PRINT,xfer=" << std::setprecision(2) << _checkIperf.getValues().at(0) << " MBit/s";
	os << ";SETCURSOR," << line++ << ",0;PRINT,Bwidth=" << std::setprecision(2) << _checkIperf.getValues().at(1) << " MBit/s";
	//	std::clog << std::string(os.str()) << std::endl;
	buffer = converter::getInstance().string2bytes(os.str());
	_guiMq.write(buffer, buffer.size());
      }
      os.str("");
      // Temperature
      if (_checkTemp.getValues().size() != 0) {
	//	buffer.assign(_checkTemp.getValues().size(), 0x00);
	os << "SETCURSOR," << line++ << ",0;PRINT,Temp=" << std::setprecision(2) << _checkTemp.getValues().at(0) << "(celsius)";
	//	std::clog << std::string(os.str()) << std::endl;
	buffer = converter::getInstance().string2bytes(os.str());
	_guiMq.write(buffer, buffer.size());
      }
      os.str("");
      // GPS
      if (_checkGps.getValues().size() != 0) {
	//	buffer.assign(_checkGps.getValues().size(), 0x00);
	os << "SETCURSOR," << line++ << ",0;PRINT,Lt=" << _checkGps.getValues().at(IDX_LATITUDE);
	//	std::clog << std::string(os.str()) << std::endl;
	buffer = converter::getInstance().string2bytes(os.str());
	_guiMq.write(buffer, buffer.size());
	os.str("");
	os << "SETCURSOR," << line++ << ",0;PRINT,Lg=" << _checkGps.getValues().at(IDX_LONGITUDE);
	//	std::clog << std::string(os.str()) << std::endl;
	buffer = converter::getInstance().string2bytes(os.str());
	_guiMq.write(buffer, buffer.size());
	os.str("");
	os << "SETCURSOR," << line++ << ",0;PRINT,s=" << _checkGps.getValues().at(IDX_SPEED) << " - e=" << _checkGps.getValues().at(IDX_ALTITUDE);
	//	std::clog << std::string(os.str()) << std::endl;
	buffer = converter::getInstance().string2bytes(os.str());
	if (_guiMq.write(buffer, buffer.size()) < 0) {
	  std::cerr << "mainApp::processGui: Failed to access GUI messqge queue " << std::strerror(errno) << std::endl;
	}
      }
      // TODO To be continued
    }
  }

  void mainApp::updateSamplePeriod() {
    _lcdTimeout += _samplePeriod;
    float samplePeriod = _samplePeriod;
    float speed = 0.0;
    try {
      speed = std::stof(_checkGps.getValues()[IDX_SPEED]) * 1.852/*knots to km/h*/;
      } catch (std::invalid_argument & i) {
      }
    //    std::clog << "mainApp::updateSamplePeriod: speed=" << speed << std::endl;
    if (speed < 1.852/*km/h*/) {
      _samplePeriod = 10e6; // 10 seconds
    } else {
      _samplePeriod = 60 * 0.01/*10meters*/ * 1e6/*µs*/ / speed;
      std::clog << "mainApp::updateSamplePeriod: new _samplePeriod=" << _samplePeriod << std::endl;
    }
    if (abs(_samplePeriod - samplePeriod) > 0.001) {
      std::vector<unsigned char> buffer = converter::getInstance().float2bytes(_samplePeriod);
      if (_parmMgr.write(buffer, buffer.size()) < 0) {
	std::cerr << "mainApp::updateSamplePeriod: Failed to write into the shared memory: " << std::strerror(errno) << std::endl;
      }
    }
  }

  int mainApp::storeData() {
    //    std::cout << _checkIperf.toString() << std::endl;
    // Clear it
    _archive.clear();
    // Fill it
    _checkIperf.serialize(_archive);
    _checkGps.serialize(_archive);
    //_checkAcc.serialize(_archive);
    _checkTemp.serialize(_archive);
    _checkRtc.serialize(_archive);
    // The last record is the tshark
    _checkTshark.serialize(_archive);

    if (_archive.toBytes().size() != 0) { // There is some data to store
      //      std::clog << "mainApp::storeData: send " << _archive.toBytes().size() << " bytes" << std::endl;
      //      dump::hexaDump(_archive.toBytes().data(), _archive.toBytes().size());
      // Send archive
      if (_dsMq.write(_archive.toBytes(), _archive.toBytes().size()) < 0) {
	std::cerr << "mainApp::storeData: Failed to archive data" << std::endl;
	return -1;
      }
    }

    return 0;
  } // End of method storeData

  void mainApp::loadPropertiesFile(const std::string & p_cfgFile) {
    // Sanity check
    if (p_cfgFile.empty()) {
      std::cerr << "mainApp::loadPropertiesFile: Key not found" << std::endl;
      return;
    }
    std::ifstream propertiesFile;
    propertiesFile.open(p_cfgFile);
    if (propertiesFile.is_open()) {
      std::string line;
      while (getline(propertiesFile, line)) {
	line = converter::getInstance().trim(line);
	if (line.empty()) {
	  continue; // Skip it
	} else if (line[0] == '#') { // Commented line
	  continue; // Skip it
	}
	_cfgMap[converter::getInstance().trim(line.substr(0, line.find('=')))] = converter::getInstance().trim(line.substr(line.find('=') + 1));
      } // End of 'while' statement
      std::clog << "mainApp::loadPropertiesFile: Stored " << _cfgMap.size() << " properties" << std::endl;
    } else {
      std::cerr << "mainApp::loadPropertiesFile: Failed to open properties file: " << std::strerror(errno) << std::endl;
    }
    propertiesFile.close();
  }

} // End of namespace wifimapping
