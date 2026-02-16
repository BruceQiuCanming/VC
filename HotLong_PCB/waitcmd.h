#ifndef WAITCMD_H
#define WAITCMD_H
#include "comm.h"



typedef CArray<MODBUS_RS485_CMD_ALL,MODBUS_RS485_CMD_ALL>CWaitCmdArray;

class CWaitCmd
{
public:
    CWaitCmd();
    virtual ~CWaitCmd();
    CWaitCmdArray m_WaitCmdArray;
    void AppendWaitCmd(MODBUS_RS485_CMD_ALL cmd);
    void RemoveFirstWaitCmd();
    void RemoveAllWaitCmd();
    bool GetFirstWaitCmd(MODBUS_RS485_CMD_ALL *cmd);
    int  SendWaitCmd(CComm *comm);

    int GetWaitCmdSize();

};

#endif // WAITCMD_H

