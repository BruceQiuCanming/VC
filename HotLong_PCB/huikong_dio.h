#ifndef HUIKONG_DIO_H
#define HUIKONG_DIO_H
#include "modbus.h"
#include    "comm.h"
#include "waitcmd.h"

typedef enum
{
    HuiKong_X1_ADDR	= 0,
    HuiKong_X2_ADDR	= 1,
    HuiKong_X3_ADDR	= 2,
    HuiKong_X4_ADDR	= 3,
}HuiKong_X_ADDR;

typedef enum
{
    HuiKong_Y1_ADDR	= 0,
    HuiKong_Y2_ADDR	= 1,
    HuiKong_Y3_ADDR	= 2,
    HuiKong_Y4_ADDR	= 3,
}HuiKong_Y_ADDR;


class CDevice_HuiKong_DIO : public CComm, public CWaitCmd
{
public:
    CDevice_HuiKong_DIO();
    ~CDevice_HuiKong_DIO();
	COMM_THREAD_PARA		m_CommPara;
    int						m_BoxNr;
    int                     m_Y[16];
    int                     m_X[16];

    MODBUS_RS485_CMD_SET_WORD_1 SetYs(int Relay_ID,bool On);
    void SetY(int Relay_ID,bool On);
    void Set_Red_Light(bool On);
    void Set_Green_Light(bool On);
    void Set_Brue_Light(bool On);
    void Set_BEEP(bool On);
    void SetCmdString(int size, void* para);
    void Init(int m_CommNr,int BoxNr);
    void Read_X(CComm *comm,int Start_X,int Counts);
    void Read_Y(CComm *comm,int Start_Y,int Counts);
    void DealOnComm(unsigned char *Data,int DataLen);
    void SetBaudrate(CComm *comm);

};



#endif // HUIKONG_DIO_H

