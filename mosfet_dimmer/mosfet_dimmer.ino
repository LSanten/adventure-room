const int gate = 11; //transistor gate
const int analog = A1; //potentiometer value to control transistor


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(gate, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int analogValue = analogRead(analog);
  int mappedValue = map(analogValue, 0, 1023, 0, 255);
  Serial.println(mappedValue);  
  
  analogWrite(gate, mappedValue);
  digitalWrite(LED_BUILTIN, LOW); 
}
