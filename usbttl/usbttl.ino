/*
此代码可以将arduino开发板作为usbttl模块使用

esp8266模块默认波特率为115200，第一次上传代码软串口波特率应设置为115200
AT+CWMODE=3 设为 client模式，
AT+CWJAP="WIFI","1234567890" 连接wifi   ，比如此命令说明wifi名称为WIFI，密码是1234567890，在使用时请做出对应修改
AT+CIFSR    查看ip,如果返回正常结果，代表连接成功
AT+UART=4800,8,1,0,0   将esp8266波特率设置为4800
*/
#include <SoftwareSerial.h>

SoftwareSerial mySerial(8, 9); // RX-8接esp8266的TX, TX-9接esp8266的RX

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  

  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(4800);//esp8266模块默认波特率为115200，第一次上传代码软串口波特率应设置为115200，修改波特率后再修改此值
  mySerial.println("AT");
}

void loop() { // run over and over
  if (mySerial.available()) {
    delay(100);
    Serial.write(mySerial.read());
  }
  if (Serial.available()) {
     delay(100);
    mySerial.write(Serial.read());
  }
}

