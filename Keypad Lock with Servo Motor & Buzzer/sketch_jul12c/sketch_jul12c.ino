/**********************************************************************
  Filename    : Combination Lock (Silent Typing)
  Description : Optimized for original Freenove layout. Buzzer only sounds
                on an incorrect 4-digit combination.
**********************************************************************/
#include <Keypad.h>

#define SERVO_PIN 21  // Define the pwm pin
#define SERVO_CHN 0   // Define the pwm channel
#define SERVO_FRQ 50  // Define the pwm frequency
#define SERVO_BIT 12  // Define the pwm precision

#define BUZZER_PIN 17 // Define the buzzer pin
#define BUZZER_CHN 1  // Separate channel for audio to prevent servo conflicts
#define BUZZER_BIT 8  

void servo_set_pin(int pin);
void servo_set_angle(int angle);
void buzzer_beep(int frequency, int duration);

// Kept EXACTLY matching your breakout board layout
byte rowPins[4] = {14, 13, 12, 11}; 
byte colPins[4] = {10,  9,  8, 18}; 

char keys[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

Keypad myKeypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);
char passWord[4] = {'1', '2', '3', '4'}; 

void setup() {
  Serial.begin(115200); 
  
  servo_set_pin(SERVO_PIN);
  servo_set_angle(0); // Start in locked position
  
  #if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
    // Core 3.0+ configurations handled inline
  #else
    ledcSetup(BUZZER_CHN, 2000, BUZZER_BIT);
    ledcAttachPin(BUZZER_PIN, BUZZER_CHN);
  #endif
}

void loop() {
  static char keyIn[4];     
  static byte keyInNum = 0; 
  
  char keyPressed = myKeypad.getKey();  

  if (keyPressed) {
    // REMOVED: The buzzer_beep from here so typing is completely silent!

    if (keyInNum < 4) {
      keyIn[keyInNum] = keyPressed;
      keyInNum++;
    }

    if (keyInNum == 4) {
      bool isRight = true;
      for (int i = 0; i < 4; i++) {
        if (keyIn[i] != passWord[i]) {
          isRight = false;
        }
      }

      if (isRight) {
        Serial.println("passWord right!");
        servo_set_angle(90);           
        delay(2000);                   
        servo_set_angle(0);            
      }
      else {
        Serial.println("passWord error!");
        // Buzzer ONLY sounds here now if the final 4-digit code is wrong
        buzzer_beep(800, 800); 
      }
      
      // Clear buffer and counter completely for the next attempt
      keyInNum = 0; 
      memset(keyIn, 0, sizeof(keyIn));
    }
  }
}

void buzzer_beep(int frequency, int duration) {
  #if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
    ledcAttach(BUZZER_PIN, frequency, BUZZER_BIT);
    ledcWrite(BUZZER_PIN, 127); 
    delay(duration);
    ledcWrite(BUZZER_PIN, 0);   
    ledcDetach(BUZZER_PIN);
  #else
    ledcWriteTone(BUZZER_CHN, frequency);
    delay(duration);
    ledcWriteTone(BUZZER_CHN, 0);
  #endif
}

void servo_set_pin(int pin) {
  #if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
    ledcAttach(pin, SERVO_FRQ, SERVO_BIT); 
  #else
    ledcAttachChannel(pin, SERVO_FRQ, SERVO_BIT, SERVO_CHN);
  #endif
}

void servo_set_angle(int angle) {
  if (angle > 180 || angle < 0) return;
  long pwm_value = map(angle, 0, 180, 102, 512);
  
  #if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
    ledcWrite(SERVO_PIN, pwm_value);
  #else
    ledcWrite(SERVO_PIN, pwm_value); 
  #endif
}