#define S0 2
#define S1 3
#define S2 4
#define S3 5
#define sensorOut 6

void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);

  // Setting frequency-scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  Serial.begin(9600);  // Initialize serial for monitoring
  Serial.println("Color Difference Monitoring");
}

void loop() {
  int R = 0;
  int G = 0;
  int B = 0;

  // Read RED
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  R = pulseIn(sensorOut, LOW);

  // Read GREEN
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  G = pulseIn(sensorOut, LOW);

  // Read BLUE
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  B = pulseIn(sensorOut, LOW);

  // Print the RGB values to the Serial Monitor
  Serial.print("R: ");
  Serial.print(R);
  Serial.print(" G: ");
  Serial.print(G);
  Serial.print(" B: ");
  Serial.println(B);

  // Calculate and print the differences between colors
  int DifRG = abs(R - G);
  int DifGB = abs(G - B);
  int DifRB = abs(R - B);

  Serial.print("Difference R-G: ");
  Serial.println(DifRG);
  
  Serial.println("--------------------");
  delay(1000);  // Delay to make the serial monitor readable
}
