//VVC HOME AUTOMATION PROJECT
//OCTOBER 27, 2016
//AUTHOR: ADRIAN TAPIA

//OUTPUTS
#define M_LIGHT1 2
#define M_LIGHT2 3
#define G_DOOR 4
#define FAN1 5
#define FAN2 6
#define FAN3 7
#define FAN4 8
#define LIGHT1 9
#define LIGHT2 10
#define LIGHT3 11
#define LIGHT4 12
#define LIGHT5 13
#define LIGHT6 22
#define GDOOR1 24
#define GDOOR2 25
#define BUZZER 19

//INPUTS
#define M_SENSOR1 A0
#define M_SENSOR2 A1
#define G_SENSOR A2
#define DOOR_IR 18

//CONSTANTS / MISC
const int DATA_TIMEOUT = 5000;
typedef unsigned short u_short;
typedef unsigned long u_long;
const u_short SHIFT_BITS = 4;
const byte PIN_MASK = 0xF;
const u_short LIGHT6_CHECK = 12;
const u_short M_LIGHT1_CHECK = 15;
const u_short ANALOG_OFFSET  = 6; 
enum io_type
{
  DIGITAL,
  ANALOG,
  SOFT_DIGITAL,
  GOTO_ANALOG,
  MASTER,
  MASTER_M_LIGHT,
  MASTER_FAN,
  MASTER_LIGHT,
  DIGITAL_READ,
  ANALOG_READ
} type;
bool gdoor_open;

//FUNCTIONS
void gpio_setup();
void read_data(const u_short id, const u_short value);
bool data_timeout();
void extract_data(const u_short data, u_short& pin, io_type& type);
void write_analog_value(const u_short pin);
void write_digital_value(const u_short pin);
void door_ir();
void master_off(const u_short value);

//SETUP
//desc. executes once when arduino boots
void setup()
{
  Serial.begin(9600);   
  Serial3.begin(9600);
  gpio_setup();
  master_off(0);

  breath_demo();
}

//LOOP
void loop()
{ 
  //flash_demo();
  //loop_demo();
}

//READ DATA
//desc. read serial data and determine what to do
void read_data(const u_short data, const u_short value)
{
  u_short pin;
  extract_data(data, pin, type);

  Serial.print("Value: ");
  Serial.println(value);
  Serial.print("Pin: ");
  Serial.println(pin);
  Serial.print("type: ");
  Serial.println(type);
  Serial.println();
  Serial.println();

  switch(type)
  {
    case DIGITAL:
      if(pin != 16) //temporary alarm control
        digitalWrite(pin, value); 
      else
        write_digital_value(value);
      break;
    case ANALOG:
      if(pin == 4) //temporary garage door control
      {
        Serial.write("gdoor");
        if(gdoor_open)
        {
          digitalWrite(GDOOR1, LOW);
          digitalWrite(GDOOR2, HIGH);
          delay(d);
          digitalWrite(GDOOR2, LOW);
          digitalWrite(GDOOR1, LOW);
          gdoor_open = false;
        }else
        {
          digitalWrite(GDOOR1, HIGH);
          digitalWrite(GDOOR2, LOW);
          delay(d);
          digitalWrite(GDOOR1, LOW);
          digitalWrite(GDOOR2, LOW);
          gdoor_open = true;
        }
      }
      if(pin != 16)
      analogWrite(pin, value);
      else
      write_analog_value(value);
      break;
    case SOFT_DIGITAL:
      break;
    case GOTO_ANALOG:
      break;
    case MASTER:
      break;
    case MASTER_M_LIGHT:
      break;
    case MASTER_FAN:
      break;
    case MASTER_LIGHT:
      break;
    case DIGITAL_READ:
      break;
    case 12:
      write_analog_value(pin);
      break;
  }
}

//SERIAL EVENT
//desc. executes when serial data is available and sends data to read_data function
void serialEvent3()
{
    u_short id = Serial3.read();
    bool timeout = data_timeout(); //wait for second value and check for timeout 
    u_short value = Serial3.read();
    
    if(!timeout)
      read_data(id, value);
    else    
      Serial.print("TIMEOUT -> ");
}

//EXTRACT DATA
//desc. 
void extract_data(const u_short data, u_short& pin, io_type& type)
{
  type = data >> SHIFT_BITS;
  pin = data & PIN_MASK;
  pin != LIGHT6_CHECK ? pin+=2 : pin = LIGHT6; //all pins equal pin bits besides light 6 and m_light 1
  pin == M_LIGHT1_CHECK ? pin = M_LIGHT1 : pin;
}

