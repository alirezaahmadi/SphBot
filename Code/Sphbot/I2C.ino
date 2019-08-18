

//Write register to i2c device with specific i2c
void writeRegister(byte deviceAddress, byte address, byte val) 
{
    Wire.beginTransmission(deviceAddress); // start transmission to device 
    Wire.write(address);       // send register address
    Wire.write(val);           // send value to write
    Wire.endTransmission();    // end transmission
}

//Read a byte from i2c device with specific address
byte readRegister(byte deviceAddress, byte address)
{
    Wire.beginTransmission(deviceAddress);
    Wire.write(address);                 // register to read
    Wire.endTransmission();
    Wire.requestFrom(deviceAddress, 2);  // read a byte
    while(!Wire.available()) { } //wait until ready
    return Wire.read();
}
