// --- Fixed Pin Definitions for Your Hardware Layout ---
const int BLUE_PIN  = 9;   
const int RED_PIN   = 10;  
const int GREEN_PIN = 11;  

const int MIC_DIGITAL_PIN = 3; 

unsigned long lastSoundTime = 0;
int clapCount = 0;

const int debounceDelay = 260; 
const int soundTimeout = 500;  // Faster response time for up to 4 claps

void setup() {
  Serial.begin(115200);
  delay(1000); 
  
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(MIC_DIGITAL_PIN, INPUT);
  
  // Power-On Test Sequence
  Serial.println("\n--- RUNNING 4-COLOR MODE INTERFACE ---");
  setColor(255, 0, 0); delay(200);
  setColor(0, 255, 0); delay(200);
  setColor(0, 0, 255); delay(200);
  SystemReset();
  
  Serial.println("System Ready! Clap away (1 to 4 times max).");
}

void loop() {
  int soundState = digitalRead(MIC_DIGITAL_PIN);
  
  if (soundState == HIGH && (millis() - lastSoundTime > debounceDelay)) { 
    clapCount++;
    lastSoundTime = millis();
    Serial.print("Pulse registered! Count: ");
    Serial.println(clapCount);
  }
  
  // Once you stop clapping for half a second, change the color
  if (clapCount > 0 && (millis() - lastSoundTime > soundTimeout)) {
    executeColorCommand(clapCount);
    clapCount = 0; 
  }
}

void executeColorCommand(int count) {
  Serial.print(">> Command Executed: ");
  switch(count) {
    case 1:
      Serial.println("1 Clap -> Purple/Leela");
      setColor(255, 0, 0);
      break;
    case 2:
      Serial.println("2 Claps -> GREEN/Grüne");
      setColor(0, 255, 0);
      break;
    case 3:
      Serial.println("3 Claps -> BLUE/Blau");
      setColor(0, 0, 255);
      break;
    case 4:
      Serial.println("4 Claps -> RED/Rot");
      setColor(255, 255, 0);
      break;
    default:
      Serial.print(count);
      Serial.println(" claps logged. Out of range (Max 4). Turning off.");
      SystemReset();
      break;
  }
}

void setColor(int red, int green, int blue) {
  analogWrite(RED_PIN,   255 - red);
  analogWrite(GREEN_PIN, 255 - green);
  analogWrite(BLUE_PIN,  255 - blue);
}

void SystemReset() {
  setColor(0, 0, 0);
}