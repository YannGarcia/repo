#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>

#include "TasksMgtImpl.h"

#include "Converter.h"

#include "Hardware/HwConfig.h"

#define EEPROM_MAGIC_NUMBER_SIZE        4  // 4 bytes
#define EEPROM_MAGIC_NUMBER_0           'Y'
#define EEPROM_MAGIC_NUMBER_1           'G'
#define EEPROM_MAGIC_NUMBER_2           'A'
#define EEPROM_MAGIC_NUMBER_3           '!'
#define BYTE_SIZE                       1  // 1 byte

// TODO Add timestamp of last access
#define EEPROM_MAGIC_NUMBER             0 
#define EEPROM_ADDRESS_BOARDSNUM        EEPROM_MAGIC_NUMBER             + EEPROM_MAGIC_NUMBER_SIZE
#define EEPROM_ADDRESS_TASKSNUM         EEPROM_ADDRESS_BOARDSNUM        + BYTE_SIZE
#define EEPROM_ADDRESS_TASKIDCOUNTER    EEPROM_ADDRESS_TASKSNUM         + BYTE_SIZE
#define EEPROM_ADDRESS_TASKOFFSET       EEPROM_ADDRESS_TASKIDCOUNTER    + BYTE_SIZE

unsigned char TasksMgtImpl::TaskSize = 0x0C;

TasksMgtImpl::TasksMgtImpl() : 
    _eeprom(I2C_SDA, I2C_SCL, I2C_EEPROM_ADDRESS, NC/*p5*/, 400000), // Create an instance of the class C24LCXX_I2C, p9/28: SDA, p10/29:SDL, p5: wired to WP input of 24LCxx, address: A3=0,A2=0,A1=1, on 3.3V I2C bus
    _boardsNum(0x01),
    _tasksNum(0x00),
    _taskIDCounter(0x00),
    _tasks()
    {
    } // End of Constructor

TasksMgtImpl::~TasksMgtImpl() {
} // End of Destructor

int TasksMgtImpl::initialize() {
    DEBUG_ENTER("TasksMgtImpl::initialize")
    
    // Retrieve the magic number 
    std::vector<unsigned char> magicNumber; 
    if (!_eeprom.Read(EEPROM_MAGIC_NUMBER, magicNumber, false, EEPROM_MAGIC_NUMBER_SIZE)) {
        DEBUG_LEAVE("TasksMgtImpl::initialize: -1")
        return -1;
    }
    // Check magic number value
    std::vector<unsigned char> expectedMagicNumber;
    {
        expectedMagicNumber.push_back(EEPROM_MAGIC_NUMBER_0);
        expectedMagicNumber.push_back(EEPROM_MAGIC_NUMBER_1);
        expectedMagicNumber.push_back(EEPROM_MAGIC_NUMBER_2);
        expectedMagicNumber.push_back(EEPROM_MAGIC_NUMBER_3);
    }
    bool checked = std::equal(expectedMagicNumber.begin(), expectedMagicNumber.end(), magicNumber.begin());
    DEBUG("TasksMgtImpl::initialize: checked=%s", (checked) ? "true" : "false");
    if (!checked) { // Memory is uninitialized
//        HEXADUMP(&expectedMagicNumber[0], expectedMagicNumber.size());
        // Write the correct magic number
        _eeprom.Write(EEPROM_MAGIC_NUMBER, expectedMagicNumber, false); // Do not store length
        // Initiaize default index
        _eeprom.Write(EEPROM_ADDRESS_BOARDSNUM, _boardsNum);
        _eeprom.Write(EEPROM_ADDRESS_TASKSNUM, _tasksNum);
        _eeprom.Write(EEPROM_ADDRESS_TASKIDCOUNTER, _taskIDCounter);
        
        DEBUG_LEAVE("TasksMgtImpl::initialize: -2")
        return -2;
    } // else, continue
    
    // Read contextual data
    loadContextualData();
    // Load EV tasks
    loadEvTasksData();
    
    DEBUG_LEAVE("TasksMgtImpl::initialize: 0")
    return 0;
}

