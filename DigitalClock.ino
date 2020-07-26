#include <WiFi.h>
#include <time.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define A        13
#define B        14
#define C        33
#define D        26
#define E        27
#define F        12
#define G        32
#define DOT      25
#define S0       35
#define S1       34
#define S2       15
#define S3       4

#define MAX_SEC  59
#define MAX_MIN  59
#define MAX_HOUR 23

const char* ssid       = "YOUR SSID";
const char* password   = "YOUR PASSWORD";

/*NTP setup*/
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 28800);

/*timer setup*/
hw_timer_t* timer = NULL;
portMUX_TYPE timer_mux = portMUX_INITIALIZER_UNLOCKED;

/*clock variables*/
volatile uint8_t sec_count = 0;
uint8_t min_count = 0;
uint8_t hour_count = 0;

uint8_t hours = 0;
uint8_t mins = 0;
uint8_t secs = 0;

String hours_str;
String mins_str;
String secs_str;

/*timer interrupt function*/
void timer_interrupt(void)
{
  portENTER_CRITICAL(&timer_mux);
  sec_count++;
  portEXIT_CRITICAL(&timer_mux);
}

/*function to select a mux channel*/
void channelSelect(uint8_t s)
{
  if(s == 0)
  {
    digitalWrite(S0, LOW);
    digitalWrite(S1, LOW);
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
  }

  else if(s == 1)
  {
    digitalWrite(S0, LOW);
    digitalWrite(S1, LOW);
    digitalWrite(S2, HIGH);
    digitalWrite(S3, LOW);
  }

  else if(s == 2)
  {
    digitalWrite(S0, LOW);
    digitalWrite(S1, HIGH);
    digitalWrite(S2, LOW);
    digitalWrite(S3, HIGH);
  }
  else if(s == 3)
  {
    digitalWrite(S0, LOW);
    digitalWrite(S1, LOW);
    digitalWrite(S2, HIGH);
    digitalWrite(S3, HIGH);
  }
}

/*function to display numbers*/
void Display(uint8_t value)
{
  if(value == 0)
  {
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(E, HIGH);
    digitalWrite(F, HIGH);
    digitalWrite(G, LOW);
  }
  else if(value == 1)
  {
    digitalWrite(A, LOW);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, LOW);
    digitalWrite(E, LOW);
    digitalWrite(F, LOW);
    digitalWrite(G, LOW);
  }
  else if(value == 2)
  {
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(C, LOW);
    digitalWrite(D, HIGH);
    digitalWrite(E, HIGH);
    digitalWrite(F, LOW);
    digitalWrite(G, HIGH);
  }
  else if(value == 3)
  {
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(E, LOW);
    digitalWrite(F, LOW);
    digitalWrite(G, HIGH);
  }
  else if(value == 4)
  {
    digitalWrite(A, LOW);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, LOW);
    digitalWrite(E, LOW);
    digitalWrite(F, HIGH);
    digitalWrite(G, HIGH);
  }
  else if(value == 5)
  {
    digitalWrite(A, HIGH);
    digitalWrite(B, LOW);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(E, LOW);
    digitalWrite(F, HIGH);
    digitalWrite(G, HIGH);
  }
  else if(value == 6)
  {
    digitalWrite(A, HIGH);
    digitalWrite(B, LOW);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(E, HIGH);
    digitalWrite(F, HIGH);
    digitalWrite(G, HIGH);
  }
  else if(value == 7)
  {
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, LOW);
    digitalWrite(E, LOW);
    digitalWrite(F, LOW);
    digitalWrite(G, LOW);
  }
  else if(value == 8)
  {
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(E, HIGH);
    digitalWrite(F, HIGH);
    digitalWrite(G, HIGH);
  }
  else if(value == 9)
  {
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(E, LOW);
    digitalWrite(F, HIGH);
    digitalWrite(G, HIGH);
  }
}

/*function to enable the dot in the display*/
void enableDot(bool value)
{
  if(value == true)
  {
    digitalWrite(DOT, HIGH);
  }
  else
  {
    digitalWrite(DOT, LOW);
  }
}

/*function to convert String to uint8_t*/
uint8_t stringToUint(String value)
{
  int num = atoi(value.c_str());
  uint32_t result = (uint32_t)num;
  uint8_t r = (uint8_t)result;
  return r;
}

void setup()
{  
  /*baud rate*/
  Serial.begin(115200);

  /*WiFi Setup*/
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  
  /*Print local IP address and start web server*/
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  /*getting ntp time*/
  timeClient.update();
  hours_str = timeClient.getHours();
  mins_str = timeClient.getMinutes();
  secs_str = timeClient.getSeconds();
  
  portENTER_CRITICAL(&timer_mux);
  sec_count = stringToUint(secs_str);
  portEXIT_CRITICAL(&timer_mux);

  min_count = stringToUint(mins_str);
  hour_count = stringToUint(hours_str);
  
  /*initialization of timer*/
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &timer_interrupt, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);
  
  /*pin setup*/
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(DOT, OUTPUT);
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
}

void loop() 
{ 
  if(sec_count > MAX_SEC)
  {
    portENTER_CRITICAL(&timer_mux);
    sec_count = 0;
    portEXIT_CRITICAL(&timer_mux);
    
    if(min_count > MAX_MIN)
    {
      min_count = 0;
      
      if(hour_count > MAX_HOUR)
      {
        hour_count = 0;
      }
      else
      {
        hour_count++;
      }
    }
    else
    {
      min_count++;
    }
  }
  
  hours = hour_count;
  mins = min_count; 
  
  channelSelect(1);
  Display(hours%10);
  enableDot(true);
  Display(10);
  hours = hours/10;
  delay(5);
  enableDot(false);
  
  channelSelect(0);
  Display(hours%10);
  hours = hours/10;
  delay(5);
  
  channelSelect(3);
  Display(mins%10);
  mins = mins/10;
  delay(5);

  channelSelect(2);
  Display(mins%10);
  mins = mins/10;
  delay(5);
}
