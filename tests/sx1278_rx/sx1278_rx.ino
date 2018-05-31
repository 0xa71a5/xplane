typedef struct command_frame__ {
  uint8_t type:3;
  uint8_t crc:5;
  uint8_t data;
} command_frame;


void send_command(command_frame *cf)
{
  uint8_t *ptr = (uint8_t *)cf;
  uint8_t crc = 0;
  crc = cf->type ^ cf->data;
  cf->crc = crc;
  for (uint8_t i = 0; i < sizeof(command_frame); i ++) {
    Serial.write(ptr[i]);
  }
}

bool parse_command(command_frame *cf, uint8_t *type, uint8_t *data)
{
  uint8_t crc = (cf->type ^ cf->data) & 0x1f;
  if (crc == cf->crc) {
    *type = cf->type;
    *data = cf->data;
    return true;
  }
  else {
    return false;
  }
}

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

uint8_t rx_buffer[6];
uint8_t *rx_ptr = rx_buffer;

#define RECV_BEGIN 0xf0
#define RECV_PERIOD 0xf1
#define RECV_END 0xf2
#define RECV_IDLE 0xf3


uint8_t rx_state = RECV_IDLE;
bool new_data_flag = false;
command_frame cf;

void loop() {

  if (Serial3.available()) {
    uint8_t tmp_data = Serial3.read();
    if (tmp_data == 0xff) {
      rx_ptr = rx_buffer;
      if (rx_state == RECV_PERIOD) {
        cf = *(command_frame *)rx_buffer;
        new_data_flag = true;
      }

      rx_state = RECV_BEGIN;  
    }
    else if (rx_state == RECV_BEGIN) {
      *rx_ptr = tmp_data;
      rx_state = RECV_PERIOD;
      rx_ptr ++;
    }
    else if (rx_state == RECV_PERIOD) {
      *rx_ptr = tmp_data;
      rx_ptr ++;
    }


    if (rx_ptr == rx_buffer + 5) {
      Serial.println("Rx ptr overflow\n");
      rx_ptr = rx_buffer;
    }
  }

  if (new_data_flag) {
    uint8_t type = 0xff;
    uint8_t data = 0;

    new_data_flag = false;

    if (parse_command(&cf, &type, &data)) {
      Serial.print("[+]type=");
      Serial.print(type);
      Serial.print(" data=");
      Serial.println(data);
    }
    else {
      Serial.println("[-] Crc err");
    }
  }

}
