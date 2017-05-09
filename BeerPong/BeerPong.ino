
int Set1 = 3;
int Set2 = 5;
int Set3 = 6;

int Up=0;
int Fade=0;

int updateTime = 10;

int Step= 2;
int lowEnd=25;
int highEnd=250;

unsigned long S1Delay1 = 0;
unsigned long S1Delay2 = 0;

void setup() {
  
  pinMode(Set1, OUTPUT);
  pinMode(Set2, OUTPUT);
  pinMode(Set3, OUTPUT);

  analogWrite(Set1, 0);
  analogWrite(Set2, 0);
  analogWrite(Set3, 0);
}

void loop() {

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
    }

    analogWrite(Set1, Fade);
    
}
