//VVC HOME AUTOMATION
//11-2-2016
//AUTHOR: ADRIAN TAPIA

const int led = D0;

int send_data(String data);

//SETUP
void setup()
{
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);
    Serial1.begin(9600);
    Spark.function("send_data", send_data);
}

//LOOP
void loop()
{
    //empty
}


//SEND DATA
//desc. send whatever comes from the web to the arduino
int send_data(String data)
{
    Serial1.write(data[0]);
    delay(5);
    Serial1.write(data[1]);
    Serial.print("value: ");
    Serial.write(data[1]+48);
    Serial.println();
    
    //use led to notify photon -> arduino communication
    digitalWrite(led,HIGH);
    delay(100);
    digitalWrite(led,LOW);
}