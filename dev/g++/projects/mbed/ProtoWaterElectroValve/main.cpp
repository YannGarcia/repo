#include <iostream>
#include <iomanip>
#include <sstream>
#include <memory>

#include "EthernetInterface.h"

#include "Network/NTPClient.h"

#include "Hardware/HwConfig.h"
#include "Hardware/LedMgmt.h"
#include "Hardware/DS130x_I2C.h"
#include "Hardware/MCP23017_I2C.h"
#include "Hardware/MCP320x_SPI.h"

#include "UserCmdImpl.h"
#include "TasksMgtImpl.h"

#include "Converter.h"

static const char *g_ipv4Address = "192.168.101.1";

static const char *g_ipv4GwAddress = "192.168.101.2";

static const char *g_ipv4Mask = "255.255.255.0";

static bool g_useDhcp = true;

/** Ethernet pointer accessor.
    NULL pointer means Ethernet access failures
 */
static std::unique_ptr<EthernetInterface> g_eth(nullptr);
/** NTP client */
static NTPClient g_ntp;
/** UDP server */
static std::unique_ptr<UDPSocket> g_udpServer(nullptr);
/** UDP listener port */
static int g_udpPort = 11000;
/** User end point address */
static Endpoint g_packetSender;
/** User command implementation */
static UserCmdImpl g_userCmdImpl;
/** Tasks management */
static TasksMgtImpl g_tasksMgtImpl;

/** RTC IC implementation */
static CDS130X_I2C g_rtc(I2C_RTCC_ADDRESS, I2C_SDA_PIN, I2C_SDL_PIN, CDS130X_I2C::One_Hz); // Create an instance of the class C24LCXX_I2C, p9: SDA, p10:SDL, Oscillator mode: 1Hz
/** GPIO Expender */
static CMCP23017_I2C g_gpioExpender(I2C_SDA_PIN, I2C_SDL_PIN, I2C_GPIO_ADDRESS, IOEXP_INTA, IOEXP_INTB, IOEXP_RESET);
/** ADC */
static CMCP320x_SPI g_adc(SPI_MOSI_PIN, SPI_MISO_PIN, SPI_SCK_PIN, CS_MCP32, &g_gpioExpender);

/** Power LED timer */
static Ticker g_powerIndicator;
/** Power LED timer callback */
static void PowerOnIndicator();
/** Power LED */
static LedMgmt g_powerOnLed(POWER_LED);

/** Tasks monitoring timer */
static Ticker g_processTasksMonitoring;
/** Tasks monitoring timer callback */
static void ProcessTasksMonitoring();
/** Debug trigger pin for task monitoring */
static LedMgmt g_taskMonitoringLed(LED2);

/** Flag to set to terminate infinite loop */ 
bool g_exit = false;

/** Initialize hardware */
static void InitHardware();
/** Uninitialize hardware */
static void UninitHardware();
/** Initialize network */
static int InitializeNetwork();

/** Check UDP command input */
static void ProcessUdpCommand();
/** Process LCD display */
static void ProcessLcdDisplay();

static void SetSystemTimeFromSNTP();
static void SetSystemTimeFromRtc();

int main() {
    // Set power on indicator
    g_powerOnLed.On();
   
    // Initialize hardware
    InitHardware();
    
    // Start task monitoring
    g_processTasksMonitoring.attach(&ProcessTasksMonitoring, 1.0);

    // System ready, indicate the begining of infinite loop flashing power on indicator
//    std::clog << "Starting flash...\r" << std::endl;
    g_powerIndicator.attach(&PowerOnIndicator, 1.0);
    // Start infinite loop
    while (!g_exit) {
//        std::clog << "Looping...\r" << std::endl;
        // Check UDP command input
        ProcessUdpCommand();
         
        // Display
        ProcessLcdDisplay();
    } // End of 'while' statement
//    std::clog << "Terminating...\r" << std::endl;
    
    // Uninitialize hardware
    UninitHardware();
    
    g_powerIndicator.detach();
    g_powerOnLed.Off();

    while (true);
} // End of 'main'

void PowerOnIndicator() {
    g_powerOnLed != g_powerOnLed;
} // End of PowerOnIndicator

void ProcessTasksMonitoring() {
    //DEBUG_ENTER("ProcessTasksMonitoring")
    
}

void InitHardware() {
    DEBUG_ENTER("InitHardware")
    
    // Initialize RTC IC and start SQWOUT clock
    if (!g_rtc.Initialize()) {
        error("RTC critical failure");
    }

    // Initialize Ethernet
    if (InitializeNetwork() != 0) {
        error("InitializeNetwork critical failure");
    }
        
    // Setup RTC
    bool setTimeDone = false;
    if (g_eth.get() != NULL) {
        // Get NTP time
        NTPResult result = g_ntp.setTime("ntp.unice.fr");
        if (result == NTP_OK) {
            // Force RTC time with new sytem time
            SetSystemTimeFromSNTP();
            setTimeDone = true;
        } // else nothing to to
    } // else nothing to to
    if (!setTimeDone) {
        // Use RTC to set system time
        SetSystemTimeFromRtc();
    }
    
    // Setup EEPROM memory
    if (g_tasksMgtImpl.initialize() == 0) {
        // Setup EVs
    }
    
    DEBUG_LEAVE("InitHardware")
}

