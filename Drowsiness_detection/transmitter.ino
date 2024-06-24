const int transmitPin = 8; // Digital pin connected to the transmitter module

void setup() {
  pinMode(transmitPin, OUTPUT);
}

void loop() {
  // Turn the transmitter on
  digitalWrite(transmitPin, HIGH);
  delay(2000); // Transmit for 2 second

  // Turn the transmitter off
  digitalWrite(transmitPin, LOW);
  delay(5000); // Wait for 1 second before transmitting again
}
