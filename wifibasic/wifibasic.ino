#include <SoftwareSerial.h>

SoftwareSerial mySerial(8, 9); // RX, TX

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // set the data rate for the SoftwareSerial port
  mySerial.begin(4800);
  delay(5000);
 sendcmd("AT+CIPSTART=\"TCP\",\"183.230.40.33\",80\r\n");
 sendcmd("AT+CIPMODE=1\r\n");
}

void loop() { // run over and over

 sendcmd("AT+CIPSEND\r\n");
 sendcmd("POST /devices/29982897/datapoints?type=3 HTTP/1.1\r\n");
 sendcmd("api-key:Aj9NRt=pYZlyZub4M=uLgsJctdI=\r\n");
 sendcmd("Host:api.heclouds.com\r\n");
 sendcmd("Content-Length:20\r\n\r\n");
 sendcmd("{\"temperature\":100}");
}
void sendcmd(char *cmd)
{  
   mySerial.write(cmd);
   delay(500);
   if (mySerial.available()) {
    delay(100);
    Serial.write(mySerial.read());
  }
}
