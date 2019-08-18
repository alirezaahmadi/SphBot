

/* ------- Register names ------- */
#define ADXL345_DEVID           0x00
#define ADXL345_RESERVED1       0x01
#define ADXL345_THRESH_TAP      0x1d
#define ADXL345_OFSX            0x1e
#define ADXL345_OFSY            0x1f
#define ADXL345_OFSZ            0x20
#define ADXL345_DUR             0x21
#define ADXL345_LATENT          0x22
#define ADXL345_WINDOW          0x23
#define ADXL345_THRESH_ACT      0x24
#define ADXL345_THRESH_INACT    0x25
#define ADXL345_TIME_INACT      0x26
#define ADXL345_ACT_INACT_CTL   0x27
#define ADXL345_THRESH_FF       0x28
#define ADXL345_TIME_FF         0x29
#define ADXL345_TAP_AXES        0x2a
#define ADXL345_ACT_TAP_STATUS  0x2b
#define ADXL345_BW_RATE         0x2c
#define ADXL345_POWER_CTL       0x2d
#define ADXL345_INT_ENABLE      0x2e
#define ADXL345_INT_MAP         0x2f
#define ADXL345_INT_SOURCE      0x30
#define ADXL345_DATA_FORMAT     0x31
#define ADXL345_DATAX0          0x32
#define ADXL345_DATAX1          0x33
#define ADXL345_DATAY0          0x34
#define ADXL345_DATAY1          0x35
#define ADXL345_DATAZ0          0x36
#define ADXL345_DATAZ1          0x37
#define ADXL345_FIFO_CTL        0x38
#define ADXL345_FIFO_STATUS     0x39

#define ADXL345_BW_1600   0xF // 1111
#define ADXL345_BW_800    0xE // 1110
#define ADXL345_BW_400    0xD // 1101  
#define ADXL345_BW_200    0xC // 1100
#define ADXL345_BW_100    0xB // 1011  
#define ADXL345_BW_50     0xA // 1010 
#define ADXL345_BW_25     0x9 // 1001 
#define ADXL345_BW_12     0x8 // 1000 
#define ADXL345_BW_6      0x7 // 0111
#define ADXL345_BW_3      0x6 // 0110
 
// Interrupt PINs ,INT1: 0 ,INT2: 1
#define ADXL345_INT1_PIN 0x00
#define ADXL345_INT2_PIN 0x01

//Interrupt bit position
#define ADXL345_INT_DATA_READY_BIT 0x07
#define ADXL345_INT_SINGLE_TAP_BIT 0x06
#define ADXL345_INT_DOUBLE_TAP_BIT 0x05
#define ADXL345_INT_ACTIVITY_BIT   0x04
#define ADXL345_INT_INACTIVITY_BIT 0x03
#define ADXL345_INT_FREE_FALL_BIT  0x02
#define ADXL345_INT_WATERMARK_BIT  0x01
#define ADXL345_INT_OVERRUNY_BIT   0x00

#define ADXL345_DATA_READY         0x07
#define ADXL345_SINGLE_TAP         0x06
#define ADXL345_DOUBLE_TAP         0x05
#define ADXL345_ACTIVITY           0x04
#define ADXL345_INACTIVITY         0x03
#define ADXL345_FREE_FALL          0x02
#define ADXL345_WATERMARK          0x01
#define ADXL345_OVERRUNY           0x00

#define ADXL345_OK                 1 // no error
#define ADXL345_ERROR              0 // indicates error is predent

#define ADXL345_NO_ERROR           0 // initial state
#define ADXL345_READ_ERROR         1 // problem reading accel
#define ADXL345_BAD_ARG            2 // bad method argument

#define ADXL345_DEVICE            (0x53) // ADXL345 device address
#define ADXL345_TO_READ           (7)    // num of bytes we are going to read each time (two bytes for each axis)

//Accelorometer varible
byte ADXL345_status;   // set when error occurs 
byte error_code;       // Initial state
float gains[3];       // counts to Gs		
float getRate();
byte _buff[6] ;        //6 bytes buffer for saving data read from the device

