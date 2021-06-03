
int led1 = D5;  
int gnd1 = D6;

int led2 = D7;  
int gnd2 = D8;

int led3 = D1;  
int gnd3 = D2;

int led4 = D3;  
int gnd4 = D4;

void setup() {
  // put your setup code here, to run once:
pinMode(led1, OUTPUT);
pinMode(gnd1, OUTPUT);

pinMode(led2, OUTPUT);
pinMode(gnd2, OUTPUT);

pinMode(led3, OUTPUT);
pinMode(gnd3, OUTPUT);

pinMode(led4, OUTPUT);
pinMode(gnd4, OUTPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(gnd1,LOW);
digitalWrite(gnd2,LOW);
digitalWrite(gnd3,LOW);
digitalWrite(gnd4,LOW);
digitalWrite(led1,HIGH);
digitalWrite(led2,HIGH);
digitalWrite(led3,HIGH);
digitalWrite(led4,HIGH);

//delay(1000);
//digitalWrite(led2,LOW);
//digitalWrite(led1,LOW);
//delay(1000);
//digitalWrite(led1,HIGH);
//digitalWrite(led2,HIGH);
}
