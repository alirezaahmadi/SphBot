

//Include Lib 
#include <Wire.h>
//#include <MapleFreeRTOS.h>

//Global Deffenition 
#define DEG2RAD (0.0174532925199432957)
#define RAD2DEG (57.295779513082320876)

//Global Varibles
signed short Gyr_x=0 , Gyr_y=0 , Gyr_z=0;                //Gyroscope sensor data
signed short Acc_x=0 , Acc_y=0 , Acc_z=0;                //Accelorometer sensor dada
signed short Cmp_x=0 , Cmp_y=0 , Cmp_z=0;                //Compasss sensor data
double q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;	 //Quaternion of sensor frame relative to auxiliary frame
double roll=0.0f, pitch=0.0f, yaw=0.0f;                   //Estimated Orientation

long IMUTaskCNT=0 , IMUTaskHz=0;                 //IMU task internal frequency value
long SerialUSBTaskCNT=0 , SerialUSBTaskHz=0;     //Serial task internal frequency value
long RTC=0;                                              //Real Time Clock value
float Hz=0;
long Lastmillis=0;
#define Gyro_Scale (0.00126) //define the gyro scale that represent it in deg/s from gyro data
//Setup the main confiquration
/* Serial device defines for dxl bus */
#define DXL_BUS_SERIAL1 1  //Dynamixel on Serial1(USART1)  <-OpenCM9.04
#define DXL_BUS_SERIAL2 2  //Dynamixel on Serial2(USART2)  <-LN101,BT210
#define DXL_BUS_SERIAL3 3  //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP
/* Dynamixel ID defines */

/* Control table defines */
#define ID_Change_Address 3
#define Goal_Postion_Address 30
#define Moving_Speed 32

#define _LOBYTE(w)           ((unsigned char)(((unsigned long)(w)) & 0xff))
#define _HIBYTE(w)           ((unsigned char)((((unsigned long)(w)) >> 8) & 0xff))

#define NEW_ID 1 //New ID to be changed.

#define Buzzer_pin 10
#define Motor_in1 2
#define Motor_in2 3
#define CameraChannel_pin 9
#define Battery_pin 8
#define EncoderCHB 6
#define EncoderCHA 7

Dynamixel Dxl(DXL_BUS_SERIAL1);

void setup()
{
  pinMode(24, OUTPUT);
  digitalWrite(24, LOW);

  SerialUSB.begin(); delay(100);                 //Config serialUSB port
  Wire.begin(1,0);   delay(100);           //initialize i2c comunication port (sda and scl)
  
  setupL3G4200D(2000);                 // Configure L3G4200  - 250, 500 or 2000 deg/sec  
  setup_ADXL345();                     //configure ADXL345 for first time 
  setup_HMC5883L();                    //configure HMC5883 magnometer sensor
  
  Dxl.begin(3);
  
  //Dxl.writeByte(BROADCAST_ID, ID_Change_Address, NEW_ID);
  //Dxl.jointMode(NEW_ID); //jointMode() to use position mode
  Dxl.writeWord(1, Moving_Speed, 300);
  Dxl.writeWord(1, Goal_Postion_Address, 512);
  
  Serial2.begin(115200); 
  
  pinMode(Motor_in1, PWM);
  pinMode(Motor_in2, PWM);
  
  pinMode(Buzzer_pin, OUTPUT);
  
  pinMode(EncoderCHA, INPUT_PULLDOWN);
  pinMode(EncoderCHB, INPUT_PULLDOWN);
  
  pinMode(CameraChannel_pin, OUTPUT);
  
  //attachInterrupt(EncoderCHA, EnoderA, RISING);
  //attachInterrupt(EncoderCHB, EnoderB, RISING);
}

 byte CM9_buffer[15];
 byte inpdata[11];
 unsigned short CheckSum=0;
 byte Buzzer = 100;
 unsigned short DynamixelPose = 512;
 byte Stabllizer = 0;
 byte CameraChannel = 0;
 unsigned short Speed = 0;
 unsigned short Battery =0;
 byte cnt=0;
 unsigned short tmp_ch = 0;
 double TMP=0;
 unsigned long PulseA=0;
 unsigned long PulseB=0;
 int Moving_Speed_M=0;
 unsigned long LastmillisM=0;