void TasksMgtImpl::loadContextualData() {
    DEBUG_ENTER("TasksMgtImpl::loadContextualData")
    
    _eeprom.Read(EEPROM_ADDRESS_BOARDSNUM, &_boardsNum);
    _eeprom.Read(EEPROM_ADDRESS_TASKSNUM, &_tasksNum);
    _eeprom.Read(EEPROM_ADDRESS_TASKIDCOUNTER, &_taskIDCounter);
    DEBUG("TasksMgtImpl::loadContextualData: boardsNum(%02x) _tasksNum(%02x) _taskIDCounter(%02x)", _boardsNum, _tasksNum, _taskIDCounter)
    
    DEBUG_LEAVE("TasksMgtImpl::loadContextualData")
}

void TasksMgtImpl::loadEvTasksData() {
    DEBUG_ENTER("TasksMgtImpl::loadEvTasksData")
    
    if (_tasksNum != 0) {
        unsigned short address = EEPROM_ADDRESS_TASKOFFSET;
        TaskImpl *taskImpl = NULL;
        for (unsigned char taskCounter = 0; taskCounter < _tasksNum; taskCounter++) {
            std::vector<unsigned char> buffer(TasksMgtImpl::TaskSize, 0x00);
            // Read data from EEPROM
            _eeprom.Read(address, buffer, false);
            // Prepare address for the next read
            address += TasksMgtImpl::TaskSize;
            // Deserialize previously read data
            TaskImpl::deserialize(buffer, &taskImpl);
            // Process it
            if (taskImpl != NULL) { // Deserialization succeed
                // Add it to the map
                std::pair<std::map<const unsigned char, TaskImpl *>::iterator, bool> result = _tasks.insert(std::pair<const unsigned char, TaskImpl *>(taskImpl->getId(), taskImpl));
                if (result.second == false) {
                    std::cerr << "TasksMgtImpl::loadEvTasksData: Failed to insert task id " << static_cast<int>(taskImpl->getId()) << "\r" << std::endl;
                    std::cout << "TasksMgtImpl::loadEvTasksData: Task already exists with a value of " << result.first->second << "\r" << std::endl;
                    continue;
                }
            } // else, drop it
        } // End of 'for' statement
    } // else, nothing to do
    
    DEBUG_LEAVE("TasksMgtImpl::loadEvTasksData: %d", _tasks.size())
}

unsigned char TasksMgtImpl::getTaskId() {
    DEBUG_ENTER("TasksMgtImpl::getTaskId: '%d'", _taskIDCounter)
    
    _eeprom.Write(EEPROM_ADDRESS_TASKIDCOUNTER, _taskIDCounter++);
    return _taskIDCounter - 1;
}

int TasksMgtImpl::createTask(const std::unique_ptr<TaskImpl> & p_task) {
    DEBUG_ENTER("TasksMgtImpl::createTask: '%d'", static_cast<int>(p_task->getId()))
    
    DEBUG("TasksMgtImpl::createTask: Before: %d", _tasks.size())
    // Calculate the memory address to store the new task
    unsigned short address = EEPROM_ADDRESS_TASKOFFSET + (p_task->getId() * TasksMgtImpl::TaskSize);
    // Serialize the new task
    std::vector<unsigned char> buffer;
    TaskImpl::serialize(*p_task, buffer);
    // Write data to the EEPROM
    _eeprom.Write(address, buffer, false);
     // Update contextual datas
    _tasksNum += 1;
    _eeprom.Write(EEPROM_ADDRESS_TASKSNUM, _tasksNum);
    // Update the tasks list
    _tasks.insert(std::pair<const unsigned char, TaskImpl *>(p_task->getId(), p_task.get()));
    DEBUG("TasksMgtImpl::createTask: After: %d", _tasks.size())
    
//#if defined(__DEBUG)
//    _eeprom.DumpMemoryArea(0, 128);
//#endif // __DEBUG
    
    DEBUG_LEAVE("TasksMgtImpl::createTask: 0")
    return 0;
}

int TasksMgtImpl::updateTask(const std::unique_ptr<TaskImpl> & p_task) {
    DEBUG_ENTER("TasksMgtImpl::updateTask: '%d'", static_cast<int>(p_task->getId()))
    
    // Sanity check: find it in the list
    std::map<const unsigned char, TaskImpl *>::iterator result = _tasks.find(p_task->getId());
    if (result == _tasks.end()) { // Not found
        return createTask(p_task);
    } 
    // Replace the task    
    result->second = p_task.get();
    // Calculate the memory address to store the new task
    unsigned short address = EEPROM_ADDRESS_TASKOFFSET + (static_cast<unsigned short>(p_task->getId()) * TasksMgtImpl::TaskSize);
    // Serialize the new task
    std::vector<unsigned char> buffer;
    TaskImpl::serialize(*p_task, buffer);
    // Write data to the EEPROM
    _eeprom.Write(address, buffer, false);
    
#if defined(__DEBUG)
    _eeprom.DumpMemoryArea(0, 128);
#endif // __DEBUG
    
    DEBUG_LEAVE("TasksMgtImpl::updateTask: 0")
    return 0;
}

