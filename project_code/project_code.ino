#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
TinyGPSPlus gps; 
SoftwareSerial ss(14, 12); 
const char* ssid = "ADD_YOUR_WIFI_NAME";
const char* password = "ADD_YOUR_WIFI_PASSWORD";
#define FIREBASE_HOST "ADD_YOUR_FIREBASE_HOST_ID"
#define FIREBASE_AUTH "ADD_YOUR_FIREBASE_HOST_AUTH"
#define OLED_RESET LED_BUILTIN 
Adafruit_SSD1306 display(OLED_RESET);
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
float latitude , longitude, speed ;
int year , month , date, hour , minute , second , sat;
String date_str , time_str , lat_str , lng_str , speed_str , sat_str;
int pm;
WiFiServer server(80);
void setup(){
ss.begin(9600);
Serial.begin(115200);
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
//WiFi.mode(WIFI_STA);
//WiFi.config(ip, gateway, subnet, DNS);
display.display();
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0,0);
display.println("ADD_YOUR_TITLE_HERE");
display.display();
delay(2000);
Serial.println();
Serial.print("Connecting to ");
Serial.print(ssid);
display.clearDisplay();
display.setCursor(0,0);
display.println("Connecting to ");
display.println(ssid);
display.display();
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED){
delay(500);
Serial.print(".");
}
Serial.println("");
display.clearDisplay();
display.setCursor(0,0);
display.println("WiFi connected");
display.println("Server started");
display.println(WiFi.localIP());
display.display();
Serial.println("WiFi connected");
server.begin();
Serial.println("Server started");
Serial.println(WiFi.localIP());
Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
delay(3000);
Firebase.setString("name", "ADD_DRIVER_NAME");
if (Firebase.failed()) {
      Serial.print("setting driver name failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
Firebase.setString("bus", "ADD_VECHILE_NUMBER");
if (Firebase.failed()) {
      Serial.print("setting bus number failed");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
Serial.println("bus and driver details sent to data base");
display.clearDisplay();
display.setCursor(0,0); 
display.println("please wait for receving of GPS signal");
display.display();
Serial.println("please wait for receving of GPS signal");
}
void loop(){
while (ss.available() > 0)
if (gps.encode(ss.read())){
if (gps.location.isValid()){
latitude = gps.location.lat();
lat_str = String(latitude , 6);
longitude = gps.location.lng();
lng_str = String(longitude , 6);
speed = gps.speed.kmph();
speed_str = String(speed , 6);
}
if (gps.date.isValid()){
date_str = "";
date = gps.date.day();
month = gps.date.month();
year = gps.date.year();
if (date < 10)
date_str = '0';
date_str += String(date);
date_str += "/";
if (month < 10)
date_str += '0';
date_str += String(month);
date_str += "/";
if (year < 10)
date_str += '0';
date_str += String(year);
}
if (gps.time.isValid()){
time_str = "";
hour = gps.time.hour();
minute = gps.time.minute();
second = gps.time.second();
minute = (minute + 30);
if (minute > 59){
minute = minute - 60;
hour = hour + 1;
}
hour = (hour + 5) ;
if (hour > 23)
hour = hour - 24;
if (hour >= 12)
pm = 1;
else
pm = 0;
hour = hour % 12;
if (hour < 10)
time_str = '0';
time_str += String(hour);
time_str += ":";
if (minute < 10)
time_str += '0';
time_str += String(minute);
time_str += ":";
if (second < 10)
time_str += '0';
time_str += String(second);
if (pm == 1)
time_str += " PM ";
else
time_str += " AM "; 
}

Firebase.setString("speed", speed_str);
if (Firebase.failed()) {
      Serial.print("setting speed failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
Firebase.setString("Date", date_str);
if (Firebase.failed()) {
      Serial.print("setting date failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
Firebase.setString("Time",time_str);
if (Firebase.failed()) {
      Serial.print("setting time failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
  
Firebase.setString("Time1",time_str);
if (Firebase.failed()) {
      Serial.print("setting time failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
Firebase.setString("latitude", lat_str);
if (Firebase.failed()) {
      Serial.print("setting latitude failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
Firebase.setString("longitude", lng_str);
if (Firebase.failed()) {
      Serial.print("setting longitude failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
Serial.print("latitude:");
Serial.println(lat_str);
Serial.print("; longitude:");
Serial.println(lng_str);
Serial.print("speed:");
Serial.print(speed_str);
Serial.println("in KMPH");
Serial.println("speed sent to firebase database");
Serial.println("latitude sent to firebase database");
Serial.println("longitude sent to firebase database"); 
display.clearDisplay();
display.setCursor(0,0);  
display.print("lat:");
display.println(lat_str);
display.print("lng:");
display.println(lng_str);
display.print("speed:");
display.print(speed_str);
display.println(" KMPH");
display.print("Time:");
display.println(time_str);
display.print("Date:");
display.println(date_str);
display.println("---------------------");
display.println("YOU_CAN_ADD_ANY_COMMENTS_TO_PRINT");
display.display();
}
WiFiClient client = server.available();
if(!client){
return;
}
Serial.println("web page is hosting");
String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n <!DOCTYPE html> <html> <head> <title>Live Bus tracking System host page</title> <style>";
s += "a:link {background-color: YELLOW;text-decoration: none;}";
s += "table, th, td {border: 1px solid black;} </style> </head> <body> <h1  style=";
s += "font-size:300%;";
s += " ALIGN=CENTER>DIET college bus live tracking system host page</h1>";
s += "<p ALIGN=CENTER style=""font-size:150%;""";
s += "> <b>Location Details</b></p> <table ALIGN=CENTER style=";
s += "width:50%";
s += "> <tr> <th>Latitude</th>";
s += "<td ALIGN=CENTER >";
s += lat_str;
s += "</td> </tr> <tr> <th>Longitude</th> <td ALIGN=CENTER >";
s += lng_str;
s += "</td> </tr> <tr>  <th>Date</th> <td ALIGN=CENTER >";
s += date_str;
s += "</td></tr> <tr> <th>Time</th> <td ALIGN=CENTER >";
s += time_str;
s += "</td>  </tr> </table> ";
s += "<p align=center><a style=""color:RED;font-size:125%;"" href=""http://maps.google.com/maps?&z=15&mrt=yp&t=k&q=";
s += lat_str;
s += "+";
s += lng_str;
s += """ target=""_top"">Click here!</a> To check the location in Google maps.</p>";
s += "<p ALIGN=CENTER style=""font-size:150%;""";
s += "> <b>  DIET Anakapalle</b></p> <table ALIGN=CENTER style=";
s += "width:50%";
s += "> <p ALIGN=CENTER style=""font-size:90%;""";
s += "> <b> This project done by Bhargavi kolichina and team.</b></p> <table ALIGN=CENTER style=";
s += "width:50%";
s += "> </body> </html> \n";
client.print(s);
}
