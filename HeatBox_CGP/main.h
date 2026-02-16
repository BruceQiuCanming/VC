#ifndef _MAIN_H_
#define _MAIN_H_

extern unsigned char G_cMode;
extern unsigned char G_cSubMode;
extern long  G_iTopTemp;	
extern int  G_iTopTime;
extern char G_cCoolSpeed;
extern char G_cTestSpeed;
extern stEEPROM G_Para;
extern signed long  G_iLastCoolTemp;

void InitEveryMode(char mode);
void DealKeyboard(void);
void SaveData(char mode);
void UpLeftRightOK(unsigned char newKeyVal);
void SetModeInit(int val);
int check_data(int source,int max,int min);
int ChgLedBufToVal(void);
void SystemParaInit(void);
void NewPID(void);
int FourBuf2Int(char* buf);
void Int2Buf(char* buf,int val,char max_buf_id);
void DisplayTemp(char* led_buf,int cur);
int CheckSum(char* source,unsigned char len);
void deal_RS485(void);
void EnableEEPROMWrite(void);
char CheckParaValid(stEEPROM *pPara);

#endif