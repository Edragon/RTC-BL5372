#include "BL5372_IIC.h"
#include "stm32f0xx_hal.h"

#define SEND_SDA_L HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9 , GPIO_PIN_RESET);// 输出低(SDA上电立即拉低)
#define SEND_SDA_H HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9 , GPIO_PIN_SET);// 输出低(SDA上电立即拉低)

#define SEND_SCL_L HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8 , GPIO_PIN_RESET);// 输出低(SCL上电立即拉低)
#define SEND_SCL_H HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8 , GPIO_PIN_SET);// 输出低(SCL上电立即拉低)


#define READ_SDA HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_9)



unsigned char Uart_RX_Buffer[30] = {0x00};
unsigned char *String;
unsigned char WR_Flag;



void Delay(unsigned int delay_Count)
{
	unsigned int i=0;
	
	for(i=0;i<delay_Count;i++)
	{
	
	}
	

} 


//初始化IIC
void IIC_Init(void)
{					     
		GPIO_InitTypeDef GPIO_InitStruct;
	 __HAL_RCC_GPIOA_CLK_ENABLE();
	   
	  /*Configure GPIO pin : PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9; ;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;//开漏输出
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	

}

void SDA_OUT(void)
{
		GPIO_InitTypeDef GPIO_InitStruct;
	 __HAL_RCC_GPIOA_CLK_ENABLE();



  /*Configure GPIO pin : PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_9 ;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;//开漏输出
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


}
void SDA_IN(void)
{
		GPIO_InitTypeDef GPIO_InitStruct;
	 __HAL_RCC_GPIOA_CLK_ENABLE();



  /*Configure GPIO pin : PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_9 ;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;//输入
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


}



//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出

	SEND_SDA_H;	
	SEND_SCL_H;	
//	delay_us(14);
	Delay(0xFF);
//开始：当SCL为高时，数据从高到低变化
	SEND_SDA_L;
//	delay_us(14);
	Delay(0xFF);	
//钳住I2C总线，准备发送或接收数据 
	SEND_SCL_L;
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出

	SEND_SCL_L;
//停止:当SCL为高时，数据从低到高变化
	SEND_SDA_L;
	
 //	delay_us(14);
	Delay(0xFF);
	SEND_SCL_H;
//	delay_us(14);	
	Delay(0xFF);	
	
//发送I2C总线结束信号
	SEND_SDA_H;
							   	
}



//产生ACK应答
void IIC_Ack(void)
{

	SEND_SCL_L;
	SDA_OUT();
	

	SEND_SDA_L;
//	delay_us(15);
		Delay(0xFF);

	SEND_SCL_H;
//	delay_us(15);
	Delay(0xFF);

	SEND_SCL_L;
}
//产生非ACK应答		    
void IIC_NAck(void)
{
//	IIC_SCL=0;
	SEND_SCL_L;
	SDA_OUT();

	SEND_SDA_H;
	
//	delay_us(15);
	Delay(0xFF);

	SEND_SCL_H;
//	delay_us(15);
	Delay(0xFF);

	SEND_SCL_L;
}			
//************************************************

unsigned char Test_Ack(void)
{  //??SDA ????
   unsigned char ACK_Flag=0;
   //SCL=0;

	SEND_SCL_L;

	SDA_OUT();

	SEND_SDA_H;
	
 // delay_us(15);
	Delay(0xFF);
	

	SEND_SCL_H;
  	
 // delay_us(15);
	Delay(0xFF);
	SDA_IN();
	
   if(READ_SDA==0)
     ACK_Flag = 1;
   else 
     ACK_Flag = 0;
  // SCL=0;
	 SEND_SCL_L;
   return ACK_Flag;
}


//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	SDA_OUT(); 	    
//拉低时钟开始数据传输
	SEND_SCL_L;
    for(t=0;t<8;t++)
    {              

			SEND_SCL_L;
		if((txd&0x80)>>7)
		{
			SEND_SDA_H;
		}
		else
		{
			SEND_SDA_L;
		}
		
		txd<<=1; 	  
	//	delay_us(15);   //延时
		Delay(0xFF);

		SEND_SCL_H;
//		delay_us(15); 	
		Delay(0xFF);
    }
	 
		SEND_SCL_L;
} 


//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        //IIC_SCL=0; 
		SEND_SCL_L;
  //      delay_us(50);
		Delay(0xFF);
			//	IIC_SCL=1;
		SEND_SCL_H;
		//		delay_us(50);
		Delay(0xFF);
        receive<<=1;
        if(READ_SDA)receive++;   
    }
        //IIC_SCL=0;
		SEND_SCL_L;		
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}
//**************************************************

unsigned char ReceiveData()
{
  unsigned char BitCnt=8,IIC_RX_Data=0;
//  unsigned char temp=0;
	

	SDA_OUT();

	SEND_SDA_H;
  do
  {

		SEND_SCL_L;
	// delay10us(); 
	Delay(0xFF);		

		
	IIC_RX_Data= (IIC_RX_Data<<1);   //
		IIC_RX_Data +=0x01;
	 BitCnt--;	  
		SEND_SCL_H;
	SDA_IN();//SDA设置为输入	
//	 delay10us();
		Delay(0xFF);
		
     if(READ_SDA == 1)
       IIC_RX_Data = IIC_RX_Data|0x01;  //???1
     else
       IIC_RX_Data = IIC_RX_Data&0x0fe; //???0	    
   }
   while(BitCnt);
	 SEND_SCL_L;
   return IIC_RX_Data;
}


//***************************************************
unsigned char WriteNByte(unsigned char sla,unsigned char *s,unsigned char n)
{
   unsigned char i;
   
   IIC_Start();  //
   IIC_Send_Byte(sla);//
   if(!Test_Ack())
   {	
      WR_Flag = 1;
	  return(0);
   }
   for(i=0;i<n;i++)//
   {
      IIC_Send_Byte(*(s+i));
	  if(!Test_Ack())
	  {
	    WR_Flag = 1;
		return(0);
	  }
   }
   IIC_Stop();
   return(1);
}
unsigned char ReadNByte(unsigned char Sal, unsigned char *p,unsigned char n)
{
  unsigned char i;
  IIC_Start();    // ??I2C
  IIC_Send_Byte((Sal)| 0x01); 
  if(!Test_Ack())
  {
  	WR_Flag = 1;
	return(0);
  }
//  delay10us();  
//  delay10us();
//  delay10us(); // 
   Delay(0xFF);
	Delay(0xFF);
	Delay(0xFF);	
  for(i=0;i<n-1;i++)  //
  {
     *(p+i)=ReceiveData(); //

		IIC_Ack();
  }
  *(p+n-1)=ReceiveData();        

	IIC_NAck();
  IIC_Stop(); 
  return(1);	 
}

unsigned int CRC16(unsigned char *ptr, unsigned char len)
{
   unsigned int crc=0xffff;
   unsigned char i;
   while(len--)
   {
       crc ^=*ptr++;
       for(i=0;i<8;i++)
	   {
	       if(crc & 0x1)
		   {
		      crc>>=1;
			  crc^=0xa001;
		   }
		   else
		   {
		      crc>>=1;
		   }
	   }
   }
   return crc;
}

unsigned char CheckCRC(unsigned char *ptr,unsigned char len)
{
  unsigned int crc;
	crc=(unsigned int)CRC16(ptr,len-2);
	if(ptr[len-1]==(crc>>8) && ptr[len-2]==(crc & 0x00ff))
	{
	    return 0xff;
	}
	else
	{
	   return 0x0;
	}
}
void Waken(void)
   {
    IIC_Start();       // 
    IIC_Send_Byte(0xB8); // 
    Test_Ack();	       // 
    HAL_Delay(2);       // 
    IIC_Stop();	
   }




unsigned char    BL5372buf[16];
	 
	 
	 
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
 unsigned char IIC_Wait_Ack(void)
{
	 unsigned char ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
//	IIC_SDAOUT_H;///IIC_SDA=1;
	SEND_SDA_H;
	 //Delay_us(1);	 
Delay(0xFF);  
//	IIC_SCL_H;///IIC_SCL=1;
	SEND_SCL_H;
//	 Delay_us(1);
Delay(0xFF);	
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
//	IIC_SCL_L;//IIC_SCL=0;//时钟输出0 	 
	SEND_SCL_L;  
	return 0;  
} 	 
///////
//////
//////

unsigned char Set_Year =0x17;	
unsigned char Set_Month =0x02;	
unsigned char Set_Day =0x25;	
unsigned char Set_Hour =0x23;	
unsigned char Set_Minute =0x30;	
unsigned char Set_Second =0x00;	
void BL5372_init(void)
{
	    unsigned char temp;
	       ///IIC_Init();
	      //初始化时间为2017年12月31日23时59分00秒
	       BL5372buf[14]=0x00;
			   BL5372buf[15]=0x20;
			   BL5372buf[0]=Set_Second;////second
			   BL5372buf[1]=Set_Minute;////minute
			   BL5372buf[2]=Set_Hour;////hour
			   BL5372buf[3]=0x04;////week
			   BL5372buf[4]=Set_Day;////day
			   BL5372buf[5]=Set_Month;////month
			   BL5372buf[6]=Set_Year;///year
			   BL5372buf[7]=0x00;
			   BL5372buf[8]=0x00;
			   BL5372buf[9]=0x00;
			   BL5372buf[10]=0x00;
			   BL5372buf[11]=0x00;
			   BL5372buf[12]=0x00;
			   BL5372buf[13]=0x00;
	      do{
	           BL5372_Write(0x00,BL5372buf,16);
	           temp=BL5372_Readonebyte(0x0f);
				   }while((temp&0x10)==0x10);
	
}	
///// Read BL5372 from a specific  address
/////readadr:  start  read  address
/////return: read value
unsigned char  BL5372_Readonebyte(unsigned char readadr)
{
	  unsigned char  tmp;
	  IIC_Start();  
	  IIC_Send_Byte(0X64);	   //发送写命令
		IIC_Wait_Ack();
	  IIC_Send_Byte(((readadr<<4)|0));	   //?
		IIC_Wait_Ack();
	  IIC_Start();  	 	   
	  IIC_Send_Byte(0X65);           //进入接收模式			   
	  IIC_Wait_Ack();
    tmp=IIC_Read_Byte(0);		   
    IIC_Stop();//产生一个停止条件	    
	  return tmp;		
}
/////write  BL5372 at a specific address
////writeadr:addres to write  
////wdat:data  to write
void BL5372_Writeonebyte(unsigned char  writeadr,unsigned char wdat)
{
	  IIC_Start();  
	  IIC_Send_Byte(0X64);	   //发送写命令
	  IIC_Wait_Ack();
	  IIC_Send_Byte(((writeadr<<4)|0));	   //?
		IIC_Wait_Ack();
	  IIC_Send_Byte(wdat);	   //?
		IIC_Wait_Ack();
	  IIC_Stop();
//	  Delay_ms(4);
	HAL_Delay(4);
}
//////read  data  from  BL5372   at spcific   address  
/////ReadAddr:  read start  address
/////pBuffer:  the  first  address  of  data
////NumToRead:
void BL5372_Read(unsigned char ReadAddr,unsigned char *pBuffer,unsigned char NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=BL5372_Readonebyte(ReadAddr++);	
		NumToRead--;
	}
}
///////write  data  from  BL5372   at spcific   address 
//////WriteAddr:the  address  shtart to  write
//////pBuffer:the  first  address  of  data
//////NumToWrite:
void BL5372_Write(unsigned char WriteAddr,unsigned char *pBuffer,unsigned char NumToWrite)
{
	while(NumToWrite--)
	{
		BL5372_Writeonebyte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}


