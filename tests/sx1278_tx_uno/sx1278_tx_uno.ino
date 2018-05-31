void setup() {
  // put your setup code here, to run once:
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  
  Serial.begin(9600);
  digitalWrite(6,1);
  digitalWrite(7,1);
  delay(500);
  Serial.write(0xc0);
  Serial.write(0x00);
  Serial.write(0x00);
  Serial.write(0x25);
  Serial.write(0x00);
  Serial.write(0x40);
  delay(500);
  Serial.end();
  digitalWrite(6,0);
  digitalWrite(7,0);
  delay(10);
  Serial.begin(19200);
  delay(100);
}

uint8_t count=0;
void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(count++);
  delay(15);
}