//Setup accelorometer for first time
void setup_ADXL345()
{
  ADXL345_status = ADXL345_OK;
  error_code = ADXL345_NO_ERROR;
	
  gains[0] = 0.00376390;
  gains[1] = 0.00376009;
  gains[2] = 0.00349265;
  
  ADXL345_powerOn();
  
  //set activity/ inactivity thresholds (0-255)
  ADXL345_setActivityThreshold(75);    //62.5mg per increment
  ADXL345_setInactivityThreshold(75);  //62.5mg per increment
  ADXL345_setTimeInactivity(10);       //How many seconds of no activity is inactive?
 
  //look of activity movement on this axes - 1 == on; 0 == off 
  ADXL345_setActivityX(1);
  ADXL345_setActivityY(1);
  ADXL345_setActivityZ(1);
 
  //look of inactivity movement on this axes - 1 == on; 0 == off
  ADXL345_setInactivityX(1);
  ADXL345_setInactivityY(1);
  ADXL345_setInactivityZ(1);
 
  //look of tap movement on this axes - 1 == on; 0 == off
  ADXL345_setTapDetectionOnX(0);
  ADXL345_setTapDetectionOnY(0);
  ADXL345_setTapDetectionOnZ(1);
 
  //set values for what is a tap, and what is a double tap (0-255)
  ADXL345_setTapThreshold(50);      //62.5mg per increment
  ADXL345_setTapDuration(15);       //625μs per increment
  ADXL345_setDoubleTapLatency(80);  //1.25ms per increment
  ADXL345_setDoubleTapWindow(200);  //1.25ms per increment
 
  //set values for what is considered freefall (0-255)
  ADXL345_setFreeFallThreshold(7);  //(5 - 9) recommended - 62.5mg per increment
  ADXL345_setFreeFallDuration(45);  //(20 - 70) recommended - 5ms per increment
 
  //setting all interupts to take place on int pin 1
  //I had issues with int pin 2, was unable to reset it
  ADXL345_setInterruptMapping( ADXL345_INT_SINGLE_TAP_BIT,   ADXL345_INT1_PIN );
  ADXL345_setInterruptMapping( ADXL345_INT_DOUBLE_TAP_BIT,   ADXL345_INT1_PIN );
  ADXL345_setInterruptMapping( ADXL345_INT_FREE_FALL_BIT,    ADXL345_INT1_PIN );
  ADXL345_setInterruptMapping( ADXL345_INT_ACTIVITY_BIT,     ADXL345_INT1_PIN );
  ADXL345_setInterruptMapping( ADXL345_INT_INACTIVITY_BIT,   ADXL345_INT1_PIN );
 
  //register interupt actions - 1 == on; 0 == off  
  ADXL345_setInterrupt( ADXL345_INT_SINGLE_TAP_BIT, 1);
  ADXL345_setInterrupt( ADXL345_INT_DOUBLE_TAP_BIT, 1);
  ADXL345_setInterrupt( ADXL345_INT_FREE_FALL_BIT,  1);
  ADXL345_setInterrupt( ADXL345_INT_ACTIVITY_BIT,   1);
  ADXL345_setInterrupt( ADXL345_INT_INACTIVITY_BIT, 1);
  
}

//set power on configuration on accelorometer
void ADXL345_powerOn() 
{
  //Join i2c bus (address optional for master) ,Turning on the ADXL345
  ADXL345_writeTo(ADXL345_POWER_CTL, 0);      
  ADXL345_writeTo(ADXL345_POWER_CTL, 16);
  ADXL345_writeTo(ADXL345_POWER_CTL, 8); 
}

