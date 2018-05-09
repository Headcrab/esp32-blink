/***************************************************** 
* ESP32 Total test
* Touch pin ==> Touch0 is T0 which is on GPIO 4 (D4).
* LED pin   ==> D2
* 
* Headcrab 180504
*****************************************************/
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

//char auth[] = "f1db0927b6a64509ad87b6487e125d9d";
char auth[] = "438d5542e9a842a08674d13101a21825";
char ssid[] = "Redme";
char pass[] = "Gogogo1!";

#define TOUCH_PIN0 T0 // GPIO4
#define TOUCH_PIN6 T6 // GPIO21
#define LED_PIN 2
int touch_value = 100;

int freq = 50;
int channel = 0;
int resolution = 8;
int dutyCycle = 21;

void gotTouch0();
void gotTouch6();

BLYNK_READ(V3)
{
  Blynk.virtualWrite(V3,temperatureRead());
}


void setup()
{
  Blynk.begin(auth,ssid,pass);
  Serial.begin(9600);
  delay(1000); 
  Serial.println("ESP32 Touch Test");
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(LED_PIN, channel);
  
  touchAttachInterrupt(TOUCH_PIN0, gotTouch0, resolution);
  touchAttachInterrupt(TOUCH_PIN6, gotTouch6, resolution);

}

void loop()
{
  Blynk.run();
  ledcWrite(channel, 0);
  delay(10);
}

void gotTouch0(){
  int t = touchRead(TOUCH_PIN0);
  Serial.println("Touch0 - ");
  Serial.println(t);
}

void gotTouch6(){
  int t = touchRead(TOUCH_PIN6);
  Serial.println("Touch6 - ");
  Serial.println(t);
}