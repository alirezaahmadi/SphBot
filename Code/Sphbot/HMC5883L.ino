

//Address of i2c Device Magnometer HMC5883L
#define HMC5883L_Address 0x1E
double CompNorm=0.0f;
//Setup Magnometer sensor for first time
void setup_HMC5883L()
{
  writeRegister(0x1E, 0x02, 0x00); 
}

//Read data from magnometer sensor
void HMC5883L_Read()
{
  byte xMSB =(byte) readRegister(HMC5883L_Address, 0x03);
  byte xLSB =(byte) readRegister(HMC5883L_Address, 0x04);
  Cmp_x = ((xMSB << 8) | xLSB);

  byte yMSB =(byte) readRegister(HMC5883L_Address, 0x05);
  byte yLSB =(byte) readRegister(HMC5883L_Address, 0x06);
  Cmp_y = ((yMSB << 8) | yLSB);

  byte zMSB =(byte) readRegister(HMC5883L_Address, 0x07);
  byte zLSB =(byte) readRegister(HMC5883L_Address, 0x08);
  Cmp_z = ((zMSB << 8) | zLSB);
  
//  CompNorm = (double)sqrt((Cmp_x*Cmp_x)+(Cmp_y*Cmp_y)+(Cmp_z*Cmp_z));
//  Cmp_x /= CompNorm;
//  Cmp_y /= CompNorm;
//  Cmp_z /= CompNorm;
}
