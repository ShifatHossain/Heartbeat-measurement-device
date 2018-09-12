#include <LiquidCrystal.h>
  #include <Boards.h>
#include <avr/interrupt.h>

LiquidCrystal lcd (8,10,4,5,6,7);
uint8_t a = 1;
uint16_t val = 0;
int cnt = 0;
int data = 0;
float dur = 0;
int bpm = 0;

void setup(){
  lcd.begin(16,2);
  initTimer();
  analogReference(INTERNAL2V56);
  Serial.begin(9600);
  
}
void initTimer()
{
  TIMSK1 |= (1 << TOIE1);
  TCCR1A = 0b00000000;
}

void startTimer()
{
  TCCR1B |= 0b00000101;
  TCCR1B &= ~(0b00000010);
}

void stopTimer()
{
  TCCR1B &= ~(0b00000101);
}

void resetTimer()
{
  TCNT1H = 0b00000100;
  TCNT1L = 0b10010100;
  //a =0;
}

uint16_t timerVal()
{
  return TCNT1H*256+TCNT1L;
}

void loop()
{
  data = analogRead(8);
 //Serial.println(data);
  if(data > 30 && data < 50)
  {
    if(cnt == 0)
    {
      startTimer();
    }
    cnt++;
  }
  if(cnt == 10)
  {
    stopTimer();
    val = timerVal();
   float dur = (val*0.0064+a*4.194)/cnt;
    bpm = 60/dur;
    //Serial.println(a);
    //Serial.println(val);
   //Serial.println(dur);
  Serial.println("Heartbeat =   " + String(bpm));
  lcd.setCursor(0,1);
  lcd.print(bpm);
    resetTimer();
    cnt=0;
    a=0;
    
  }  
}

