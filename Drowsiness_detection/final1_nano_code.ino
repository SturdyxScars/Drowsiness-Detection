#define Buzzer 3 // Define the pin for the buzzer
#define transmitPin 8 // Digital pin connected to the transmitter module

static const int sensorPin = A1; // A1 corresponds to analog pin 1 on the Arduino Nano

int SensorStatePrevious = HIGH; // Start with HIGH to indicate no activity
unsigned long minSensorDuration = 3000;

unsigned long SensorLongMillis;
bool SensorStateLongTime = false;

const int intervalSensor = 50;
unsigned long previousSensorMillis;
unsigned long SensorOutDuration;

unsigned long currentMillis;

bool isDriverAwake = true; // Initialize to true, assuming the driver is awake initially

void setup() {
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
  pinMode(Buzzer, OUTPUT); // Initialize the buzzer pin as an output
  noTone(Buzzer); // Turn off the buzzer initially
  pinMode(transmitPin, OUTPUT);
  digitalWrite(transmitPin, HIGH); // Set transmitPin to HIGH initially
  Serial.print("Initial transmitPin state: ");
  Serial.println(digitalRead(transmitPin));
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
      Serial.println("Button long pressed");
      // Activate the buzzer
      tone(Buzzer, 1000, 1000); // Activate the buzzer with a 1kHz frequency for 1 second

      // Set transmitPin to LOW when sleep is detected
      digitalWrite(transmitPin, LOW);
      // Set the driver awake flag to false
      isDriverAwake = false;
    }
    if (SensorState == LOW && SensorStatePrevious == HIGH) {
      SensorStatePrevious = LOW;
      SensorStateLongTime = false;
      digitalWrite(Buzzer, LOW); // Turn off the buzzer
      Serial.println("Button released");

      // Set transmitPin to HIGH when the driver is awake
      digitalWrite(transmitPin, HIGH);
      // Reset the driver awake flag to true
      isDriverAwake = true;
    }
    previousSensorMillis = currentMillis;
  }
}

void loop() {
  currentMillis = millis();
  readSensorState();
  
  if (!isDriverAwake) {
    tone(Buzzer, 1000); // Activate the buzzer with a 1kHz frequency
    digitalWrite(transmitPin, LOW);
  } else {
    noTone(Buzzer); // Turn off the buzzer if the driver is awake
    digitalWrite(transmitPin, HIGH);
  }
  Serial.print("Initial transmitPin state: ");
  Serial.println(digitalRead(transmitPin));
}
