#pragma once

#include <map>
#include <memory> // Used for std::unique_ptr

#include "Hardware/24LCxx_I2C.h"

#include "TaskImpl.h"

/**
 *
 * ORG EEPROM_ADDRESS        Contains the tasks descriptions. Rate/time are in BCD format
 *      0x00                            Number supported boards
 *      0x01                            Number of items
 *      0x02                            TaskID counter
 *      0x03                            Task identifier
 *      0x04                            Start month in BCD format
 *      0x05                            Start day in BCD format
 *      0x06                            Stop month in BCD format
 *      0x07                            Stop day in BCD format
 *      0x08                            Start hour in BCD format
 *      0x09                            Start minutes in BCD format
 *      0x0A                            Stop hour in BCD format
 *      0x0B                            Stop minute in BCD format
 *      0x0C                            External conditions code
 *      0x0D                            Repeatition code 
 *      0x0E                            Status/Port/Board address
 *
 */
class TasksMgtImpl {
    static unsigned char TaskSize;

    /** Microchip EEP;ROM IC implementation */
    C24LCXX_I2C _eeprom;
    /** Number supported boards */
    unsigned char _boardsNum;
    /** Number of items */
    unsigned char _tasksNum;
    /** TaskID counter */
    unsigned char _taskIDCounter;
    /** The map of couple (start time, TaskImpl) */
    std::map<const unsigned char, TaskImpl *> _tasks;
    
public:
    TasksMgtImpl();
    virtual ~TasksMgtImpl();
    
    /** 
     * @desc Initialize tasks list from EEPROM
     */
    int initialize();
    unsigned char getTaskId();
    int createTask(const std::unique_ptr<TaskImpl> & p_task);
    int updateTask(const std::unique_ptr<TaskImpl> &  p_task);
    int deleteTask(const unsigned char p_taskId);
    int listTasks(iostream & p_output, const bool p_forceReload = false);
    int getTaskDescription(const unsigned char p_taskId, std::string & p_taskDescription);
    int getTaskStatus(const unsigned char p_taskId, std::string & p_taskDescription);
    int getForceTaskStart(const unsigned char p_taskId);
    int getForceTaskStop(const unsigned char p_taskId);
    int setBoardNumber(const unsigned char p_boardNumber);
    /**
     * @desc Force memory dump
     * @remark __DEBUG flag shall be defined
     */
    void forceMemoryDump(const short p_address = 0, const short p_length = 128);
    void forceResetMemory();
    
private:
    void loadContextualData();
    void loadEvTasksData();
    void writeTask(const TaskImpl & p_task, iostream & p_output);
}; // End of class TasksMgtImpl