double AXLNorm=0.0f;
// Reads the acceleration into three variable x, y and z
void ADXL3450_Read()
{
  byte xMSB =(byte) readRegister(ADXL345_DEVICE, ADXL345_DATAX1);
  byte xLSB =(byte) readRegister(ADXL345_DEVICE, ADXL345_DATAX0);
  Acc_x =(signed short)((xMSB << 8) | xLSB);

  byte yMSB =(byte) readRegister(ADXL345_DEVICE, ADXL345_DATAY1);
  byte yLSB =(byte) readRegister(ADXL345_DEVICE, ADXL345_DATAY0);
  Acc_y =(signed short)((yMSB << 8) | yLSB);

  byte zMSB =(byte) readRegister(ADXL345_DEVICE, ADXL345_DATAZ1);
  byte zLSB =(byte) readRegister(ADXL345_DEVICE, ADXL345_DATAZ0);
  Acc_z =(signed short)((zMSB << 8) | zLSB);
  
//  AXLNorm = (double)sqrt((Acc_x*Acc_x)+(Acc_y*Acc_y)+(Acc_z*Acc_z));
//  Acc_x /= AXLNorm;
//  Acc_y /= AXLNorm;
//  Acc_z /= AXLNorm;
}

// Writes val to address register on device
void ADXL345_writeTo(byte address, byte val) 
{
	Wire.beginTransmission(ADXL345_DEVICE); // start transmission to device 
	Wire.write(address);             // send register address
	Wire.write(val);                 // send value to write
	Wire.endTransmission();         // end transmission
}

// Reads num bytes starting from address register on device in to _buff array
void ADXL345_readFrom(byte address, byte num, byte _buff[]) 
{
	Wire.beginTransmission(ADXL345_DEVICE); // start transmission to device 
	Wire.write(address);             // sends address to read from
	Wire.endTransmission();         // end transmission
	
	Wire.beginTransmission(ADXL345_DEVICE);   // start transmission to device
	Wire.requestFrom(ADXL345_DEVICE, num);    // request 6 bytes from device
	
	byte _i = 0;
	while(Wire.available())              // device may send less than requested (abnormal)
	{ 
		_buff[_i] = Wire.read();    // receive a byte
		_i++;
	}
	if(_i != num)
        {
		ADXL345_status = ADXL345_ERROR;
		error_code = ADXL345_READ_ERROR;
	}
	Wire.endTransmission();         // end transmission
}

// Sets the range setting, possible values are: 2, 4, 8, 16
void ADXL345_setRangeSetting(int val) 
{
	byte _s;
	byte _b;
	
	switch (val) 
        {
		case 2:  
			_s = B00000000; 
			break;
		case 4:  
			_s = B00000001; 
			break;
		case 8:  
			_s = B00000010; 
			break;
		case 16: 
			_s = B00000011; 
			break;
		default: 
			_s = B00000000;
	}
	ADXL345_readFrom(ADXL345_DATA_FORMAT, 1, &_b);
	_s |= (_b & B11101100);
	ADXL345_writeTo(ADXL345_DATA_FORMAT, _s);
}

// Sets the INT_INVERT bit
// if set to 0 sets the interrupts to active high
// if set to 1 sets the interrupts to active low
void ADXL345_setInterruptLevelBit(bool interruptLevelBit) 
{
	ADXL345_setRegisterBit(ADXL345_DATA_FORMAT, 5, interruptLevelBit);
}

// Sets the FULL_RES bit
// if set to 1, the device is in full resolution mode, where the output resolution increases with the
//   g range set by the range bits to maintain a 4mg/LSB scal factor
// if set to 0, the device is in 10-bit mode, and the range buts determine the maximum g range
//   and scale factor
void ADXL345_setFullResBit(bool fullResBit) 
{
	ADXL345_setRegisterBit(ADXL345_DATA_FORMAT, 3, fullResBit);
}


// Sets the JUSTIFY bit
// if sets to 1 selects the left justified mode
// if sets to 0 selects right justified mode with sign extension
void ADXL345_setJustifyBit(bool justifyBit) 
{
	ADXL345_setRegisterBit(ADXL345_DATA_FORMAT, 2, justifyBit);
}

// Sets the THRESH_TAP byte value
// it should be between 0 and 255
// the scale factor is 62.5 mg/LSB
// A value of 0 may result in undesirable behavior
void ADXL345_setTapThreshold(int tapThreshold) 
{
	tapThreshold = constrain(tapThreshold,0,255);
	byte _b = byte (tapThreshold);
	ADXL345_writeTo(ADXL345_THRESH_TAP, _b);  
}