//DATA TIMEOUT
//desc. return false if serial data isn't avaible before timeout time(DATA_TIMEOUT)
bool data_timeout()
{
  bool timeout = false;
  u_long time = millis();
  while(!Serial3.available() && !timeout)
    millis() - time > DATA_TIMEOUT ? timeout = true : timeout;
  return timeout;
}

//WRITE DIGITAL VALUE
//Desc. read digital pin and write it to the photon
void write_digital_value(const u_short value)
{
  if(value == 0)
    master_off(0);
}

//WRITE ANALOG VALUE
//Desc. read analog pin and write value to the photon
void write_analog_value(const u_short pin)
{
    Serial3.write((u_short)(analogRead(pin) / 4));
}

//DOOR SENSOR
//desc. door ir sensor has been broken
void door_ir()
{
  //TODO, use interupts
}

//MASTER OFF
//desc. turn everytging off
void master_off(const u_short value)
{
  for(int i=0; i<255; i++)
  {
    digitalWrite(M_LIGHT1, value);
    digitalWrite(M_LIGHT2, value);
    digitalWrite(G_DOOR, value);
    digitalWrite(FAN1, value);
    digitalWrite(FAN2, value);
    digitalWrite(FAN3, value);
    digitalWrite(FAN4, value);
    digitalWrite(LIGHT1, value);
    digitalWrite(LIGHT2, value);
    digitalWrite(LIGHT3, value);
    digitalWrite(LIGHT4, value);
    digitalWrite(LIGHT5, value);
  }
}

//GPIO SETUP
//desc. setup inputs and outputs
void gpio_setup()
{
  pinMode(M_LIGHT1, OUTPUT);
  pinMode(M_LIGHT2, OUTPUT);
  pinMode(G_DOOR, OUTPUT);
  pinMode(FAN1, OUTPUT);
  pinMode(FAN2, OUTPUT);
  pinMode(FAN3, OUTPUT);
  pinMode(FAN4, OUTPUT);
  pinMode(LIGHT1, OUTPUT);
  pinMode(LIGHT2, OUTPUT);
  pinMode(LIGHT3, OUTPUT);
  pinMode(LIGHT4 ,OUTPUT);
  pinMode(LIGHT5 ,OUTPUT);
  pinMode(LIGHT6 ,OUTPUT);
  pinMode(M_SENSOR1, INPUT);
  pinMode(M_SENSOR2, INPUT);
  pinMode(G_SENSOR, INPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);
  pinMode(A8, INPUT);
  pinMode(A9, INPUT);
  pinMode(A10, INPUT);
  pinMode(A11, INPUT);
  pinMode(A12, INPUT);
  pinMode(A13, INPUT);
  pinMode(A14, INPUT);
  pinMode(A15, INPUT);
  pinMode(DOOR_IR, INPUT);
  pinMode(BUZZER, OUTPUT);
  attachInterrupt(analogPinToInterrupt(DOOR_IR), door_ir, HIGH);
}

//ANIMATIONS BELOW #####################################################################################################################
//note. unable to control via the photon during animations

void breath_demo()
{
  for(int value=0; value<180; value++)
  {
    analogWrite(M_LIGHT1, value);
    analogWrite(M_LIGHT2, value);
    analogWrite(G_DOOR, value);
    analogWrite(FAN1, value);
    analogWrite(FAN2, value);
    analogWrite(FAN3, value);
    analogWrite(FAN4, value);
    analogWrite(LIGHT1, value);
    analogWrite(LIGHT2, value);
    analogWrite(LIGHT3, value);
    analogWrite(LIGHT4, value);
    analogWrite(LIGHT5, value);
    delay(3);
  }  

  for(int value=255; value>0; value--)
  {
    analogWrite(M_LIGHT1, value);
    analogWrite(M_LIGHT2, value);
    analogWrite(G_DOOR, value);
    analogWrite(FAN1, value);
    analogWrite(FAN2, value);
    analogWrite(FAN3, value);
    analogWrite(FAN4, value);
    analogWrite(LIGHT1, value);
    analogWrite(LIGHT2, value);
    analogWrite(LIGHT3, value);
    analogWrite(LIGHT4, value);
    analogWrite(LIGHT5, value);
    delay(3);
  }
}

