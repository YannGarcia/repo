#pragma once

#include <memory>
#include <vector>

#include "UserCmdDesc.h"
#include "TasksMgtImpl.h"

/**
 * @desc
 * \remark The user command syntax uses basic grammar: <command>[=[parm[,]]]
 *         where:
 *               <command> could be:
 *                  ALV: Alive, check the server is alive
 *                  CFG: Configure
 *                          [Board number]
 *                  CTK: Create new task
 *                  UTK: Update task
 *                          Task identifier
 *                  DTK: Delete task
 *                          Task identifier
 *                  LTK: List tasks
 *                  GTK: Get task description
 *                          Task identifier
 *                  UDP: Change UDP server port
 *                  GTS: Get status task (running, stopped, forced)
 *                  FTK: Force a task to start
 *                  FST: Force a task to stop
 *                  RST: Reset
 */
#define ALV_CMD     "ALV"
#define CFG_CMD     "CFG"
#define CTK_CMD     "CTK"
#define UTK_CMD     "UTK"
#define DTK_CMD     "DTK"
#define LTK_CMD     "LTK"
#define GTK_CMD     "GTK"
#define UDP_CMD     "UDP"
#define GTS_CMD     "GTS"
#define FTK_CMD     "FTK"
#define FST_CMD     "FST"
#define RST_CMD     "RST"
 
class UserCmdImpl {
  std::unique_ptr<UserCmdDesc> _ptr;
  std::string _taskDescription;
    
 public:
  UserCmdImpl();
  virtual ~UserCmdImpl();
        
  std::unique_ptr<UserCmdDesc> & parse(const std::vector<unsigned char> & p_cmdBuffer);
  int execute(const std::unique_ptr<UserCmdDesc> & p_cmdDesc, TasksMgtImpl & p_memoryImpl);
        
  inline std::string & getTaskDescription() { return _taskDescription; };
        
}; // End of class UserCmdImpl
