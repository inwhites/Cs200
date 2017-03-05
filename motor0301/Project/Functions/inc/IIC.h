//PB1 -- SDA   PB0 -- SCL
void IIC_GPIO_Config(void);
int IIC_Start(void);
void IIC_Stop(void);
void I2C_Ack(void);
void I2C_NoAck(void);
int I2C_WaitAck(void);
void I2C_SendByte(u8 SendByte);
unsigned char I2C_RadeByte(void);      
int Single_Write(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data);
unsigned char Single_Read(unsigned char SlaveAddress,unsigned char REG_Address);				      
