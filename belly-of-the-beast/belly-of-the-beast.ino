// @author Leon Santen
// @project Interactive Art
// To open port on Linux: sudo chmod a+rw /dev/ttyACM0 

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

#define LED_PIN    6 // Which pin on the Arduino is connected to the NeoPixels?
#define LED_COUNT 8  // How many NeoPixels are attached to the Arduino? 

Adafruit_NeoPixel strip1(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

const int TRIGGER = 8;
const int LED = 13;
uint32_t serial_freq_time = 0;
float measured_serial_freq;
uint32_t US_frequency = 9; //frequency: clicks per second
uint32_t LED_frequency = 10; //frequency of flickering for LEDS
uint32_t current_millis; 
uint32_t US_last_task_millis = 0;
uint32_t LED_last_task_millis = 0;
uint32_t US_task_length = 1000/US_frequency; //task length for US sensors
uint32_t LED_task_length = 1000/LED_frequency; //task length for LED flickering
int receivedSerial; // received serial input from max msp

int state = 0;
String subState;
int oldState = 0;
String oldSubState;

int number_of_sensors = 1; //the amount of ultrasonic sensors used
int sensor_pins[] = {A2}; // input pins for sensor data - pin number refers to sensor from left to right in direction of traffic

void setup()
{
  pinMode(TRIGGER, OUTPUT);
  digitalWrite(TRIGGER, HIGH);
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  strip1.begin();
  strip1.show(); // Initialize all pixels to 'off'

  state = 18;
}
void loop()
{
  current_millis = millis(); //current time for all frequencies later in the code

  
  if (state==1){
    //light settings for state 1
    if (current_millis - LED_last_task_millis >= LED_task_length){
      LED_last_task_millis = millis();

      int LED_color_array[5][3] = { {255,102,25},
                                    {255,121,25},
                                    {255,90,0},
                                    {255,77,0},
                                    {255,101,66} };
      uint32_t LED_random_color = random(0,4);
      uint32_t orange = strip1.Color(LED_color_array[LED_random_color][0],LED_color_array[LED_random_color][1],LED_color_array[LED_random_color][2]);
      strip1.fill(orange, 0, 7);
    }    
  }
  
  if (state==2){
    //light settings for state 2
    uint32_t red_orange = strip1.Color(255, 45, 0);
    strip1.fill(red_orange, 0, 7);      
  }
  
  if (state>=3 and state < 20){
    //light settings for state 3 - 9 - higher states will dimm the light
    uint32_t red_R = 255 / ((float(state)/1.0) - 2.0);
    uint32_t red_G = 10 / (float(state)/3.0);
    uint32_t red_dimmable = strip1.Color(red_R, red_G, 0);
    strip1.fill(red_dimmable, 0, 7);    
  }

  if (state==20){
    //light settings for state 3 - 9 - higher states will dimm the light
    uint32_t red_dimmable = strip1.Color(0, 0, 0);
    strip1.fill(red_dimmable, 0, 7);    
  }
  

  // Get signal from US sensors
  if (state >= 1){
    
  
    // send trigger and run code in if statement with frequency --> US_frequency
    if (current_millis - US_last_task_millis >= US_task_length){
      digitalWrite(LED, HIGH);
      US_last_task_millis = millis();
      // generate the pulse to trigger the sensor
      digitalWrite(TRIGGER, LOW);
      delayMicroseconds(50);
      digitalWrite(TRIGGER, HIGH);
      delayMicroseconds(50);
    
      unsigned int sensor_data[number_of_sensors];
      String data_string; // string that contains data
  
      // loop through sensors, read data, convert to mm, print data    
      for (int i=0; i < number_of_sensors; i++){
        sensor_data[i] = analogRead(sensor_pins[i]);
        sensor_data[i] = (unsigned int)(((unsigned long)sensor_data[i] * 5000)/1024); //change from analog value to voltage
        sensor_data[i] = (unsigned int)((float)sensor_data[i] / 0.33f); //0.33mV equals 1mm
        //sensor_data[i] = (unsigned int)((float)sensor_data[i] * 0.438f);     
        //sensor_data[i] = sensor_data[i] - 25
        
        data_string.concat(sensor_data[i]);
        if (i < number_of_sensors - 1){
          data_string.concat(' ');
        }
        
      }
      //Serial.println(data_string);
      subState = data_string;
      
      measured_serial_freq = 1000.0/(float(millis() - serial_freq_time)); // calc frequency with current time and time from last loop (serial_freq_time)
      serial_freq_time = millis(); // take current time for next freq measurement    
    }
   digitalWrite(LED, LOW);
  }

  //RUN EVERY TIME - OUTSIDE OF STATEMACHINE
  if (state != oldState or subState != oldSubState){    
    Serial.print(state);
    Serial.print(" ");
    Serial.println(subState);
    oldState = state;
    oldSubState = subState;
  }

  // check for serial messages from max msp
  if (Serial.available()){
    receivedSerial = Serial.read();
    state = receivedSerial;
  }

  // SHOW LED Changes
  strip1.show();
}
