 // Define trig and echo pins
  const int trigPin = A0;
  const int echoPin = A1;
  const int LEDPin = 13;

  // define variables
  long duration;
  int distance;

  //Cat entered sandbox distance
  const int catDistance = 10;
  
void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LEDPin, OUTPUT);  // LED-pin
  Serial.begin(9600);  // Starts the serial communication at 9600 bd
  
}

void loop() {
  // Clear trigpin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Set the trigpin to HIGH for 10 µs, wait for response
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the exhopin, and return the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance - Speed of sound = 0.034 cm/µs, divide by two due to double travel distance
  distance = duration*0.034/2;

  // Print
  Serial.print("Distance: ");
  Serial.println(distance);

  // If cat enters box, turn on LED and 
  if(distance <= catDistance){
    while(true){
      digitalWrite(LEDPin, HIGH);
    }
  }
}
