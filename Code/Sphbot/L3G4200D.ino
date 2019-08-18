

//Internal register of Gyro sensor L3G4200D
#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24
double GyroNorm=0.0f;
byte L3G4200D_Address = 105;  //I2C address of the L3G4200D

//Read gyroscope values
void L3G4200D_Read()
{  
  byte xMSB =(byte) readRegister(L3G4200D_Address, 0x29);
  byte xLSB =(byte) readRegister(L3G4200D_Address, 0x28);
  Gyr_x =(signed short)((xMSB << 8) | xLSB);

  byte yMSB =(byte) readRegister(L3G4200D_Address, 0x2B);
  byte yLSB =(byte) readRegister(L3G4200D_Address, 0x2A);
  Gyr_y =(signed short)((yMSB << 8) | yLSB);

  byte zMSB =(byte) readRegister(L3G4200D_Address, 0x2D);
  byte zLSB =(byte) readRegister(L3G4200D_Address, 0x2C);
  Gyr_z =(signed short)((zMSB << 8) | zLSB);
  
//  GyroNorm = (double)sqrt((Gyr_x*Gyr_x)+(Gyr_y*Gyr_y)+(Gyr_z*Gyr_z));
//  Gyr_x /= GyroNorm;
//  Gyr_y /= GyroNorm;
//  Gyr_z /= GyroNorm;
}

//setup for firs time he gyroscope sensor
void setupL3G4200D(int scale)
{
  // Enable x, y, z and turn off power down:
  writeRegister(L3G4200D_Address, CTRL_REG1, 0b00001111);

  // If you'd like to adjust/use the HPF, you can edit the line below to configure CTRL_REG2:
  writeRegister(L3G4200D_Address, CTRL_REG2, 0b00000000);

  // Configure CTRL_REG3 to generate data ready interrupt on INT2
  // No interrupts used on INT1, if you'd like to configure INT1
  // or INT2 otherwise, consult the datasheet:
  writeRegister(L3G4200D_Address, CTRL_REG3, 0b00001000);

  // CTRL_REG4 controls the full-scale range, among other things:
  if(scale == 250)
  {
    writeRegister(L3G4200D_Address, CTRL_REG4, 0b00000000);
  }
  else if(scale == 500)
  {
    writeRegister(L3G4200D_Address, CTRL_REG4, 0b00010000);
  }
  else //2000 dps
  {
    writeRegister(L3G4200D_Address, CTRL_REG4, 0b00110000);
  }

  // CTRL_REG5 controls high-pass filtering of outputs, use it
  writeRegister(L3G4200D_Address, CTRL_REG5, 0b00000000);
}