// set/get the gain for each axis in Gs / count
void ADXL345_setAxisGains(double *_gains)
{
	int i;
	for(i = 0; i < 3; i++)
        {
		gains[i] = _gains[i];
	}
}

// Sets the OFSX, OFSY and OFSZ bytes
// OFSX, OFSY and OFSZ are user offset adjustments in twos complement format with
// a scale factor of 15,6mg/LSB
// OFSX, OFSY and OFSZ should be comprised between 
void ADXL345_setAxisOffset(int x, int y, int z) 
{
	ADXL345_writeTo(ADXL345_OFSX, byte (x));  
	ADXL345_writeTo(ADXL345_OFSY, byte (y));  
	ADXL345_writeTo(ADXL345_OFSZ, byte (z));  
}

// Gets the OFSX, OFSY and OFSZ bytes
void ADXL345_getAxisOffset(int* x, int* y, int*z) 
{
	byte _b;
	ADXL345_readFrom(ADXL345_OFSX, 1, &_b);  
	*x = int (_b);
	ADXL345_readFrom(ADXL345_OFSY, 1, &_b);  
	*y = int (_b);
	ADXL345_readFrom(ADXL345_OFSZ, 1, &_b);  
	*z = int (_b);
}

// Sets the DUR byte
// The DUR byte contains an unsigned time value representing the maximum time
// that an event must be above THRESH_TAP threshold to qualify as a tap event
// The scale factor is 625µs/LSB
// A value of 0 disables the tap/double tap funcitons. Max value is 255.
void ADXL345_setTapDuration(int tapDuration) 
{
	tapDuration = constrain(tapDuration,0,255);
	byte _b = byte (tapDuration);
	ADXL345_writeTo(ADXL345_DUR, _b);  
}

// Gets the DUR byte
int ADXL345_getTapDuration() 
{
	byte _b;
	ADXL345_readFrom(ADXL345_DUR, 1, &_b);  
	return int (_b);
}

// Sets the latency (latent register) which contains an unsigned time value
// representing the wait time from the detection of a tap event to the start
// of the time window, during which a possible second tap can be detected.
// The scale factor is 1.25ms/LSB. A value of 0 disables the double tap function.
// It accepts a maximum value of 255.
void ADXL345_setDoubleTapLatency(int doubleTapLatency) 
{
	byte _b = byte (doubleTapLatency);
	ADXL345_writeTo(ADXL345_LATENT, _b);  
}

// Gets the Latent value
int ADXL345_getDoubleTapLatency() 
{
	byte _b;
	ADXL345_readFrom(ADXL345_LATENT, 1, &_b);  
	return int (_b);
}

// Sets the Window register, which contains an unsigned time value representing
// the amount of time after the expiration of the latency time (Latent register)
// during which a second valud tap can begin. The scale factor is 1.25ms/LSB. A
// value of 0 disables the double tap function. The maximum value is 255.
void ADXL345_setDoubleTapWindow(int doubleTapWindow) 
{
	doubleTapWindow = constrain(doubleTapWindow,0,255);
	byte _b = byte (doubleTapWindow);
	ADXL345_writeTo(ADXL345_WINDOW, _b);  
}

// Gets the Window register
int ADXL345_getDoubleTapWindow() 
{
	byte _b;
	ADXL345_readFrom(ADXL345_WINDOW, 1, &_b);  
	return int (_b);
}

// Sets the THRESH_ACT byte which holds the threshold value for detecting activity.
// The data format is unsigned, so the magnitude of the activity event is compared 
// with the value is compared with the value in the THRESH_ACT register. The scale
// factor is 62.5mg/LSB. A value of 0 may result in undesirable behavior if the 
// activity interrupt is enabled. The maximum value is 255.
void ADXL345_setActivityThreshold(int activityThreshold) 
{
	activityThreshold = constrain(activityThreshold,0,255);
	byte _b = byte (activityThreshold);
	ADXL345_writeTo(ADXL345_THRESH_ACT, _b);  
}

// Gets the THRESH_ACT byte
int ADXL345_getActivityThreshold() 
{
	byte _b;
	ADXL345_readFrom(ADXL345_THRESH_ACT, 1, &_b);  
	return int (_b);
}

