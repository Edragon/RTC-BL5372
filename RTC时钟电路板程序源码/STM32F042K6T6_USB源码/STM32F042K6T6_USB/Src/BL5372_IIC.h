#ifndef __BL5372_IIC_H__
#define __BL5372_IIC_H__







/*
 * Ricoh has a family of I2C based RTCs, which differ only slightly from
 * each other.  Differences center on pinout (e.g. how many interrupts,
 * output clock, etc) and how the control registers are used.  The '372
 * is significant only because that's the one this driver first supported.
 */
#define BL5372_REG_SECS	0
#define BL5372_REG_MINS	1
#define BL5372_REG_HOURS	2
#define BL5372_REG_WDAY	3
#define BL5372_REG_DAY		4
#define BL5372_REG_MONTH	5
#define BL5372_REG_YEAR	6
#define BL5372_REG_TRIM	7
#	define BL5372_TRIM_XSL		0x80
#	define BL5372_TRIM_MASK	0x7F

#define RS5C_REG_ALARM_A_MIN	8			/* or ALARM_W */
#define RS5C_REG_ALARM_A_HOURS	9
#define RS5C_REG_ALARM_A_WDAY	10

#define RS5C_REG_ALARM_B_MIN	11			/* or ALARM_D */
#define RS5C_REG_ALARM_B_HOURS	12
#define RS5C_REG_ALARM_B_WDAY	13			/* (ALARM_B only) */

#define RS5C_REG_CTRL1		14
#	define RS5C_CTRL1_AALE		(1 << 7)	/* or WALE */
#	define RS5C_CTRL1_BALE		(1 << 6)	/* or DALE */
#	define RV5C387_CTRL1_24		(1 << 5)
#	define BL5372A_CTRL1_SL1	(1 << 5)
#	define RS5C_CTRL1_CT_MASK	(7 << 0)
#	define RS5C_CTRL1_CT0		(0 << 0)	/* no periodic irq */
#	define RS5C_CTRL1_CT4		(4 << 0)	/* 1 Hz level irq */
#define RS5C_REG_CTRL2		15
#	define BL5372_CTRL2_24		(1 << 5)
#	define R2025_CTRL2_XST		(1 << 5)
#	define RS5C_CTRL2_XSTP		(1 << 4)	/* only if !R2025S/D */
#	define RS5C_CTRL2_CTFG		(1 << 2)
#	define RS5C_CTRL2_AAFG		(1 << 1)	/* or WAFG */
#	define RS5C_CTRL2_BAFG		(1 << 0)	/* or DAFG */


/* to read (style 1) or write registers starting at R */
#define RS5C_ADDR(R)		(((R) << 4) | 0)


void BL5372_init(void);
unsigned char  BL5372_Readonebyte(unsigned char readadr);
void BL5372_Writeonebyte(unsigned char  writeadr,unsigned char wdat);
void BL5372_Read(unsigned char ReadAddr,unsigned char *pBuffer,unsigned char NumToRead);
void BL5372_Write(unsigned char WriteAddr,unsigned char *pBuffer,unsigned char NumToWrite);



void IIC_Init(void);











#endif