void flash_demo()
{
  for(int i=0; i<5; i++)
  {
    digitalWrite(BUZZER, HIGH);
    delay(100);
    for(int value=0; value<180; value++)
  {
    analogWrite(M_LIGHT1, value);
    analogWrite(M_LIGHT2, value);
    analogWrite(G_DOOR, value);
    analogWrite(FAN1, value);
    analogWrite(FAN2, value);
    analogWrite(FAN3, value);
    analogWrite(FAN4, value);
    analogWrite(LIGHT1, value);
    analogWrite(LIGHT2, value);
    analogWrite(LIGHT3, value);
    analogWrite(LIGHT4, value);
    analogWrite(LIGHT5, value);
  }  

  for(int value=255; value>0; value--)
  {
    analogWrite(M_LIGHT1, value);
    analogWrite(M_LIGHT2, value);
    analogWrite(G_DOOR, value);
    analogWrite(FAN1, value);
    analogWrite(FAN2, value);
    analogWrite(FAN3, value);
    analogWrite(FAN4, value);
    analogWrite(LIGHT1, value);
    analogWrite(LIGHT2, value);
    analogWrite(LIGHT3, value);
    analogWrite(LIGHT4, value);
    analogWrite(LIGHT5, value);
  }
      digitalWrite(BUZZER, LOW);
  } 
}

void loop_demo()
{
  int d = 150;

  for(int i=0; i<50; i++)
  {   
    analogWrite(M_LIGHT2, 255);
    analogWrite(G_DOOR, 150);
    analogWrite(M_LIGHT1, 70);
    analogWrite(LIGHT1, 0);
    analogWrite(LIGHT2, 0);
    analogWrite(LIGHT3, 0);
    analogWrite(LIGHT4, 0);
    analogWrite(LIGHT3, 0);
    analogWrite(LIGHT5, 0);
    delay(d);
    analogWrite(M_LIGHT2, 0);
    analogWrite(G_DOOR, 255);
    analogWrite(M_LIGHT1, 150);
    analogWrite(LIGHT1, 70);
    analogWrite(LIGHT2, 0);
    analogWrite(LIGHT3, 0);
    analogWrite(LIGHT4, 0);
    analogWrite(LIGHT3, 0);
    analogWrite(LIGHT5, 0);
    delay(d);
    analogWrite(M_LIGHT2, 0);
    analogWrite(G_DOOR, 0);
    analogWrite(M_LIGHT1, 255);
    analogWrite(LIGHT1, 150);
    analogWrite(LIGHT2, 70);
    analogWrite(LIGHT3, 0);
    analogWrite(LIGHT4, 0);
    analogWrite(LIGHT3, 0);
    analogWrite(LIGHT5, 0);
    delay(d);
    analogWrite(M_LIGHT2, 0);
    analogWrite(G_DOOR, 0);
    analogWrite(M_LIGHT1, 0);
    analogWrite(LIGHT1, 255);
    analogWrite(LIGHT2, 150);
    analogWrite(LIGHT3, 70);
    analogWrite(LIGHT4, 0);
    analogWrite(LIGHT3, 0);
    analogWrite(LIGHT5, 0);
    delay(d);
    analogWrite(M_LIGHT2, 0);
    analogWrite(G_DOOR, 0);
    analogWrite(M_LIGHT1, 0);
    analogWrite(LIGHT1, 0);
    analogWrite(LIGHT2, 255);
    analogWrite(LIGHT3, 150);
    analogWrite(LIGHT4, 70);
    analogWrite(LIGHT3, 0);
    analogWrite(LIGHT5, 0);
    delay(d);
    analogWrite(M_LIGHT2, 0);
    analogWrite(G_DOOR, 0);
    analogWrite(M_LIGHT1, 0);
    analogWrite(LIGHT1, 0);
    analogWrite(LIGHT2, 0);
    analogWrite(LIGHT3, 255);
    analogWrite(LIGHT4, 150);
    analogWrite(LIGHT3, 70);
    analogWrite(LIGHT5, 0);
    delay(d);
    analogWrite(M_LIGHT2, 0);
    analogWrite(G_DOOR, 0);
    analogWrite(M_LIGHT1, 0);
    analogWrite(LIGHT1, 0);
    analogWrite(LIGHT2, 0);
    analogWrite(LIGHT3, 0);
    analogWrite(LIGHT4, 255);
    analogWrite(LIGHT3, 150);
    analogWrite(LIGHT5, 70);
    delay(d);
    analogWrite(M_LIGHT2, 70);
    analogWrite(G_DOOR, 0);
    analogWrite(M_LIGHT1, 0);
    analogWrite(LIGHT1, 0);
    analogWrite(LIGHT2, 0);
    analogWrite(LIGHT3, 0);
    analogWrite(LIGHT4, 0);
    analogWrite(LIGHT3, 255);
    analogWrite(LIGHT5, 150);
    delay(d);
    analogWrite(M_LIGHT2, 150);
    analogWrite(G_DOOR, 70);
    analogWrite(M_LIGHT1, 0);
    analogWrite(LIGHT1, 0);
    analogWrite(LIGHT2, 0);
    analogWrite(LIGHT3, 0);
    analogWrite(LIGHT4, 0);
    analogWrite(LIGHT3, 0);
    analogWrite(LIGHT5, 255);
    delay(d);
    breath_demo();
  }
    
}