// Sets the THRESH_INACT byte which holds the threshold value for detecting inactivity.
// The data format is unsigned, so the magnitude of the inactivity event is compared 
// with the value is compared with the value in the THRESH_INACT register. The scale
// factor is 62.5mg/LSB. A value of 0 may result in undesirable behavior if the 
// inactivity interrupt is enabled. The maximum value is 255.
void ADXL345_setInactivityThreshold(int inactivityThreshold) 
{
	inactivityThreshold = constrain(inactivityThreshold,0,255);
	byte _b = byte (inactivityThreshold);
	ADXL345_writeTo(ADXL345_THRESH_INACT, _b);  
}

// Gets the THRESH_INACT byte
int ADXL345_getInactivityThreshold() 
{
	byte _b;
	ADXL345_readFrom(ADXL345_THRESH_INACT, 1, &_b);  
	return int (_b);
}

// Sets the TIME_INACT register, which contains an unsigned time value representing the
// amount of time that acceleration must be less thant the value in the THRESH_INACT
// register for inactivity to be declared. The scale factor is 1sec/LSB. The value must
// be between 0 and 255.
void ADXL345_setTimeInactivity(int timeInactivity) 
{
	timeInactivity = constrain(timeInactivity,0,255);
	byte _b = byte (timeInactivity);
	ADXL345_writeTo(ADXL345_TIME_INACT, _b);  
}

// Gets the TIME_INACT register
int ADXL345_getTimeInactivity() 
{
	byte _b;
	ADXL345_readFrom(ADXL345_TIME_INACT, 1, &_b);  
	return int (_b);
}

// Sets the THRESH_FF register which holds the threshold value, in an unsigned format, for
// free-fall detection. The root-sum-square (RSS) value of all axes is calculated and
// compared whith the value in THRESH_FF to determine if a free-fall event occured. The 
// scale factor is 62.5mg/LSB. A value of 0 may result in undesirable behavior if the free-fall
// interrupt is enabled. The maximum value is 255.
void ADXL345_setFreeFallThreshold(int freeFallThreshold) 
{
	freeFallThreshold = constrain(freeFallThreshold,0,255);
	byte _b = byte (freeFallThreshold);
	ADXL345_writeTo(ADXL345_THRESH_FF, _b);  
}

// Gets the THRESH_FF register.
int ADXL345_getFreeFallThreshold() 
{
	byte _b;
	ADXL345_readFrom(ADXL345_THRESH_FF, 1, &_b);  
	return int (_b);
}

// Sets the TIME_FF register, which holds an unsigned time value representing the minimum
// time that the RSS value of all axes must be less than THRESH_FF to generate a free-fall 
// interrupt. The scale factor is 5ms/LSB. A value of 0 may result in undesirable behavior if
// the free-fall interrupt is enabled. The maximum value is 255.
void ADXL345_setFreeFallDuration(int freeFallDuration) 
{
	freeFallDuration = constrain(freeFallDuration,0,255);  
	byte _b = byte (freeFallDuration);
	ADXL345_writeTo(ADXL345_TIME_FF, _b);  
}

