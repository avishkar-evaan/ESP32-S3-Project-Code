#define PIN_LED    2
#define PIN_BUTTON 13

void setup() {
  // Initialize LED as an output
  pinMode(PIN_LED, OUTPUT);
  
  // Use INPUT_PULLDOWN to keep the pin at 0V (LOW) when button is released.
  // This prevents the "always ON" floating pin issue you had earlier.
  pinMode(PIN_BUTTON, INPUT_PULLDOWN);
}

void loop() {
  // Since your button is connected to voltage (3.3V), 
  // it will read HIGH when you press it.
  if (digitalRead(PIN_BUTTON) == HIGH) {
    digitalWrite(PIN_LED, HIGH); // Turn LED on
  } 
  else {
    digitalWrite(PIN_LED, LOW);  // Turn LED off
  }
}void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
