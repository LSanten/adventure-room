// @author Leon Santen
// @project Interactive Art
// To open port on Linux: sudo chmod a+rw /dev/ttyACM0 

const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 9; // Analog output pin that the LED is attached to
const int LDRPin = A1;

const int buttonPin1 = 2;
const int buttonPin2 = 3;
const int buttonPin3 = 4;
const int usTriggerPin = 5;
const int usEchoPin = 6;

int state = 0;
int subState = 0;
int oldState = 0;
int oldSubState = 0;

int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;

int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // mapped value output

long usDuration;
int usDistance;


void setup() {
  // initialize serial communication at 9600 bits per second:
  pinMode(usTriggerPin, OUTPUT);
  pinMode(usEchoPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // // CHECK ALL SENSORS
  // check buttons
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
  buttonState3 = digitalRead(buttonPin3);
  
  // // DO SOMETHING WITH SENSOR VALUES

  if (state == 1){
    subState =0; //clear substate value
    // Read potentiometer
    sensorValue = analogRead(analogInPin);
    outputValue = map(sensorValue, 0, 1023, 0, 1000);
    subState = outputValue;
  }
  if (state == 2){
    subState =0; //clear substate value
    // //Read Ultrasonic Sensor
    // Clears the usTriggerPin
    digitalWrite(usTriggerPin, LOW);
    delayMicroseconds(2);
    // Sets the usTriggerPin on HIGH state for 10 micro seconds
    digitalWrite(usTriggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(usTriggerPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    usDuration = pulseIn(usEchoPin, HIGH);
    // Calculating the distance
    usDistance= usDuration*0.034/2;
    subState = 6 * usDistance;
  }
  if (state == 3){
    subState =0; //clear substate value
    sensorValue = analogRead(LDRPin);
    outputValue = map(sensorValue, 0, 1023, 0, 1000);
    subState = outputValue;
  }
  // map potentiometer value
  

  if (buttonState1 == HIGH) {
    state = 1;
    }
  if (buttonState2 == HIGH) {
    state = 2;
    }
  if (buttonState3 == HIGH) {
    state = 3;
    }
  
  // If state and subState differ from old values, print
  if (state != oldState or subState != oldSubState){
    
    Serial.print(state);
    Serial.print(" ");
    Serial.println(subState);
    oldState = state;
    oldSubState = subState;
  } 
}
