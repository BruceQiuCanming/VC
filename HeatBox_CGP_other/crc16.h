#ifndef _CRC_H_
#define _CRC_H_

#ifdef HIGH_VOLT
	#define CRC_START_VAL_0		0xF2
	#define CRC_START_VAL_1		0xAF
#else
	#define CRC_START_VAL_0		0xFF
	#define CRC_START_VAL_1		0xFF
#endif

void CRC16(unsigned char *source, int  n, unsigned char crc_init[2], unsigned char ReturnData[2]);

#endif