int TasksMgtImpl::deleteTask(const unsigned char p_taskId) {
    DEBUG_ENTER("TasksMgtImpl::deleteTask: '%d' '%d'", p_taskId, _tasks.size())
    
    // Sanity check
    std::map<const unsigned char, TaskImpl *>::iterator result = _tasks.find(p_taskId);
    if (result == _tasks.end()) { // Not found
        return -1;
    } 
    
    // Free resources and remove from the task list
    delete result->second;
    _tasks.erase(result);
    DEBUG("TasksMgtImpl::deleteTask: new size='%d'", _tasks.size())
    
    // Update the number of task
    _tasksNum -= 1;
    _eeprom.Write(EEPROM_ADDRESS_TASKSNUM, _tasksNum);
    DEBUG("TasksMgtImpl::deleteTask: new tasks number='%d'", _tasksNum)
   
    // Shift the task in memory
    unsigned short address = EEPROM_ADDRESS_TASKOFFSET;
    for (std::map<const unsigned char, TaskImpl *>::iterator it = _tasks.begin(); it != _tasks.end(); ++it) {
        // Serialize the new task
        std::vector<unsigned char> buffer;
        TaskImpl::serialize(*it->second, buffer);
        // Write data to the EEPROM
        _eeprom.Write(address, buffer, false);
        address += TasksMgtImpl::TaskSize;
    } // End of 'for' statement
    
    _eeprom.EraseMemoryArea(address, TasksMgtImpl::TaskSize, 0xFF);   
    
    DEBUG_LEAVE("TasksMgtImpl::deleteTask: 0")
    return 0;
}

int TasksMgtImpl::listTasks(iostream & p_output, const bool p_forceReload) {
    DEBUG_ENTER("TasksMgtImpl::listTasks: '%x' '%d'", static_cast<unsigned char>(p_forceReload), _tasks.size())
    
    // Sanity check
    if (_tasks.size() == 0) {
        return -1;
    }

    if (p_forceReload) {
        // Read contextual data
        loadContextualData();
        // Load EV tasks
        loadEvTasksData();
    }        
        
    p_output << "TaskId | Status | StartMonth | StartDay | StopMonth | StopDay | StartHour | StartMin | StopHour | StopMin | #Board | #Port" << "\r" << std::endl;
    for (std::map<const unsigned char, TaskImpl *>::iterator it = _tasks.begin(); it != _tasks.end(); ++it) {
        writeTask(*it->second, p_output);
    } // End of 'for' statement
    
    DEBUG_LEAVE("TasksMgtImpl::listTasks: 0")
    return 0;
}

int TasksMgtImpl::getTaskDescription(const unsigned char p_taskId, std::string & p_taskDescription) {
    DEBUG_ENTER("TasksMgtImpl::getTaskDescription: %d", p_taskId)
    
    // Sanity check
    std::map<const unsigned char, TaskImpl *>::iterator result = _tasks.find(p_taskId);
    if (result == _tasks.end()) { // Not found
        p_taskDescription.assign("");
        return -1;
    } 
    
    std::stringstream output;
    writeTask(*result->second, output);
    p_taskDescription.assign(output.str());
    DEBUG("TasksMgtImpl::getTaskDescription: %s", p_taskDescription.c_str())
    
    DEBUG_LEAVE("TasksMgtImpl::getTaskDescription: 0")
    return 0;
}

int TasksMgtImpl::getTaskStatus(const unsigned char p_taskId, std::string & p_taskDescription) {
    DEBUG_ENTER("TasksMgtImpl::getTaskStatus: %d", p_taskId)
    
    // Sanity check
    std::map<const unsigned char, TaskImpl *>::iterator result = _tasks.find(p_taskId);
    if (result == _tasks.end()) { // Not found
        return -1;
    } 
    p_taskDescription.assign(CConverter::getInstance().int2string(static_cast<int>(result->second->getMode())));
    
    DEBUG_LEAVE("TasksMgtImpl::getTaskStatus: 1")
    return 1;
}

