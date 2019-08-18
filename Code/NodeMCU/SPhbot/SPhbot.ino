#include <ESP8266WiFi.h>
#include <string>

 byte Wifi_buffer[20480]; 
 byte CM9_buffer[15];
  
 int CheckSum=0;
 byte Buzzer = 0;
 int DynamixelPose = 0;
 byte Stablizer = 0;
 byte CameraChannel = 0;
 int Speed = 0;
 int Battery =0;

 
const char* ssid = "Dlink-A2";
const char* password = "093616099900";

WiFiServer server(7676);
WiFiClient client;
 
void setup() {
  Serial.begin(115200);
  delay(10);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

}

void loop() {
  //if a client has connected
  client = server.available();
//  if (!client) {
//    return;
//  }
 
  // Wait until the client sends some data
  //Serial.println("new client");
//  while(!client.available()){
//    delay(1);
//  }
  if (client) {
    
    //Serial.println("Client connected.");
    
    while (client.connected()) {  
      
      if (client.available()) {
        //get data from wifi
        client.available();
        client.readBytes(Wifi_buffer,11);
        client.flush();
        
        //write data to CM9 here
        Serial.print((char)Wifi_buffer[0]);
        Serial.print((char)Wifi_buffer[1]);
        Serial.print((char)Wifi_buffer[2]);
        Serial.print((char)Wifi_buffer[3]);
        Serial.print((char)Wifi_buffer[4]);
        Serial.print((char)Wifi_buffer[5]);
        Serial.print((char)Wifi_buffer[6]);
        Serial.print((char)Wifi_buffer[7]);
        Serial.print((char)Wifi_buffer[8]);
        Serial.print((char)Wifi_buffer[9]);
        Serial.print((char)Wifi_buffer[10]);
        
        
        if(Serial.available()){
        CM9_buffer[0]=(byte)Serial.read();
        CM9_buffer[1]=(byte)Serial.read();
        CM9_buffer[2]=(byte)Serial.read();
        CM9_buffer[3]=(byte)Serial.read();
        CM9_buffer[4]=(byte)Serial.read();
        CM9_buffer[5]=(byte)Serial.read();
        CM9_buffer[6]=(byte)Serial.read();
        CM9_buffer[7]=(byte)Serial.read();
        CM9_buffer[8]=(byte)Serial.read();
        CM9_buffer[9]=(byte)Serial.read();
        CM9_buffer[10]=(byte)Serial.read();
        CM9_buffer[11]=(byte)Serial.read();
        CM9_buffer[12]=(byte)Serial.read();
        CM9_buffer[13]=(byte)Serial.read();
        CM9_buffer[14]=(byte)Serial.read();
        if(CM9_buffer[0]==255)
          if(CM9_buffer[1]==150)
          {
            //for(byte tmpn=0;tmpn<15;tmpn++)Serial.println((byte)CM9_buffer[tmpn]);
              client.println("");
              client.print(CM9_buffer[0]);client.print(",");  
              client.print(CM9_buffer[1]);client.print(",");
              client.print(CM9_buffer[2]);client.print(",");
              client.print(CM9_buffer[3]);client.print(",");
              client.print(CM9_buffer[4]);client.print(",");
              client.print(CM9_buffer[5]);client.print(",");
              client.print(CM9_buffer[6]);client.print(",");
              client.print(CM9_buffer[7]);client.print(",");
              client.print(CM9_buffer[8]);client.print(",");
              client.print(CM9_buffer[9]);client.print(",");
              client.print(CM9_buffer[10]);client.print(",");
              client.print(CM9_buffer[11]);client.print(",");
              client.print(CM9_buffer[12]);client.print(",");
              client.print(CM9_buffer[13]);client.print(",");
              client.println(CM9_buffer[14]);
              client.flush();
          }
          else
          {
            for(byte cnt=0;cnt<15;cnt++)CM9_buffer[cnt] = 0;
          }
        }
      }
    }
  }
}

 
