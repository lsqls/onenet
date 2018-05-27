#include <HttpPacket.h>
#include <SoftwareSerial.h>
#include "SIM900.h"
#include "inetGSM.h"
InetGSM inet;



char host[] = "api.heclouds.com";
int port=80;
char onenetresult[100];
int onenetlength=0;
HttpPacketHead packet;
float i=0;//上传的数据
char buff[10];

void setup()
{
  Serial.begin(9600); 
  Serial.println(F("GSM.."));
  if (gsm.begin(9600)) 
    Serial.println(F("READY"));
  else Serial.println(F("IDLE"));
}

 
void loop()
{
  //格式化字符串
  char *value;
  value = dtostrf(i, 3, 2, buff);
  char p[100];
  sprintf(p,"{\"datastreams\":[{\"id\":\"temperature\",\"datapoints\":[{\"value\":%s}]}]}",value);
  packet.setHostAddress("api.heclouds.com");
  packet.setDevId("29982897");
  packet.setAccessKey("RbDdq8h3nqHrA5Q86UZdWkdMBP8=");
  /*create the http message about add datapoint ，生成http数据包，用于上传数据*/
  packet.createCmdPacket(POST, TYPE_DATAPOINT, p);
  if (strlen(packet.content))
    Serial.print(packet.content);
  Serial.print(p);
  Serial.println("\n");
   i=i+10;

  onenetuploaddata(host,port,packet.content,p,onenetresult,onenetlength);
  delay(300);
}

void onenetuploaddata(char* host, int port,char* header,char* raw_request, char* result, int resultlength)
{
  if (inet.attachGPRS("3gnet", "", ""))//原本填的是internet.wind,3gnet是联通的接入点，cmnet为移动接入点
    Serial.println(F("gprs attached"));
  else Serial.println(F("gprs eorro"));
  delay(1000);
  int suif = inet.onenetupload(host, port,header, raw_request, result, resultlength);
  if(suif)
    Serial.println(result);
    Serial.println(F("upload success"));
    
}