//Main Loop
void loop()
{
   digitalWrite(Buzzer_pin,HIGH); 
  Dxl.writeWord(10, Moving_Speed, 400);
  tmp_ch = 0;
  CheckSum=0;
  if(Serial2.available()){
    
    inpdata[0] = Serial2.read();
    inpdata[1] = Serial2.read();
    inpdata[2] = Serial2.read();
    inpdata[3] = Serial2.read();
    inpdata[4] = Serial2.read();
    inpdata[5] = Serial2.read();
    inpdata[6] = Serial2.read();
    inpdata[7] = Serial2.read();
    inpdata[8] = Serial2.read();
    inpdata[9] = Serial2.read();
    inpdata[10] = Serial2.read();
    
    if(inpdata[0] == 255)
      if(inpdata[1] == 200)
      {
        CheckSum = (inpdata[10]<<8) + inpdata[9];
        for(cnt=2 ; cnt<9 ; cnt++) tmp_ch += inpdata[cnt];
        if(CheckSum == tmp_ch)
        {
          Buzzer = (byte)inpdata[2];
          DynamixelPose =(unsigned short)(inpdata[4]<<8)+inpdata[3];
          Stabllizer = (byte)inpdata[5];
          Speed =(unsigned short) (inpdata[7]<<8)+inpdata[6];
          CameraChannel = (byte)inpdata[8];
          
          toggleLED();
          
            tmp_ch = 0;
            CheckSum=0;
          
          //for(cnt=0;cnt<11;cnt++)SerialUSB.print(inpdata[cnt]);
          
          
          CM9_buffer[0]=(byte)255;
          CM9_buffer[1]=(byte)150;
          CM9_buffer[2]=_LOBYTE((signed short)(roll*100));
          CM9_buffer[3]=_HIBYTE((signed short)(roll*100));
          CM9_buffer[4]=_LOBYTE((signed short)(pitch*100));
          CM9_buffer[5]=_HIBYTE((signed short)(pitch*100));
          CM9_buffer[6]=_LOBYTE((signed short)(yaw*100));
          CM9_buffer[7]=_HIBYTE((signed short)(yaw*100));
          Get_Moving_Speed(); 
          CM9_buffer[8]=_LOBYTE((signed short)(Moving_Speed_M)); 
          CM9_buffer[9]=_HIBYTE((signed short)(Moving_Speed_M)); 
          Battery = analogRead(Battery_pin);
          CM9_buffer[10]=_LOBYTE((signed short)(Battery));
          CM9_buffer[11]=_HIBYTE((signed short)(Battery)); 
          CM9_buffer[12]=(byte)Hz;
          for(cnt=2;cnt<13;cnt++)tmp_ch += CM9_buffer[cnt];
          CM9_buffer[13]=_LOBYTE((signed short)(tmp_ch)); 
          CM9_buffer[14]=_HIBYTE((signed short)(tmp_ch));
          
          for(cnt=0;cnt<15;cnt++)Serial2.write(CM9_buffer[cnt]);
          
          tmp_ch = 0;
          CheckSum=0;
        }
      }
  }
 // analogWrite(Motor_in2,1023);
  //analogWrite(Motor_in1,0);
  //delay(5000);
//  pwmWrite(Motor_in2,(65535)*0.1);
//  analogWrite(Motor_in1,0);
  digitalWrite(Buzzer_pin,HIGH);    // 327675 -> off
  Dxl.writeWord(1, Goal_Postion_Address, DynamixelPose);
  
  if(Speed>0 && Speed<=1000 )
  {
    pwmWrite(Motor_in1,(unsigned short)(65.535 * Speed));
    pwmWrite(Motor_in2,0);
  }
  if(Speed>1000 && Speed<=2000)
  { 
    pwmWrite(Motor_in2,(unsigned short)(65.535 * Speed));
    pwmWrite(Motor_in1,0);
  }
  
    if(Speed == 0 )
  {
    pwmWrite(Motor_in2,0);
    pwmWrite(Motor_in1,0);
  }
  
  digitalWrite(CameraChannel_pin,CameraChannel);
  
//          CM9_buffer[0]=(byte)255;
//          CM9_buffer[1]=(byte)150;
//          CM9_buffer[2]=_LOBYTE((signed short)(roll*100));
//          CM9_buffer[3]=_HIBYTE((signed short)(roll*100));
//          CM9_buffer[4]=_LOBYTE((signed short)(pitch*100));
//          CM9_buffer[5]=_HIBYTE((signed short)(pitch*100));
//          CM9_buffer[6]=_LOBYTE((signed short)(yaw*100));
//          CM9_buffer[7]=_HIBYTE((signed short)(yaw*100)); 
//          CM9_buffer[8]=_LOBYTE((signed short)(DynamixelPose)); 
//          CM9_buffer[9]=_HIBYTE((signed short)(DynamixelPose)); 
//          CM9_buffer[10]=_LOBYTE((signed short)(DynamixelPose));
//          CM9_buffer[11]=_HIBYTE((signed short)(DynamixelPose)); 
//          CM9_buffer[12]=(byte)CameraChannel;
//          for(cnt=2;cnt<13;cnt++)tmp_ch += CM9_buffer[cnt];
//          CM9_buffer[13]=_LOBYTE((signed short)(tmp_ch)); 
//          CM9_buffer[14]=_HIBYTE((signed short)(tmp_ch));
//          
//          for(cnt=0;cnt<15;cnt++)Serial2.write(CM9_buffer[cnt]);
//  delay(5);
  ADXL3450_Read();     //Read Accelerometer\

  L3G4200D_Read();     //Read Gyroscope
 
  HMC5883L_Read();     //Read Magnometer

  //IMUUpdate((float)Gyr_x*Gyro_Scale , (float)Gyr_y*Gyro_Scale , (float)Gyr_z*Gyro_Scale , Acc_x , Acc_y , Acc_z, 0 , 0, 0, Hz);                 //No Compass Used
  IMUUpdate((float)Gyr_x*Gyro_Scale , (float)Gyr_y*Gyro_Scale , (float)Gyr_z*Gyro_Scale , Acc_x , Acc_y , Acc_z, Cmp_x , Cmp_y , Cmp_z , Hz); //Compass data are used
  getRollPitchYaw();   //To calculate angle from quaternian. 
  
  //Send_Info(); 
  Hz =(1000 / ( (millis()) - Lastmillis ));
  Lastmillis=millis();
  delay(5);
  //Send_Info_packet();
  
}

void Get_Moving_Speed(void)
{
  
//  Moving_Speed_M = 0;
//  if((millis() - LastmillisM ) == 1000)
//  {
//    Moving_Speed_M = PulseA/450;
//  }
//  LastmillisM=millis();
}

void EnoderA(void)
{  
  PulseA++;
}

void EnoderB(void)
{  
  PulseB++;
}

















