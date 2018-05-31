typedef struct command_frame__ 
{
  uint8_t type:3;
  uint8_t crc:5;
  uint8_t data;
} command_frame;


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


void send_command(command_frame *cf)
{
  uint8_t *ptr = (uint8_t *)cf;
  uint8_t crc = 0;
  crc = (cf->type ^ cf->data) & 0x1f;
  cf->crc = crc;
  Serial.write(0xff);
  for (uint8_t i = 0; i < sizeof(command_frame); i ++) {
    Serial.write(ptr[i]);
  }
}

bool parse_command(command_frame *cf, uint8_t *type, uint8_t *data)
{
  uint8_t crc = cf->type ^ cf->data;
  if (crc == cf->crc) {
    *type = cf->type;
    *data = cf->data;
    return true;
  }
  else {
    return false;
  }
}

#define ROLL_DATA  0
#define YAW_DATA   1
#define SPED_DATA  2
#define PITCH_DATA 3

uint8_t count = 0;
uint8_t type = 0;

void loop() {
  command_frame cf;
  cf.type = type;
  cf.data = count++;
  send_command(&cf);
  delay(15);

  if (count == 181) {
    count = 0;
    type = (type + 1) % 4;
  }
}
