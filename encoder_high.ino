/* Encoder high https://arduino.stackexchange.com/questions/36802/two-outputs-which-one-leads-the-other
 *  
 */ 
 //The following three vars should be visible to loop(), which
// can find out if an encoder changed by testing rotoInt.
static   byte roABold;          // A,B states for rotor
volatile long roCount;      // current rotary count
long tocdo;
volatile byte rotoInt;          // Rotary-interrupt flag (change flag)
unsigned long timer_gio;
// jiw (c) 2015  Offered without warranty under GPL v3 terms as at http://www.gnu.org/licenses/gpl.html

ISR(PCINT1_vect) {  // This plus Arduino overhead of 2.6 us probably takes ca 5 us total per interrupt
  enum { upMask = 0x66, downMask = 0x99 };// up & down are complements
  byte roABnew = PINC & 3;  // Read pins  14, 15, which are A0, A1
  byte criterion = roABnew ^ roABold;
  if (criterion==1 || criterion==2) {
    if (upMask & (1 << (2*roABold + roABnew/2))) // Embeds state transition rules
      ++roCount;
    else --roCount;             // upMask = ~downMask
  }
  roABold = roABnew;            // Save new state
  rotoInt = 1;                  // Say something happened
}

// For rotary encoder initialization, in setup(), say:   setup_rotocoder();
void setup_rotocoder() {
  rotoInt = roABold = 0;
  roCount = 0;              // Start with a middle count
  pinMode(A0, INPUT_PULLUP);    // pin 14, A0, PC0, for pin-change interrupt
  pinMode(A1, INPUT_PULLUP);    // pin 15, A1, PC1, for pin-change interrupt
  PCMSK1 |= 0x03;
  PCIFR  |= 0x02;             // clear pin-change interrupts if any
  PCICR  |= 0x02;             // enable pin-change interrupts
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
setup_rotocoder();
timer_gio = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
    if ( (unsigned long) (millis() - timer_gio) > 10000 ) {
        timer_gio = millis();
        long tam =0;
        //if (tocdo < roCount){tam = roCount - tocdo;}
        //else tam= tocdo - roCount;
        tam = roCount - tocdo;
        Serial.println(tam);
        tocdo=roCount;
        Serial.println(tam);
        tam=tam/100;
        Serial.println(tam);
        tam=tam*6;
        Serial.print(roCount);
        Serial.print(" V/P: ");
        Serial.println(tam);
    }
}
