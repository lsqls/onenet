/*
此代码用于将数据上传到onenet平台
原理是利用arduino软串口通过AT指令控制esp8266，
在上传此代码前请用usbttl或者arduino+代码进行预设置
AT+CWMODE=3 设为 client模式，
AT+CWJAP="WIFI","1234567890" 连接wifi   ，比如此命令说明wifi名称为WIFI，密码是1234567890，在使用时请做出对应修改
AT+CIFSR    查看ip,如果返回正常结果，代表连接成功
AT+UART=4800,8,1,0,0   将esp8266波特率设置为4800，使用默认115200的波特率会出错
代码最好拔插一次数据线再上传，不然会有玄学bug
*/
#include <SoftwareSerial.h>
#define DeviceId "29982897"//修改为你的设备ID
#define apikey "Aj9NRt=pYZlyZub4M=uLgsJctdI="//修改为你设备的api-key

SoftwareSerial mySerial(9,10); // RX-8接esp8266的TX, TX-9接esp8266的RX
void setup() {
  Serial.begin(9600);
  while (!Serial) {
  }
  mySerial.begin(9600);
  delay(5000);
  while(mySerial.available() > 0)
     mySerial.read();
  while(Serial.available() > 0)
     Serial.read();  
  delay(1000);
 sendcmd("AT+CGCLASS=\"B\"\r\n");
 sendcmd("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n");
 sendcmd("AT+CGATT=1\r\n");;
 delay(3000);
 sendcmd("AT+CIPCSGP=1,\"CMNET\"\r\n");
  delay(3000);
  sendcmd("AT+CLPORT=\"TCP\",\"2000\"\r\n");
  
  delay(3000);
  
   randomSeed(analogRead(A0));
}
float i;//上传的数据
void loop() { // run over and over
 i = random(500);
 char *value;
 char buff[10];
 value = dtostrf(i, 3, 2, buff);
 uploaddata("temperature",value);//随机数测试
 uploaddata("Location","{\"lon\":116.2950369010,\"lat\":40.1548081709}");//  40.1548081709,116.2950369010 测试经纬度
 delay(3000);
}
void sendcmd(char *cmd)
{  
   mySerial.write(cmd);
   delay(100);
   //if (mySerial.available()) 
   {
   // delay(100);
   // Serial.write(mySerial.read());
  }
}
void uploaddata(char * dsid,char * data)//dsid为数据流id（名称），data为要上传的数据串
{
 char jsonstring[100];
 char end_c[2];
  end_c[0]=0x1a;
  end_c[1]='\0';
 sprintf(jsonstring,"{\"datastreams\":[{\"id\":\"%s\",\"datapoints\":[{\"value\":%s}]}]}",dsid,data);
 sendcmd("AT+CIPSTART=\"TCP\",\"183.230.40.33\",\"80\"\r\n");
 sendcmd("AT+CIPSEND\r\n");
 sendcmd("POST /devices/");
 sendcmd(DeviceId);
 sendcmd("/datapoints HTTP/1.1\r\n");
 sendcmd("api-key:");
 sendcmd(apikey);
 sendcmd("\r\n");
 sendcmd("Host:api.heclouds.com\r\n");
 sendcmd("Content-Length:");
 mySerial.print(strlen(jsonstring));
 sendcmd("\r\n\r\n");
 sendcmd(jsonstring); 
 mySerial.write(end_c);
 sendcmd("AT+CIPCLOSE=1\r\n");
}

