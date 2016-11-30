#include <string>

#include "UserCmdImpl.h"

#include "Converter.h"

UserCmdImpl::UserCmdImpl() : _taskDescription("") {
    _ptr.reset(NULL);
}

UserCmdImpl::~UserCmdImpl() {
}

std::unique_ptr<UserCmdDesc> & UserCmdImpl::parse(const std::vector<unsigned char> & p_cmdBuffer) {
    DEBUG_ENTER("UserCmdImpl::parse: %s", CConverter::getInstance().hexa2string(p_cmdBuffer).c_str())
    
    // Sanity check
    if (p_cmdBuffer.size() < 2) {
        std::cerr << "UserCmdImpl::parse: Wrong parameters\r" << std::endl;
        _ptr.reset(NULL);
        return _ptr;
    }
    
    // Check command identifier
    std::string str((char *)p_cmdBuffer.data(), (char *)p_cmdBuffer.data() + 2);
    DEBUG("UserCmdImpl::parse: Process command '%s'\r", str.c_str())
    if (p_cmdBuffer[2] != '=') {
        if ((p_cmdBuffer[2] == '\r') || (p_cmdBuffer[2] == '\n')) {
            _ptr.reset(new UserCmdDesc);
            _ptr.get()->setCommand(str);
            // No parameters, check this is an authorized command
            if (
                (str.compare(CTK_CMD) != 0) && 
                (str.compare(UTK_CMD) != 0) && 
                (str.compare(LTK_CMD) != 0) && 
                (str.compare(DTK_CMD) != 0) && 
                (str.compare(GTK_CMD) != 0) && 
                (str.compare(GTS_CMD) != 0) && 
                (str.compare(FTK_CMD) != 0) && 
                (str.compare(FST_CMD) != 0) && 
                (str.compare(ALV_CMD) != 0) && 
                (str.compare(CFG_CMD) != 0) && 
                (str.compare(RST_CMD) != 0) && 
                (str.compare(UDP_CMD) != 0) 
            ) { // TODO Enhanced with new commands without arguments
                // Unsupported command
                std::cerr << "UserCmdImpl::parse: command: '" << str << "' requires parameters\r" << std::endl;
                _ptr.reset(nullptr);
            }
        } else { // Invalid command
            std::cerr << "UserCmdImpl::parse: Invalid command: '" << str << "'\r" << std::endl;
            _ptr.reset(nullptr);
        }
        return _ptr;
    }
    _ptr.reset(new UserCmdDesc);    
    _ptr.get()->setCommand(str);
    // Extract parameters
    int i = 3; // Skip '='
    std::stringstream ss;
//    DEBUG("UserCmdImpl::parse: i='%d' - End loop:'%d'\r", i, p_cmdBuffer.size() - 2)
    while (i <= p_cmdBuffer.size() - 2/*skip LF*/) {
         do {
//            DEBUG("UserCmdImpl::parse: Process char '%02x'\r", p_cmdBuffer[i])
            ss << static_cast<int>(p_cmdBuffer[i++]);
//            DEBUG("UserCmdImpl::parse: Current string '%s'\r", ss.str().c_str())
        } while ((i < p_cmdBuffer.size() - 2/*skip LF*/) and (p_cmdBuffer[i] != ','));
//        DEBUG("UserCmdImpl::parse: Add new parameter '%s' - Length:'%d'\r", ss.str().c_str(), ss.str().length())
        // Add new parameter
        _ptr.get()->addParam(ss.str());
        // Clear content
        ss.str("");
        // Skip separator ','
        i += 1;
    } // End of 'while' loop
        
    return _ptr;
}

