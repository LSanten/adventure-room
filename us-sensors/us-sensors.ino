// @author Leon Santen
// @project Interactive Art
// To open port on Linux: sudo chmod a+rw /dev/ttyACM0 

#include "Arduino.h"

const int TRIGGER = 8;
const int LED = 13;
uint32_t serial_freq_time = 0;
float measured_serial_freq;
uint32_t US_frequency = 10; //frequency: clicks per second
uint32_t current_millis; 
uint32_t task_millis = 0;
uint32_t task_length = 1000/US_frequency;
int receivedSerial; // received serial input from max msp
const int gate = 11; // gate value for transistor for LED

int state = 0;
String subState;
int oldState = 0;
String oldSubState;

int number_of_sensors = 1; //the amount of ultrasonic sensors used
int sensor_pins[] = {A3}; // input pins for sensor data - pin number refers to sensor from left to right in direction of traffic

void setup()
{
  pinMode(TRIGGER, OUTPUT);
  digitalWrite(TRIGGER, HIGH);
  pinMode(LED, OUTPUT);
  pinMode(gate, OUTPUT);
  Serial.begin(9600);
}
void loop()
{
  state = 1;
  
  if (state==1){
    current_millis = millis();
  
    // send trigger and run code in if statement with frequency --> US_frequency
    if (current_millis - task_millis >= task_length){
      digitalWrite(LED, HIGH);
      task_millis = millis();
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
    analogWrite(gate, receivedSerial);
  }
}
