/********************************************************* 
* ESP32, ESP8266(ESP-01S) Blynk total test
* Touch pin 1   ==> Touch0 is T0 which is on GPIO 4 (D4).
* Touch pin 2   ==> Touch6 is T6 which is on GPIO 21 (D14).
* LED pin       ==> D2
* Headcrab 180504
**********************************************************/
#include <functional>
//#include <FunctionalInterrupts.h>

#if defined(ESP32)
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#endif

#if defined (ESP8266)
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#endif

/*********************************************************
 * Class Encoder operates encoders attachet to the board
 * 
 * Headcrab 180516 
*********************************************************/ 

class Encoder;
void got_enc_a_rise(Encoder*);

class Encoder{
  public:
    Encoder(){};
    Encoder(void (*)(), uint8_t pin_a, uint8_t pin_b, uint8_t pin_button=255)
      :pin_a_(pin_a), pin_b_(pin_b), pin_button_(pin_button)
    {
      pinMode(pin_a_, INPUT);
      pinMode(pin_b_, INPUT);
      if(pin_button_!=255) pinMode(pin_button_, INPUT);
    };

    void begin(){
      attachInterrupt(pin_a_, std::bind(got_enc_a_rise, &this), RISING);
      // attachInterrupt(pin_a_, []{got_enc_a_rise();}, RISING);
      // attachInterrupt(pin_a_, &fn, RISING);
      //attachInterrupt(pin_b_,got_enc_b_rise,RISING);
    };
 
    void (*value_change)();

  private:
    uint8_t pin_a_;
    uint8_t pin_b_;
    uint8_t pin_button_;

    // void got_enc_a_fall(){
    //   detachInterrupt(pin_a_);
    //   if(value_change) value_change();
    //   Serial.println("got_enc_a_fall");
    //   attachInterrupt(pin_a_,std::bind(&Encoder::got_enc_a_rise),RISING);
    // }

    // void got_enc_b_rise(){
    //   detachInterrupt(pin_b_);
    //   if(value_change) value_change();
    //   Serial.println("got_enc_b_rise");
    //   attachInterrupt(pin_b_,std::bind(&Encoder::got_enc_b_fall),FALLING);
    // }

    // void got_enc_b_fall(){
    //   detachInterrupt(pin_b_);
    //   if(value_change) value_change();
    //   Serial.println("got_enc_b_fall");
    //   attachInterrupt(pin_b_,std::bind(&Encoder::got_enc_b_rise),RISING);
    // }
};

void got_enc_a_rise(Encoder* temp_encoder){
//      detachInterrupt(pin_a_);
  if(temp_encoder->value_change) temp_encoder->value_change();
  Serial.println("got_enc_a_rise");
  //auto fn = &Encoder::got_enc_a_rise;
//      attachInterrupt(pin_a_, std::bind(&Encoder::got_enc_a_rise,this), FALLING);
}

//char auth_second[] = "f1db0927b6a64509ad87b6487e125d9d";
char auth_first[] = "438d5542e9a842a08674d13101a21825";
char ssid_mobile[] = "Redme";
char pass_mobile[] = "Gogogo1!";
char ssid_home[] = "3507260";
char pass_home[] = "Jw2Cckq8";

#if defined(ESP32)
#define TOUCH_PIN0 4//T0 // GPIO4
#define TOUCH_PIN6 21//T6 // GPIO21
#endif

#define LED_PIN 2
#define ENC_A 0
#define ENC_B 2

BlynkTimer timer;

#if defined(ESP32)
int touch_value = 100;
int freq = 50;
int channel = 0;
int resolution = 8;
int dutyCycle = 21;

void gotTouch0();
void gotTouch6();
void send_cpu_temperature(); // blynk V5
#endif

// void got_enc_a_rise();
// void got_enc_b_rise();
// void got_enc_a_fall();
// void got_enc_b_fall();

void encoder_changed();
Encoder enc(encoder_changed,ENC_A,ENC_B);

void setup(){
  // Blynk.begin(auth_first,ssid_mobile,pass_mobile);
  Blynk.begin(auth_first,ssid_mobile,pass_mobile);
  enc.begin();
  Serial.begin(9600);
  delay(1000); 
  Serial.println("ESP32 Touch Test");

#if defined(ESP32)
  timer.setInterval(1000L,send_cpu_temperature);
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(LED_PIN, channel);
  
  touchAttachInterrupt(TOUCH_PIN0, gotTouch0, resolution);
  touchAttachInterrupt(TOUCH_PIN6, gotTouch6, resolution);
#endif

  // pinMode(ENC_A,INPUT);
  // pinMode(ENC_B,INPUT);
  // attachInterrupt(ENC_A,got_enc_a_rise,RISING);
  // attachInterrupt(ENC_B,got_enc_b_rise,RISING);
//  pinMode(LED_BUILTIN, OUTPUT);

}

void loop(){
  Blynk.run();

#if defined(ESP32)
  ledcWrite(channel, 255);
#endif

  delay(10);
}

// Blynk **********************************************************************

#if defined(ESP32)

// blynk PUSH
void send_cpu_temperature(){
  Blynk.virtualWrite(V0, temperatureRead());
  //Serial.write((int)t);
  //Serial.write("\n");
}

// blynk PULL
BLYNK_READ(V0){
  Blynk.virtualWrite(V5,temperatureRead());
}


// Touch **********************************************************************

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

#endif

// Encoder ********************************************************************

void encoder_changed(){
  Serial.println("encoder_changed");
}

// void got_enc_a_rise(){
//   detachInterrupt(ENC_A);
//   Serial.println("Encoder A");
//   attachInterrupt(ENC_A,got_enc_a_fall,FALLING);
//   delay(10);
// }

// void got_enc_b_rise(){
//   detachInterrupt(ENC_B);
//   Serial.println("Encoder B");
//   attachInterrupt(ENC_B,got_enc_b_fall,FALLING);
//   delay(10);
// }

// void got_enc_a_fall(){
//   detachInterrupt(ENC_A);
//   Serial.println("Encoder A fall");
//   attachInterrupt(ENC_A,got_enc_a_rise,RISING);
//   delay(10);
// }

// void got_enc_b_fall(){
//   detachInterrupt(ENC_B);
//   Serial.println("Encoder B fall");
//   attachInterrupt(ENC_B,got_enc_b_rise,RISING);
//   delay(10);
// }
