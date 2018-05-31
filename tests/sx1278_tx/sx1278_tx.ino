void setup() {
  // put your setup code here, to run once:
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  
  Serial.begin(115200);
  Serial3.begin(9600);
  digitalWrite(6,1);
  digitalWrite(7,1);
  delay(500);
  Serial3.write(0xc0);
  Serial3.write(0x00);
  Serial3.write(0x00);
  Serial3.write(0x25);
  Serial3.write(0x00);
  Serial3.write(0x40);
  delay(500);
  Serial3.end();
  digitalWrite(6,0);
  digitalWrite(7,0);
  delay(10);
  Serial3.begin(19200);
  delay(100);
  
  Serial.println("Begin\n");
}

void loop() {
  // put your main code here, to run repeatedly:
  String a = String(millis());
  Serial3.println(a);
  Serial.println(a);
  //delay(15);
  //delay(100);
}