// Gets the TIME_FF register.
int ADXL345_getFreeFallDuration() 
{
	byte _b;
	ADXL345_readFrom(ADXL345_TIME_FF, 1, &_b);  
	return int (_b);
}
bool ADXL345_isActivityXEnabled() 
{  
	return ADXL345_getRegisterBit(ADXL345_ACT_INACT_CTL, 6); 
}
bool ADXL345_isActivityYEnabled() 
{  
	return ADXL345_getRegisterBit(ADXL345_ACT_INACT_CTL, 5); 
}
bool ADXL345_isActivityZEnabled() 
{  
	return ADXL345_getRegisterBit(ADXL345_ACT_INACT_CTL, 4); 
}
bool ADXL345_isInactivityXEnabled() 
{  
	return ADXL345_getRegisterBit(ADXL345_ACT_INACT_CTL, 2); 
}
bool ADXL345_isInactivityYEnabled() 
{  
	return ADXL345_getRegisterBit(ADXL345_ACT_INACT_CTL, 1); 
}
bool ADXL345_isInactivityZEnabled() 
{  
	return ADXL345_getRegisterBit(ADXL345_ACT_INACT_CTL, 0); 
}
void ADXL345_setActivityX(bool state) 
{  
	ADXL345_setRegisterBit(ADXL345_ACT_INACT_CTL, 6, state); 
}
void ADXL345_setActivityY(bool state) 
{  
	ADXL345_setRegisterBit(ADXL345_ACT_INACT_CTL, 5, state); 
}
void ADXL345_setActivityZ(bool state) 
{  
	ADXL345_setRegisterBit(ADXL345_ACT_INACT_CTL, 4, state); 
}
void ADXL345_setInactivityX(bool state) 
{  
	ADXL345_setRegisterBit(ADXL345_ACT_INACT_CTL, 2, state); 
}
void ADXL345_setInactivityY(bool state) 
{  
	ADXL345_setRegisterBit(ADXL345_ACT_INACT_CTL, 1, state); 
}
void ADXL345_setInactivityZ(bool state) 
{  
	ADXL345_setRegisterBit(ADXL345_ACT_INACT_CTL, 0, state); 
}
bool ADXL345_isActivityAc() 
{ 
	return ADXL345_getRegisterBit(ADXL345_ACT_INACT_CTL, 7); 
}
bool ADXL345_isInactivityAc()
{ 
	return ADXL345_getRegisterBit(ADXL345_ACT_INACT_CTL, 3); 
}
void ADXL345_setActivityAc(bool state) 
{  
	ADXL345_setRegisterBit(ADXL345_ACT_INACT_CTL, 7, state); 
}
void ADXL345_setInactivityAc(bool state) 
{  
	ADXL345_setRegisterBit(ADXL345_ACT_INACT_CTL, 3, state); 
}
bool ADXL345_getSuppressBit()
{ 
	return ADXL345_getRegisterBit(ADXL345_TAP_AXES, 3); 
}
void ADXL345_setSuppressBit(bool state) 
{  
	ADXL345_setRegisterBit(ADXL345_TAP_AXES, 3, state); 
}
bool ADXL345_isTapDetectionOnX()
{ 
	return ADXL345_getRegisterBit(ADXL345_TAP_AXES, 2); 
}
void ADXL345_setTapDetectionOnX(bool state) 
{  
	ADXL345_setRegisterBit(ADXL345_TAP_AXES, 2, state); 
}
bool ADXL345_isTapDetectionOnY()
{ 
	return ADXL345_getRegisterBit(ADXL345_TAP_AXES, 1); 
}
void ADXL345_setTapDetectionOnY(bool state) 
{  
	ADXL345_setRegisterBit(ADXL345_TAP_AXES, 1, state); 
}
bool ADXL345_isTapDetectionOnZ()
{ 
	return ADXL345_getRegisterBit(ADXL345_TAP_AXES, 0); 
}
void ADXL345_setTapDetectionOnZ(bool state) 
{  
	ADXL345_setRegisterBit(ADXL345_TAP_AXES, 0, state); 
}

bool ADXL345_isActivitySourceOnX()
{ 
	return ADXL345_getRegisterBit(ADXL345_ACT_TAP_STATUS, 6); 
}
bool ADXL345_isActivitySourceOnY()
{ 
	return ADXL345_getRegisterBit(ADXL345_ACT_TAP_STATUS, 5); 
}
bool ADXL345_isActivitySourceOnZ()
{ 
	return ADXL345_getRegisterBit(ADXL345_ACT_TAP_STATUS, 4); 
}

bool ADXL345_isTapSourceOnX()
{ 
	return ADXL345_getRegisterBit(ADXL345_ACT_TAP_STATUS, 2); 
}
bool ADXL345_isTapSourceOnY()
{ 
	return ADXL345_getRegisterBit(ADXL345_ACT_TAP_STATUS, 1); 
}
bool ADXL345_isTapSourceOnZ()
{ 
	return ADXL345_getRegisterBit(ADXL345_ACT_TAP_STATUS, 0); 
}

