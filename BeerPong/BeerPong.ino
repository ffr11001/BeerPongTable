#include <SoftwareSerial.h>// import the serial library

SoftwareSerial BlueTooth(10, 11); // RX, TX

// Pins setup

int auxIn = 0;
int micIn = 1;

int strobePin=2;
int resetPin=3;

int B_R = 5;
int R_B = 6;
int G_G = 9;

// Global variables

int Up=0;
int Fade=0;

int Mode =0;
int Input=0;

char ch_arr[3];
int spectrumValue[7];
int updateTime = 10;

int filter=80;
int Step= 2;
int lowEnd=0;
int highEnd=250;

unsigned long S1Delay1 = 0;
unsigned long S1Delay2 = 0;

int inc = 0;

void setup() {
  
  pinMode(B_R, OUTPUT);
  pinMode(R_B, OUTPUT);
  pinMode(G_G, OUTPUT);

  analogWrite(B_R, 0);
  analogWrite(R_B, 0);
  analogWrite(G_G, 0);

  BlueTooth.begin(9600);
  Serial.begin(9600);
  
}

void loop() {

  switch(Input){
      case 0:
        changeFade();
      break;
      case 1:
        AUX();
      break;
      case 2:
        MIC();
      break;
  }

  switch(Mode){
      case 0:
        correctSide();
      break;
      case 1:
        inverseSide();
      break;
      case 2:
        greenOverload();
      break;
      case 3:
        updateTime=200;
        takeTurn(inc);
      break;
      case 4:
        event();
      break;
      case 5:
        updateTime=200;
        madness(inc);
      break;
  }
  
  if(BlueTooth.available()>0){
    String temp = BlueTooth.readString();
    temp.toCharArray(ch_arr, 4);
    /*Serial.println(ch_arr[0]);
    Serial.println(ch_arr[1]);
    Serial.println(ch_arr[2]);
    Serial.println(ch_arr[3]);*/ 
    BlueTooth.flush(); 
    Set(ch_arr);
  }
}

void AUX () {
  
  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);
  for (int i=0;i<7;i++){
    digitalWrite(strobePin, LOW);
    delayMicroseconds(30);
    spectrumValue[i]=analogRead(auxIn);
    spectrumValue[i]=constrain(spectrumValue[i], filter, 1023);
    spectrumValue[i]=map(spectrumValue[i], filter,1023,0,255);
  }
  Fade=spectrumValue[0];
  Serial.println(analogRead(auxIn));
}

void MIC () {
  
  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);
  for (int i=0;i<7;i++){
    digitalWrite(strobePin, LOW);
    delayMicroseconds(30);
    spectrumValue[i]=analogRead(micIn);
    spectrumValue[i]=constrain(spectrumValue[i], filter, 1023);
    spectrumValue[i]=map(spectrumValue[i], filter,1023,0,255);
  }
  Fade=spectrumValue[0];
}

void changeFade (){
  
  if(S1Delay1==0){
    S1Delay1 = millis();
  }

  S1Delay2 = millis();
  if(S1Delay2 - S1Delay1 > updateTime){
    S1Delay1=0;
    S1Delay2=0;
    if(Fade>=highEnd){
      Up=0;
      }
    else if(Fade<=lowEnd){
      Up=1;
      }
    switch(Up){
      case 0:
        Fade=Fade-Step;
      break;
      case 1:
        Fade=Fade+Step;
      break;
      }
      
    inc++;
    inc= Reset(inc);
  }
}

void Set (char ch_arr[]){
  
  switch(ch_arr[0]){
      case '0':
        Mode= ch_arr[1] - '0';
        Serial.println("Mode"); 
      break;
      case '1':
        updateTime = ch_arr[1] - '0' + 1;
        Serial.println("SemiMode"); 
      break;
      case '2':
        Input = ch_arr[1] - '0';
        Serial.println("AdvancedMode"); 
      break;
  }
    Serial.println(Mode); 
    Serial.println(Input); 
    Serial.println(updateTime);  
}

void correctSide (){
  
    analogWrite(B_R, Fade); 
    analogWrite(R_B, 0);  
    analogWrite(G_G, 0); 
    
  } 
  
void inverseSide (){
  
    analogWrite(B_R, 0); 
    analogWrite(R_B, Fade);  
    analogWrite(G_G, 0); 
    
  }  

void greenOverload(){
  
    analogWrite(B_R, 0); 
    analogWrite(R_B, 0);  
    analogWrite(G_G, Fade); 
  
  }
  
void takeTurn (int i){

  if(i % 2 == 0){ 
    analogWrite(G_G, lowEnd); 
    analogWrite(R_B, lowEnd); 
    analogWrite(B_R, highEnd); 
   }else{       
    analogWrite(G_G, lowEnd); 
    analogWrite(B_R, lowEnd);  
    analogWrite(R_B, highEnd);  
   }

}

void event (){
  
    analogWrite(B_R, Fade);
    analogWrite(R_B, Fade); 
    analogWrite(G_G, Fade); 
  
  }

void madness (int i){

  switch(i % 10){
      case 0:
        analogWrite(B_R, 255);
        analogWrite(R_B, 0); 
        analogWrite(G_G, 0); 
      break;
      case 1:
        analogWrite(B_R, 125);
        analogWrite(R_B, 125); 
        analogWrite(G_G, 0); 
      break;
      case 2:
        analogWrite(B_R, 0);
        analogWrite(R_B, 255); 
        analogWrite(G_G, 0); 
      break;
      case 3:
        analogWrite(B_R, 0);
        analogWrite(R_B, 125); 
        analogWrite(G_G, 125); 
      break;
      case 4:
        analogWrite(B_R, 0);
        analogWrite(R_B, 0); 
        analogWrite(G_G, 255); 
      break;
      case 5:
        analogWrite(B_R, 255);
        analogWrite(R_B, 0); 
        analogWrite(G_G, 0); 
      break;
      case 6:
        analogWrite(B_R, 0);
        analogWrite(R_B, 255); 
        analogWrite(G_G, 0); 
      break;
      case 7:
        analogWrite(B_R, 0);
        analogWrite(R_B, 0); 
        analogWrite(G_G, 255); 
      break;
      case 8:
        analogWrite(B_R, 100);
        analogWrite(R_B, 100); 
        analogWrite(G_G, 100); 
      break;
      case 9:
        analogWrite(B_R, 0);
        analogWrite(R_B, 0); 
        analogWrite(G_G, 0); 
      break;
  }
}

int Reset (int i){
  if(i>1000000){
    i=0;
    }
  return i;
  }
