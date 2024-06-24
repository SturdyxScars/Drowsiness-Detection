#define Relay 13
#define Buzzer 3 // Define the pin for the buzzer

static const int sensorPin = 2;

int SensorStatePrevious = HIGH; // Start with HIGH to indicate no activity
unsigned long minSensorDuration = 3000;

unsigned long SensorLongMillis;
bool SensorStateLongTime = false;

const int intervalSensor = 50;
unsigned long previousSensorMillis;
unsigned long SensorOutDuration;

unsigned long currentMillis;

void setup() {
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
  pinMode(Relay, OUTPUT);
  pinMode(Buzzer, OUTPUT); // Initialize the buzzer pin as an output
  noTone(Buzzer); // Turn off the buzzer initially
}

void readSensorState() {
  if (currentMillis - previousSensorMillis > intervalSensor) {
    int SensorState = digitalRead(sensorPin);
    if (SensorState == HIGH && SensorStatePrevious == LOW && !SensorStateLongTime) {
      SensorLongMillis = currentMillis;
      SensorStatePrevious = HIGH;
      Serial.println("Button pressed");
    }
    SensorOutDuration = currentMillis - SensorLongMillis;
    if (SensorState == HIGH && !SensorStateLongTime && SensorOutDuration >= minSensorDuration) {
      SensorStateLongTime = true;
      digitalWrite(Relay, HIGH);
      Serial.println("Button long pressed");
      // Activate the buzzer
      tone(Buzzer, 1000, 1000); // Activate the buzzer with a 1kHz frequency for 1 second
    }
    if (SensorState == LOW && SensorStatePrevious == HIGH) {
      SensorStatePrevious = LOW;
      SensorStateLongTime = false;
      digitalWrite(Relay, LOW);
      digitalWrite(Buzzer, LOW); // Turn off the buzzer
      Serial.println("Button released");
    }
    previousSensorMillis = currentMillis;
  }
}

void loop() {
  currentMillis = millis();
  readSensorState();
}