bool ADXL345_isAsleep()
{ 
	return ADXL345_getRegisterBit(ADXL345_ACT_TAP_STATUS, 3); 
}

bool ADXL345_isLowPower()
{ 
	return ADXL345_getRegisterBit(ADXL345_BW_RATE, 4); 
}
void ADXL345_setLowPower(bool state) 
{  
	ADXL345_setRegisterBit(ADXL345_BW_RATE, 4, state); 
}

double ADXL345_getRate()
{
	byte _b;
	ADXL345_readFrom(ADXL345_BW_RATE, 1, &_b);
	_b &= B00001111;
	return (pow(2,((int) _b)-6)) * 6.25;
}

void ADXL345_setRate(double rate)
{
	byte _b,_s;
	int v = (int) (rate / 6.25);
	int r = 0;
	while (v >>= 1)
	{
		r++;
	}
	if (r <= 9) 
        { 
		ADXL345_readFrom(ADXL345_BW_RATE, 1, &_b);
		_s = (byte) (r + 6) | (_b & B11110000);
		ADXL345_writeTo(ADXL345_BW_RATE, _s);
	}
}

void ADXL345_set_bw(byte bw_code)
{
	if((bw_code < ADXL345_BW_3) || (bw_code > ADXL345_BW_1600))
        {
		ADXL345_status = false;
		error_code = ADXL345_BAD_ARG;
	}
	else
        {
		ADXL345_writeTo(ADXL345_BW_RATE, bw_code);
	}
}

byte ADXL345_get_bw_code()
{
	byte bw_code;
	ADXL345_readFrom(ADXL345_BW_RATE, 1, &bw_code);
	return bw_code;
}


//Used to check if action was triggered in interrupts
//Example triggered(interrupts, ADXL345_SINGLE_TAP);
bool ADXL345_triggered(byte interrupts, int mask)
{
	return ((interrupts >> mask) & 1);
}


/*
 ADXL345_DATA_READY
 ADXL345_SINGLE_TAP
 ADXL345_DOUBLE_TAP
 ADXL345_ACTIVITY
 ADXL345_INACTIVITY
 ADXL345_FREE_FALL
 ADXL345_WATERMARK
 ADXL345_OVERRUNY
 */

byte ADXL345_getInterruptSource() 
{
	byte _b;
	ADXL345_readFrom(ADXL345_INT_SOURCE, 1, &_b);
	return _b;
}

bool ADXL345_getInterruptSource(byte interruptBit) 
{
	return ADXL345_getRegisterBit(ADXL345_INT_SOURCE,interruptBit);
}

bool ADXL345_getInterruptMapping(byte interruptBit) 
{
	return ADXL345_getRegisterBit(ADXL345_INT_MAP,interruptBit);
}

// Set the mapping of an interrupt to pin1 or pin2
// eg: setInterruptMapping(ADXL345_INT_DOUBLE_TAP_BIT,ADXL345_INT2_PIN);
void ADXL345_setInterruptMapping(byte interruptBit, bool interruptPin) 
{
	ADXL345_setRegisterBit(ADXL345_INT_MAP, interruptBit, interruptPin);
}

bool ADXL345_isInterruptEnabled(byte interruptBit) 
{
	return ADXL345_getRegisterBit(ADXL345_INT_ENABLE,interruptBit);
}

void ADXL345_setInterrupt(byte interruptBit, bool state) 
{
	ADXL345_setRegisterBit(ADXL345_INT_ENABLE, interruptBit, state);
}

void ADXL345_setRegisterBit(byte regAdress, int bitPos, bool state) 
{
	byte _b;
	ADXL345_readFrom(regAdress, 1, &_b);
	if (state) {
		_b |= (1 << bitPos);  // forces nth bit of _b to be 1.  all other bits left alone.
	} 
	else 
        {
		_b &= ~(1 << bitPos); // forces nth bit of _b to be 0.  all other bits left alone.
	}
	ADXL345_writeTo(regAdress, _b);  
}

bool ADXL345_getRegisterBit(byte regAdress, int bitPos) 
{
	byte _b;
	ADXL345_readFrom(regAdress, 1, &_b);
	return ((_b >> bitPos) & 1);
}
