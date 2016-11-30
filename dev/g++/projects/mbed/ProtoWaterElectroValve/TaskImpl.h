#pragma once

#include <vector>
#include <memory>

#include "mbed.h"

/** This class describes a programmable task:
 * - eValve identifier
 * - Date range to activate it
 * - Periode to swith on
 *
 * @author Yann Garcia (Don't hesitate to contact me: garcia.yann@gmail.com)
 */
class TaskImpl {
    
public:
    static const unsigned char TaskId = 0x00;
    static const unsigned char StartMonth = 0x01;
    static const unsigned char StartDay = 0x02;
    static const unsigned char StopMonth = 0x03;
    static const unsigned char StopDay = 0x04;
    static const unsigned char StartHour = 0x05;
    static const unsigned char SartMin = 0x06;
    static const unsigned char StopHour = 0x07;
    static const unsigned char StopMin = 0x08;
    static const unsigned char Cond = 0x09;
    static const unsigned char Period = 0x0a;
    static const unsigned char BoardNum = 0x0b;
    static const unsigned char PortNum = 0x0b;
    static const unsigned char Size = 0x0C;

    /** TODO
     */
    enum Periodicity {
        Even,   //<! Switch on only for even day
        Odd,    //<! Switch on only for odd day
        Always  //<! Switch on only for every day
    };
    /** TODO
     */
    enum Status {
        PowerOn,    //<! The eValve is powered on
        PowerOff    //<! The eValve is powered off
    };
    
private:
    /** Task identifier */
    unsigned char _id;
    /** Start month in BCD format */
    unsigned char _startMonth;
    /** Start day in BCD format */
    unsigned char _startDay;
    /** Stop month in BCD format */
    unsigned char _stopMonth;
    /** Stop day in BCD format */
    unsigned char _stopDay;
    /** Start hour in BCD format */
    unsigned char _startHour;
    /** Start minutes in BCD format */
    unsigned char _startMin;
    /** Stop hour in BCD format */
    unsigned char _stopHour;
    /** Stop minute in BCD format */
    unsigned char _stopMin;
    /** External conditions */
    unsigned char _condition;
    /** Repeatition code */
    Periodicity _periodicity;
    /** Status/Port/Board address */
    unsigned char _boardNum;
    unsigned char _portNum;  
    Status _mode;
    
public:
//    TaskImpl() { };
    TaskImpl(const unsigned char p_taskId, const unsigned char p_startMonth, const unsigned char p_startDay, const unsigned char p_stopMonth, const unsigned char p_stopDay, const unsigned char p_startHour, const unsigned char p_startMin, const unsigned char p_stopHour, const unsigned char p_stopMin, const unsigned char p_condition, const unsigned char p_periodicity, const unsigned char p_boardNum, const unsigned char p_portNum);
    virtual ~TaskImpl() { };
    
    /** 
     * @desc Get the task identifier 
     */
    inline unsigned char getId() const { return _id; };
    /** 
     * @desc Get the start month 
     */
    inline unsigned char getStartMonth() const { return _startMonth; };
    /** 
     * @desc Get the start day 
     */
    inline unsigned char getStartDay() const { return _startDay; };
    /** 
     * @desc Get the end month 
     */
    inline unsigned char getStopMonth() const { return _stopMonth; };
    /** 
     * @desc Get the end day 
     */
    inline unsigned char getStopDay() const { return _stopDay; };
    /** 
     * @desc Get the start hour 
     */
    inline unsigned char getStartHour() const { return _startHour; };
    /** 
     * @desc Get the start minutes 
     */
    inline unsigned char getStartMin() const { return _startMin; };
    /** 
     * @desc Get the end hour 
     */
    inline unsigned char getStopHour() const { return _stopHour; };
    /** 
     * @desc Get the end minute 
     */
    inline unsigned char getStopMin() const { return _stopMin; };
    /** 
     * @desc Get the external conditions code 
     */
    inline unsigned char getCondition() const { return _condition; };
    /** 
     * @desc Get the repeatition code 
     */
    inline Periodicity getPeriodicity() const { return _periodicity; };
    /** 
     * @desc Get the status/Port/Board address 
     */
    inline unsigned char getBoardNum() const { return _boardNum; };
    inline unsigned char getPortNum() const { return _portNum; };  
    inline Status getMode() const { return _mode; };
    
    void setMode(const Status p_mode);
    
 public:
    static void serialize(const TaskImpl & p_task, std::vector<unsigned char> & p_ar);
    static void deserialize(const std::vector<unsigned char> & p_ar, TaskImpl * * p_task);
}; // End of class TaskImpl
