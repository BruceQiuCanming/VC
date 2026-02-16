#include "stdafx.h"

#include "waitcmd.h"
#include "comm.h"

CWaitCmd::CWaitCmd()
{

}

CWaitCmd::~CWaitCmd()
{

}

int CWaitCmd::GetWaitCmdSize()
{
    return this->m_WaitCmdArray.GetSize();
}

void CWaitCmd::AppendWaitCmd(MODBUS_RS485_CMD_ALL cmd)
{
   this->m_WaitCmdArray.Add(cmd);
}
void CWaitCmd::RemoveAllWaitCmd(void)
{
    while(m_WaitCmdArray.GetSize() > 0)
    {
        m_WaitCmdArray.RemoveAt(0);
    }
}

void CWaitCmd::RemoveFirstWaitCmd(void)
{
   if(m_WaitCmdArray.GetSize() > 0)
   {
       m_WaitCmdArray.RemoveAt(0);
   }
}

bool CWaitCmd::GetFirstWaitCmd(MODBUS_RS485_CMD_ALL *cmd)
{
    if(m_WaitCmdArray.GetSize() > 0)
    {
        m_WaitCmdArray.GetAt(0);
        return true;
    }
    return false;
}

int CWaitCmd::SendWaitCmd(CComm *comm)
{
    if(m_WaitCmdArray.GetSize() > 0)
    {
       MODBUS_RS485_CMD_ALL all = m_WaitCmdArray.GetAt(0);
       if( comm->WriteComm(&all.RS485_Addr,all.data_size) == -999)
       {
           AfxMessageBox(_T("-999"));
       }
       comm->m_LastSendCmd = all;
       

       //    G_MainWindow->m_DebugDlg[0].DisplayUpdateDownFirmware(GetWaitCmdSize());
       


       return all.data_size;
    }
    return 0;
}
