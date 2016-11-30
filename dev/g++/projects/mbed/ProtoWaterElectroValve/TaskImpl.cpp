#include "TaskImpl.h"

#include "Converter.h"

#include "Debug.h"

TaskImpl::TaskImpl(const unsigned char p_taskId, const unsigned char p_startMonth, const unsigned char p_startDay, const unsigned char p_stopMonth, const unsigned char p_stopDay, const unsigned char p_startHour, const unsigned char p_startMin, const unsigned char p_stopHour, const unsigned char p_stopMin, const unsigned char p_condition, const unsigned char p_periodicity, const unsigned char p_boardNum, const unsigned char p_portNum): _id(p_taskId), _startMonth(p_startMonth), _startDay(p_startDay), _stopMonth(p_stopMonth), _stopDay(p_stopDay), _startHour(p_startHour), _startMin(p_startMin), _stopHour(p_stopHour), _stopMin(p_stopMin), _condition(p_condition), _boardNum(p_boardNum), _portNum(p_portNum), _mode(TaskImpl::PowerOff) {
    switch (p_periodicity) {
        case 0x00:
            _periodicity = TaskImpl::Even;
            break;
        case 0x01:
            _periodicity = TaskImpl::Odd;
            break;
        default:
            _periodicity = TaskImpl::Always;
    } // End of 'switch' statement
} // End of Ctor

void TaskImpl::setMode(const Status p_mode) {
    DEBUG_ENTER("TaskImpl::setMode: %s", p_mode)
    
    // Sanity check
    if (_mode == p_mode) {
        return;
    }
    
    _mode = p_mode;
    switch (_mode) {
        case TaskImpl::PowerOn:
            // TODO Do action 
            break;
        case TaskImpl::PowerOff:
            // TODO Do action 
            break;
    } // End of 'switch' statement
    
    DEBUG_LEAVE("TaskImpl::setMode")
}

void TaskImpl::serialize(const TaskImpl & p_task, std::vector<unsigned char> & p_ar) {
    DEBUG_ENTER("TaskImpl::serialize")
    
    p_ar.clear();
    p_ar.push_back(p_task.getId());
    p_ar.push_back(p_task.getStartMonth());
    p_ar.push_back(p_task.getStartDay());
    p_ar.push_back(p_task.getStopMonth());
    p_ar.push_back(p_task.getStopDay());
    p_ar.push_back(p_task.getStartHour());
    p_ar.push_back(p_task.getStartMin());
    p_ar.push_back(p_task.getStopHour());
    p_ar.push_back(p_task.getStopMin());
    p_ar.push_back(p_task.getCondition());
    p_ar.push_back(p_task.getPeriodicity());
    p_ar.push_back(
        (static_cast<unsigned char>(p_task.getMode()) & 0x01 << 7) | 
        (p_task.getBoardNum() & 0x07) << 3 | 
        p_task.getPortNum() & 0x03
    );
    
    DEBUG("TaskImpl::serialize: size=%d, '%s'\r", p_ar.size(), CConverter::getInstance().hexa2string(p_ar).c_str());
}

void TaskImpl::deserialize(const std::vector<unsigned char> & p_ar, TaskImpl * * p_task) {
    DEBUG_ENTER("TaskImpl::deserialize: size=%d, '%s'\r", p_ar.size(), CConverter::getInstance().hexa2string(p_ar).c_str());
    
    // Sanity check
    if (p_ar.size() != TaskImpl::Size) {
        return;
    }
        
    *p_task = new TaskImpl(
        p_ar[0], // taskId
        p_ar[1], // startMonth
        p_ar[2], // startDay
        p_ar[3], // endMonth
        p_ar[4], // endDay
        p_ar[5], // startHour
        p_ar[6], // startMin
        p_ar[7], // endHour
        p_ar[8], // endMin
        p_ar[9], // condition
        p_ar[10], // periodicity
        (p_ar[11] & 0x1C) >> 3, // boardNum,
        p_ar[11] & 0x03 // portNum
    );
}