int UserCmdImpl::execute(const std::unique_ptr<UserCmdDesc> & p_cmdDesc, TasksMgtImpl & p_tasksMgtImpl) {
    // check command
    if (p_cmdDesc->getCommand().compare(CTK_CMD) == 0) { // Create task
        // Create a new task
        std::unique_ptr<TaskImpl> task(
            new TaskImpl(
                static_cast<unsigned char>(p_tasksMgtImpl.getTaskId()), // Unknown task identifier, it shall be set by TasksMgtImpl
                CConverter::getInstance().string2bcd(p_cmdDesc->getParmNo(TaskImpl::StartMonth)),
                CConverter::getInstance().string2bcd(p_cmdDesc->getParmNo(TaskImpl::StartDay)),
                CConverter::getInstance().string2bcd(p_cmdDesc->getParmNo(TaskImpl::StopMonth)),
                CConverter::getInstance().string2bcd(p_cmdDesc->getParmNo(TaskImpl::StopDay)),
                CConverter::getInstance().string2bcd(p_cmdDesc->getParmNo(TaskImpl::StartHour)),
                CConverter::getInstance().string2bcd(p_cmdDesc->getParmNo(TaskImpl::SartMin)),
                CConverter::getInstance().string2bcd(p_cmdDesc->getParmNo(TaskImpl::StopHour)),
                CConverter::getInstance().string2bcd(p_cmdDesc->getParmNo(TaskImpl::StopMin)),
                CConverter::getInstance().string2bcd(p_cmdDesc->getParmNo(TaskImpl::Cond)),
                static_cast<unsigned char>(CConverter::getInstance().string2int(p_cmdDesc->getParmNo(TaskImpl::Period))),
                static_cast<unsigned char>(CConverter::getInstance().string2int(p_cmdDesc->getParmNo(TaskImpl::BoardNum))),
                static_cast<unsigned char>(CConverter::getInstance().string2int(p_cmdDesc->getParmNo(TaskImpl::PortNum)))
            )
        );
        // Update EEPROM
        p_tasksMgtImpl.createTask(task);
        
        // Return the task number
        unsigned char result = task->getId();
        task.release();
        return result;
    } else if (p_cmdDesc.get()->getCommand().compare(UTK_CMD) == 0) { // Update task
        // Create a new task
        std::unique_ptr<TaskImpl> task(
            new TaskImpl(
                static_cast<unsigned char>(CConverter::getInstance().string2int(p_cmdDesc->getParmNo(TaskImpl::TaskId))), // Well known task identifier
                CConverter::getInstance().string2bcd(p_cmdDesc->getParmNo(TaskImpl::StartMonth)),
                CConverter::getInstance().string2bcd(p_cmdDesc->getParmNo(TaskImpl::StartDay)),
                CConverter::getInstance().string2bcd(p_cmdDesc->getParmNo(TaskImpl::StopMonth)),
                CConverter::getInstance().string2bcd(p_cmdDesc->getParmNo(TaskImpl::StopDay)),
                CConverter::getInstance().string2bcd(p_cmdDesc->getParmNo(TaskImpl::StartHour)),
                CConverter::getInstance().string2bcd(p_cmdDesc->getParmNo(TaskImpl::SartMin)),
                CConverter::getInstance().string2bcd(p_cmdDesc->getParmNo(TaskImpl::StopHour)),
                CConverter::getInstance().string2bcd(p_cmdDesc->getParmNo(TaskImpl::StopMin)),
                CConverter::getInstance().string2bcd(p_cmdDesc->getParmNo(TaskImpl::Cond)),
                static_cast<unsigned char>(CConverter::getInstance().string2int(p_cmdDesc->getParmNo(TaskImpl::Period))),
                static_cast<unsigned char>(CConverter::getInstance().string2int(p_cmdDesc->getParmNo(TaskImpl::BoardNum))),
                static_cast<unsigned char>(CConverter::getInstance().string2int(p_cmdDesc->getParmNo(TaskImpl::PortNum)))
            )
        );
        // Update EEPROM
        p_tasksMgtImpl.updateTask(task);
        
        // Return the task number
        return task->getId();
    } else if (p_cmdDesc.get()->getCommand().compare(DTK_CMD) == 0) { // Delete task
        unsigned char taskId = static_cast<unsigned char>(CConverter::getInstance().string2int(p_cmdDesc->getParmNo(0)));
        return p_tasksMgtImpl.deleteTask(taskId);
    } else if (p_cmdDesc.get()->getCommand().compare(GTK_CMD) == 0) { // Get task description
        unsigned char taskId = static_cast<unsigned char>(CConverter::getInstance().string2int(p_cmdDesc->getParmNo(0)));
        int result = p_tasksMgtImpl.getTaskDescription(taskId, _taskDescription);
        if (result == 0) {
            return 1;
        }
        return result;
    } else if (p_cmdDesc.get()->getCommand().compare(LTK_CMD) == 0) { // List tasks
        // List all tasks
        std::stringstream output;
        bool forceReload = false;
        if (p_cmdDesc->getParmsNumber() == 1) {
            forceReload = static_cast<bool>((CConverter::getInstance().string2int(p_cmdDesc->getParmNo(0)) == 1));
            DEBUG("UserCmdImpl::execute: LT command with forceReload: %x\r", static_cast<unsigned char>(forceReload))
        }
        int result = p_tasksMgtImpl.listTasks(output, forceReload);
        if (result == 0) {
//            DEBUG("UserCmdImpl::execute: %s", output.str().c_str());
            _taskDescription.assign(output.str());
            return 1;
        }
        return result;
    } else if (p_cmdDesc.get()->getCommand().compare(GTS_CMD) == 0) { // Status task
        unsigned char taskId = static_cast<unsigned char>(CConverter::getInstance().string2int(p_cmdDesc->getParmNo(0)));
        DEBUG("UserCmdImpl::execute: TS command: %d\r", taskId)
        return p_tasksMgtImpl.getTaskStatus(taskId, _taskDescription);
    } else if (p_cmdDesc.get()->getCommand().compare(FTK_CMD) == 0) { // Force start
        unsigned char taskId = static_cast<unsigned char>(CConverter::getInstance().string2int(p_cmdDesc->getParmNo(0)));
        DEBUG("UserCmdImpl::execute: TS command: %d\r", taskId)
        return p_tasksMgtImpl.getForceTaskStart(taskId);
    } else if (p_cmdDesc.get()->getCommand().compare(FST_CMD) == 0) { // Force stop
        unsigned char taskId = static_cast<unsigned char>(CConverter::getInstance().string2int(p_cmdDesc->getParmNo(0)));
        DEBUG("UserCmdImpl::execute: TS command: %d\r", taskId)
        return p_tasksMgtImpl.getForceTaskStop(taskId);
    } else if (p_cmdDesc.get()->getCommand().compare(CFG_CMD) == 0) {
        if (p_cmdDesc->getParmsNumber() == 0) {
            // Force memory dump
            p_tasksMgtImpl.forceMemoryDump(0, 128);
        } else {
            unsigned char boardsNum = static_cast<unsigned char>(CConverter::getInstance().string2int(p_cmdDesc->getParmNo(0)));
            DEBUG("UserCmdImpl::execute: CF command: %d\r", boardsNum)
            p_tasksMgtImpl.setBoardNumber(boardsNum);
        }
    } else if (p_cmdDesc.get()->getCommand().compare(RST_CMD) == 0) { // Reset all, include memory reset
        p_tasksMgtImpl.forceResetMemory();
    } else if (p_cmdDesc.get()->getCommand().compare(ALV_CMD) == 0) { // Alive
        // Alive, Nothing to do
    } else if (p_cmdDesc.get()->getCommand().compare(UDP_CMD) == 0) {
    } else { // Unsupported command
        std::cerr << "UserCmdImpl::execute: Unsupported user command: " << p_cmdDesc.get()->getCommand() << "\r" << std::endl;
        return -5;
    }
    
    return 0;
}