int TasksMgtImpl::getForceTaskStart(const unsigned char p_taskId) {
    DEBUG_ENTER("TasksMgtImpl::getForceTaskStart: %d", p_taskId)
    
    // Sanity check
    std::map<const unsigned char, TaskImpl *>::iterator result = _tasks.find(p_taskId);
    if (result == _tasks.end()) { // Not found
        return -1;
    } 
    
    result->second->setMode(TaskImpl::PowerOn);
    
    DEBUG_LEAVE("TasksMgtImpl::getForceTaskStart: 0")
    return 0;
}

int TasksMgtImpl::getForceTaskStop(const unsigned char p_taskId) {
    DEBUG_ENTER("TasksMgtImpl::getForceTaskStop: %d", p_taskId)
    
    // Sanity check
    std::map<const unsigned char, TaskImpl *>::iterator result = _tasks.find(p_taskId);
    if (result == _tasks.end()) { // Not found
        return -1;
    } 
    
    result->second->setMode(TaskImpl::PowerOff);
    
    DEBUG_LEAVE("TasksMgtImpl::getForceTaskStop: 0")
    return 0;
}

int TasksMgtImpl::setBoardNumber(const unsigned char p_boardNumber) {
    DEBUG_ENTER("TasksMgtImpl::setBoardNumber: %d", p_boardNumber)
    
    _boardsNum = p_boardNumber;
    _eeprom.Write(EEPROM_ADDRESS_BOARDSNUM, _boardsNum);
    
    DEBUG_LEAVE("TasksMgtImpl::setBoardNumber: 0")
    return 0;
}

void TasksMgtImpl::forceMemoryDump(const short p_address, const short p_length) {
    DEBUG_ENTER("TasksMgtImpl::forceMemoryDump:")
    
    for (std::map<const unsigned char, TaskImpl *>::iterator it = _tasks.begin(); it != _tasks.end(); ++it)
        DEBUG(
            "TasksMgtImpl::forceMemoryDump: %d %02x %02x %02x %02x %02x %02x", 
            it->second->getId(),
            it->second->getStartDay(),
            it->second->getStopMonth(),
            it->second->getStopDay(),
            it->second->getStopDay(),
            it->second->getStartHour(),
            it->second->getStartMin()
       )
#if defined(__DEBUG)
    _eeprom.DumpMemoryArea(p_address, p_length);
#endif // __DEBUG

    DEBUG_LEAVE("TasksMgtImpl::forceMemoryDump")
}

void TasksMgtImpl::forceResetMemory() {
    _eeprom.EraseMemoryArea(0, 256, 0xFF);
    // Free resources
    for (std::map<const unsigned char, TaskImpl *>::iterator it = _tasks.begin(); it != _tasks.end(); ++it) {
        delete it->second;
    } // End of 'for' loop
    _tasks.clear();
    _boardsNum = 0x01;
    _tasksNum = 0x00;
    _taskIDCounter = 0x00;
    initialize(); 
}

void TasksMgtImpl::writeTask(const TaskImpl & p_task, iostream & p_output) {
    DEBUG_ENTER("TasksMgtImpl::writeTask: %d", p_task.getId())
    
    p_output << std::dec << 
        " " << static_cast<unsigned int>(p_task.getId()) << " | ";
    if (p_task.getMode() == TaskImpl::PowerOff) {
        p_output << "OFF" << " | ";
    } else {
        p_output << "ON " << " | ";
    }
    p_output << std::hex << std::setw(2) << std::setfill('0') << 
        static_cast<unsigned int>(p_task.getStartMonth()) << " | " << 
        static_cast<unsigned int>(p_task.getStartDay()) << " | " << 
        static_cast<unsigned int>(p_task.getStopMonth()) << " | " << 
        static_cast<unsigned int>(p_task.getStopDay()) << " | " << 
        static_cast<unsigned int>(p_task.getStopDay()) << " | " << 
        static_cast<unsigned int>(p_task.getStartHour()) << " | " << 
        static_cast<unsigned int>(p_task.getStartMin()) << " | " << 
        static_cast<unsigned int>(p_task.getStopHour()) << " | " << 
        static_cast<unsigned int>(p_task.getStopMin()) << " | " << 
        static_cast<unsigned int>(p_task.getBoardNum()) << " | " << 
        static_cast<unsigned int>(p_task.getPortNum()) << " | " << 
        " \r" << 
        std::endl;
    DEBUG_LEAVE("TasksMgtImpl::writeTask")
}