int InitializeNetwork() {
    DEBUG_ENTER("InitializeNetwork")
    
    // Initialize the interface
    g_eth.reset(new EthernetInterface);
    int result;
    if (g_useDhcp == true) {
        result = g_eth->init();
    } else {
        DEBUG("InitializeNetwork: Call Ethernet init with IP address: %s\r", g_ipv4Address)
        result = g_eth->init(g_ipv4Address, g_ipv4Mask, g_ipv4GwAddress);
    }
    DEBUG("InitializeNetwork: Ethernet init returns %d\r", result);
    if (result < 0) { // Bring the interface up
        // Free pointer
        g_eth.reset();
        DEBUG_LEAVE("InitializeNetwork: -1")
        return -1;
    }
    // Connect the interface
    if (g_eth->connect() != 0) {
        std::cerr << "Ethernet connect failure\r" << std::endl;
        // Free pointer
        g_eth.reset();
        DEBUG_LEAVE("InitializeNetwork: -2")
        return -2;
    }
    Thread::wait(1); // Needed after startup, due to used of Ethernet.
    DEBUG("IP details: %s - %s\r", g_eth->getIPAddress(), g_eth->getMACAddress ())
    // Start UDP server
    g_udpServer.reset(new UDPSocket);
     if (g_udpServer->bind(g_udpPort) != 0) {
        std::cerr << "UDP socket binding failure\r" << std::endl;
        g_udpServer.reset(NULL);
        DEBUG_LEAVE("InitializeNetwork: -3")
        return -3;
    }       
    g_udpServer->set_blocking (false, 1000);

    DEBUG_LEAVE("InitializeNetwork: 0")
    return 0;    
}

void UninitHardware() {
    g_udpServer->close();
    g_udpServer.reset(NULL);
    g_eth->disconnect();
    g_eth.reset(NULL);
}

void ProcessUdpCommand() {
//    DEBUG_ENTER("ProcessUdpCommand")
    
    // Sanity check
    if (g_udpServer.get() == NULL) {
        std::cerr << "ProcessUdpCommand: UDP server not initialized\r" << std::endl;
        return;
    }

    // Read buffer
//    DEBUG("\r\nWait for packet...\r\n");
    std::vector<unsigned char> buffer(128, 0x00);
    int result = g_udpServer->receiveFrom(g_packetSender, reinterpret_cast<char *>(buffer.data()), 128); 
    if (result < 0) {
        std::cerr << "ProcessUdpCommand: receive failed: " << result << "\r" << std::endl;
        return;
    } else if (result == 0) {
        return;
    }
//    DEBUG("ProcessUdpCommand: Received %d bytes %s\r", result, CConverter::getInstance().time2string(g_rtc.GetTime()).c_str())
    buffer.resize(result);
    // Process the command
    result = 0;
    ostringstream os;
    std::unique_ptr<UserCmdDesc> & cmd = g_userCmdImpl.parse(buffer);
    if (cmd.get() == NULL) {
        os << "Unsupported command=";
        std::cerr << "ProcessUdpCommand: Failed to parse user command\r" << std::endl;
        result = -1;
    } else {
        os << cmd->getCommand() << "=";
        result = g_userCmdImpl.execute(cmd, g_tasksMgtImpl);
        if (result < 0) {
            std::cerr << "ProcessUdpCommand: Failed to execute user command: " << std::dec << result << "\r" << std::endl;
        }
    }
    // Send command confirm and result code
    if (result < 0) {
        os << "KO" << "," << result;
    } else if (result == 0) { // No additional data 
        os << "OK" << "," << result;
    } else {
        os << "OK" << "," << g_userCmdImpl.getTaskDescription();
    }
    DEBUG("ProcessUdpCommand: Send status %s\r", (char *)(os.str().c_str()))
    g_udpServer->sendTo(g_packetSender, (char *)(os.str().c_str()), os.str().length());
    // Free allocated objects    
    DEBUG("Free allocated objects\r")
    cmd.reset(nullptr);
    
//    DEBUG_LEAVE("ProcessUdpCommand")
}

void ProcessLcdDisplay() {
    //DEBUG_ENTER("ProcessLcdDisplay")
    
    //DEBUG_LEAVE("ProcessLcdDisplay")
}

void SetSystemTimeFromSNTP() {
    DEBUG_ENTER("SetSystemTimeFromSNTP")
    
    time_t seconds = time(NULL);
   

//    std::string str(ctime(&seconds)); // Formatted date/time: Www Mmm dd hh:mm:ss yyyy
    char buffer[32];
    strftime(buffer, 32, "%a %m %d %H:%M:%S %Y", std::localtime(&seconds)); /* Www Mmm dd hh:mm:ss yyyy */
    g_rtc.SetTime(std::string(buffer));
    g_rtc.RestartClock();

    DEBUG("SetSystemTimeFromSNTP: current time is %s\r", buffer);
    
    DEBUG_LEAVE("SetSystemTimeFromSNTP")
}

void SetSystemTimeFromRtc() {
    DEBUG_ENTER("SetSystemTimeFromRtc")
    
    struct tm t = g_rtc.GetTime();
    set_time(mktime(&t));
    DEBUG("SetSystemTimeFromRtc: set time: %d\r", (unsigned int)mktime(&t));
    
    DEBUG_LEAVE("SetSystemTimeFromRtc")
}
