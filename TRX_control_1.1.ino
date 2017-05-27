/*

ArduinoMega 2560 TRX controler By H.Rahimi EP4HR
Version 1.0 11 Feb 2017
*/

// Include the library code
#include <LiquidCrystal.h>
#include <rotary.h>
#include <EEPROM.h>

//Setup some items
#define W_CLK 8   // Pin 8 - connect to AD9850 module word load clock pin (CLK)
#define FQ_UD 9   // Pin 9 - connect to freq update pin (FQ)
#define DATA 10   // Pin 10 - connect to serial data load pin (DATA)
#define RESET 11  // Pin 11 - connect to reset pin (RST) 
#define pulseHigh(pin) {digitalWrite(pin, HIGH); digitalWrite(pin, LOW); }
Rotary r = Rotary(2,3); // sets the pins the rotary encoder uses.  Must be interrupt pins.
LiquidCrystal lcd(12, 13, 7, 6, 5, 4); // LCD pin for (RS,E,D4,D5,D6,D7)
int_fast32_t rx=14000000; // Base (starting) frequency of VFO. 
int_fast32_t rx2=1; // variable to hold the updated frequency
int_fast32_t increment = 100; // starting VFO update increment in HZ.
int_fast32_t iffreq = 8000000; // Intermedite Frequency + Amount to subtract (+) from base frequency.
int buttonstate = 0;
int buttonstate2 = 0;
int GoIF = 1;
String hertz = " 100Hz";
int  hertzPosition = 10;
byte ones,tens,hundreds,thousands,tenthousands,hundredthousands,millions ;  
String freq; // string to hold the frequency
int_fast32_t timepassed = millis(); // int to hold the arduino miilis since startup
int memstatus = 1;  // value to notify if memory is current or old. 0=old, 1=current.
// add PIN for Band select

int M3 = 25 ;//Pin for select 3.5 Mhz RF Filter
int M7 = 26;//Pin for select 7 Mhz RF Filter
int M10 = 27 ;//Pin for select 10 Mhz RF Filter
int M14 = 28 ;//Pin for select 14 Mhz RF Filter
int M18 = 29 ;//Pin for select 18 Mhz RF Filter
int M21 = 30 ;//Pin for select 21 Mhz RF Filter
int M24 = 31 ;//Pin for select 24 Mhz RF Filter
int M28 = 32 ;//Pin for select 28 Mhz RF Filter
int i=1;// For Select vfoA or vfoB
int j=2;//for selecet Mode USB LSB CW
int k=0;//for select Split
int L=1;// For select Antenna A or B 
long f28 =28020000 ;//intial value of 28 MHz
long f24 =24900000 ;//intial value of 24 MHz
long f21 =21020000 ;//intial value of 21 MHz
long f18 =18080000 ;//intial value of 18 MHz
long f14 =14020000 ;//intial value of 14 MHz
long f10 =10120000 ;//intial value of 10 MHz
long f7 =7020000 ;//intial value of 7 MHz
long f3 =3502000 ;//intial value of 3.5 MHz
long vfoA;//Frequency of vfo A
long vfoB ;// Frequency of vfo B
int band ;
int Mod=3;//chose CW mode for forst time
int ModA=3;
int ModB=3;
int Ant=1;
int AntA=1;
int AntB=1;
int ATT=1;
int ATTA=1;
int ATTB=1;
int ATTport=42; // Pin for hardware select Att on or off
int split=0;
int USB=35;//Pin for hardware select for USB mode
int LSB=36;//Pin for hardware select for LSB mode
int CW =37;//Pin for hardware select for CW mode
int Antport=40;//Pin for hardware select antenna 1 or  2

int Agc=1;
int AgcA=1;
int AgcB=1;
int Agcport=41; //Pin for hardware select AGC mode Slow or Fast
int S;// for RX strange
int Pwr; // for Tx power
int Level; // for s meter and Power meter
float Vlote;// input DC voltage by resistor divider divie by 3
 


// end PIN for Band select

int ForceFreq = 0;//in above line if put 1 in first time use intial freq and if put 0 use last freq .


void setup() {
  pinMode(A0,INPUT); // Connect to a button that goes to GND on push
  pinMode(A5,INPUT); // IF sense **********************************************
  digitalWrite(A0,HIGH);
  digitalWrite(A5,HIGH);
   // defind pin mode for Band select ,Ant ,Agc ,ATT

pinMode(M3 ,OUTPUT) ;
pinMode(M7 ,OUTPUT) ;
pinMode(M10 ,OUTPUT) ;
pinMode(M14 ,OUTPUT) ;
pinMode(M18 ,OUTPUT) ;
pinMode(M21 ,OUTPUT) ;
pinMode(M24 ,OUTPUT) ;
pinMode(M28 ,OUTPUT) ;
pinMode(USB ,OUTPUT) ;
pinMode(LSB ,OUTPUT) ;
pinMode(CW ,OUTPUT) ;
pinMode(Antport ,OUTPUT) ;
digitalWrite(Antport,LOW);//intial antenna is Antenna A
pinMode(ATTport,OUTPUT);
digitalWrite(ATTport,LOW);//intial ATT is off
pinMode(Agcport,OUTPUT);
digitalWrite(Agcport,LOW);//intialAgc is slow
  digitalWrite(USB,LOW);
  digitalWrite(LSB,LOW);
  digitalWrite(CW,HIGH); //intial modulation condition is CW 



  // end defind band select
  // intial value for Band select
  lcd.begin(20, 4);

 Serial.begin(9600);
lcd.setCursor(0,0); // 
pinMode(A0,INPUT); // Connect to a button that goes to GND on push
digitalWrite(A0,HIGH);

attachInterrupt(0,MyIsr,CHANGE); // This added for Mega 2560
attachInterrupt(1,MyIsr,CHANGE);// This Added for Mega 2560
 

  pinMode(FQ_UD, OUTPUT);
  pinMode(W_CLK, OUTPUT);
  pinMode(DATA, OUTPUT);
  pinMode(RESET, OUTPUT); 
  pulseHigh(RESET);
  pulseHigh(W_CLK);
  pulseHigh(FQ_UD);  // this pulse enables serial mode on the AD9850 - Datasheet page 12.

 
  // Load the stored frequency  
  if (ForceFreq == 0) {
    freq = String(EEPROM.read(0))+String(EEPROM.read(1))+String(EEPROM.read(2))+String(EEPROM.read(3))+String(EEPROM.read(4))+String(EEPROM.read(5))+String(EEPROM.read(6));
    rx = freq.toInt(); 
    vfoA=rx;
    vfoB=rx;
   lcd.setCursor(0,0);
 lcd.print(" TRX control Ver 1.0 "); 
  lcd.setCursor(0,1);
 lcd.print("      By EP4HR      "); 
  lcd.setCursor(0,2);
 lcd.print("     11 Feb 2017   "); 
 delay (3000);
 lcd.clear();
 lcd.setCursor(0,0);
  lcd.print("vfoA");
   lcd.setCursor(14,1); 
   lcd.print(hertz); 
  }
}

void loop() { 

  
    //// // select band and mode
     
     lcd.setCursor(0,2);
  int x;

  x = analogRead (2);// Read analog PIN 
 

 // select Band
  if (x>=70 and  x<200) 
{
 if(band == 3){f3=rx;band=7; rx=f7;}
  else if(band == 7){f7=rx;band=10; rx=f10;} 
   else if(band ==10){f10=rx;band=14; rx=f14;} 
  else if(band == 14){f14=rx;band=18; rx=f18;} 
  else if(band == 18){f18=rx;band=21; rx=f21;} 
  else if(band == 21){f21=rx;band=24; rx=f24;} 
   else if(band == 24){f24=rx;band=28; rx=f28;}   
 else if(band==28){f28=rx;band=3;rx=f3;}
}
  if( x>=200 and x < 400)
{ if(band == 28){f28=rx;band=24; rx=f24;}
else if(band ==24){f24=rx;band=21; rx=f21;} 
   else if(band ==21){f21=rx;band=18; rx=f18;} 
  else if(band == 18){f18=rx;band=14; rx=f14;} 
  else if(band == 14){f14=rx;band=10; rx=f10;} 
  else if(band == 10){f10=rx;band=7; rx=f7;} 
   else if(band == 7){f7=rx;band=3; rx=f3;}   
 else if(band==3){f3=rx;band=28;rx=f28;}
}
 
// end selecet Band

//Select Mode

  if (  x<70) 
  {
   {Mod++;} 
   if(Mod>3){Mod=1;}
   

  }

 
/////////// end select mod
// select VFO
 if (x>400 and  x<=600) 
 { 
  {  i++ ; }
  
  if (i>2){i=1;}
 
        
if (i==1 ){ vfoB=rx;rx=vfoA;ModB=Mod;Mod=ModA;AntB=Ant;Ant=AntA;AgcB=Agc;Agc=AgcA;ATTB=ATT;ATT=ATTA; lcd.setCursor(0,0);
    lcd.print("vfoA");}
else if (i==2 ){ vfoA=rx;rx=vfoB;ModA=Mod,Mod=ModB;AntA=Ant;Ant=AntB;AgcA=Agc;Agc=AgcB;ATTA=ATT;ATT=ATTB; lcd.setCursor(0,0);
    lcd.print("vfoB");}
}

//End select VFO
/////////////////////////////select split
if (x>600 and x<800){
  {split++;}
  if(split>1){split=0;}
  if (split==0){ lcd.setCursor(0,1);
    lcd.print("     ");}
   if (split==1) {lcd.setCursor(0,1);
    lcd.print("SPLIT");}
  
}
////////////////////////////end chek split
// Print and send  Mod
    lcd.setCursor(6,2);
    lcd.print("       ");
     lcd.setCursor(10,2);
    if (Mod==1){digitalWrite(USB,HIGH);lcd.print("USB");}
  else {digitalWrite(USB,LOW) ;}
   if (Mod==2){ {digitalWrite(LSB,HIGH) ;}lcd.print("LSB");}
    else {digitalWrite(LSB,LOW) ;}
    if (Mod==3){  {digitalWrite(CW,HIGH) ;}lcd.print("CW ");}
     else {digitalWrite(CW,LOW) ;}
 // End  Print and send  Mod
 // Print and send Ant
  int y;
  y = analogRead (A3);// Read analog PIN y
  if (y<70){
     {  Ant++ ; }
  
  if (Ant>2){Ant=1;}

  }
   lcd.setCursor(0,2);
  lcd.print("Ant   ");
     lcd.setCursor(3,2);
        
  if (Ant==1){lcd.print("A");digitalWrite(Antport,LOW) ;}
   else if (Ant==2){lcd.print("B");digitalWrite(Antport,HIGH) ;}
  // End  Print and send Ant 
  // select ATT
   if (y>200 and y<=400){
  {ATT++;}
  if (ATT>2){ATT=1;}

  }
 lcd.setCursor(5,2);
 
  if (ATT==1){lcd.print("   ");digitalWrite(ATTport,LOW) ;}
   else if (ATT==2){lcd.print("ATT");digitalWrite(ATTport,HIGH) ;}
  //End select ATT
  
  //Select AGC
  if (y>070 and y<=200){
  {Agc++;}
  if (Agc>2){Agc=1;}

  }
 lcd.setCursor(6,1);
  lcd.print("Agc ");
     lcd.setCursor(9,1);
    
  if (Agc==1){lcd.print("S");digitalWrite(Agcport,LOW) ;}
   else if (Agc==2){lcd.print("F");digitalWrite(Agcport,HIGH) ;}
  
  //Show Battery Voltage
lcd.setCursor(14,2);
lcd.print("      ");

float  Volt;

 Volt = analogRead (A4);
 lcd.setCursor(14,2);
 lcd.print(Volt/68);
 lcd.setCursor(18,2); 
 lcd.print("V "); 
 //end Battery Voltage
 //Show Power or Rx S
 Pwr = analogRead (A7);
 S = analogRead (A6);
lcd.setCursor(0,3);
int Level;
if (k==0){Level=S/12;lcd.print(" S ");}//maximum voltage for S meter is 1 Volt
else {Level=Pwr/12;lcd.print("PWR");}//maximum voltage for Power is 1 Volt

 lcd.setCursor(3,3);
if( Level==0){ lcd.print("                 ");}
else if (Level==1) {lcd.write(255);lcd.print("               ");}
else if (Level==2){lcd.write(255);lcd.write(255);lcd.print("               ");}
else if (Level==3){lcd.write(255);lcd.write(255);lcd.write(255);lcd.print("              ");}
else if (Level==4){lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.print("             ");}
else if (Level==5){lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.print("            ");}
else if (Level==6){lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.print("           ");}
else if (Level==7){lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.print("          ");}
else if (Level==8){lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.print("         ");}
else if (Level==9){lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.print("        ");}
else if (Level==10){lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);
lcd.write(255);lcd.write(255);lcd.print("       ");}
else if (Level==11){lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);
lcd.print("      ");}
else if (Level==12){lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);
lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.print("     ");}
else if (Level==13){lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);
lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.print("    ");}
else if (Level==14){lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);
lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.print("  ");}
else if (Level==15){lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);
lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.print(" ");}
else if (Level>=16){lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);
lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);lcd.write(255);}

  delay (250);

  
 
  // Update the display and frequency if the new Freq NEQ the old Freq  
  //Here can add Band 
  if (rx != rx2){   
        showFreq();
        sendFrequency(rx);
        rx2 = rx;        
      } 
     
  // Rotate through the rate of tuning as you hold down the button
  buttonstate = digitalRead(A0);
  if(buttonstate == LOW) {
        setincrement();        
    };
    
  // Check for PIN low to drive IF offset Freq -TX RX on off
  buttonstate = digitalRead(A5);  
    if (buttonstate != buttonstate2){
        if(buttonstate == LOW) {       
              lcd.setCursor(18,0);
              lcd.print("TX"); 
            
                // chek for split
            if (k==1){ goto a;}
        { k=1;}
            if(split==1 )
 {
{i++;}
if(i>2){i=1;}
if (i==1 ){ vfoB=rx;rx=vfoA;ModB=Mod;Mod=ModA;AntB=Ant;Ant=AntA; lcd.setCursor(0,0);
    lcd.print("vfoA");}
else if (i==2 ){ vfoA=rx;rx=vfoB;ModA=Mod,Mod=ModB;AntA=Ant;Ant=AntB; lcd.setCursor(0,0);
    lcd.print("vfoB");}
 
 } 

            a:;
             // GoIF = 0; //change it to GoIF=1 for IF frequency shit for both TX and RX Mode
              buttonstate2 = buttonstate; 
              sendFrequency(rx);         
          }
        else{
            // for split mod
          if (k==1){
           if(split==1 )
 {
 {i++;}
if(i>2){i=1;} 
if (i==1 ){ vfoB=rx;rx=vfoA;ModB=Mod;Mod=ModA;AntB=Ant;Ant=AntA; lcd.setCursor(0,0);
    lcd.print("vfoA");}
else if (i==2 ){ vfoA=rx;rx=vfoB;ModA=Mod,Mod=ModB;AntA=Ant;Ant=AntB; lcd.setCursor(0,0);
    lcd.print("vfoB");}
 
 } 
          }
         { k=0;}// add for split
            lcd.setCursor(18,0);
            lcd.print("RX");
            GoIF = 1;
            buttonstate2 = buttonstate;
            sendFrequency(rx);       
          };
    };
    
    // Write the frequency to memory if not stored and 2 seconds have passed since the last frequency change.
    if(memstatus == 0){   
      if(timepassed+2000 < millis()){
        storeMEM();
        }
      }  
    
 }
 
 
// Interrupt routine to catch the rotary encoder
//ISR(PCINT2_vect)
 void MyIsr(void)   //Thid added for Mega 2560
{
  unsigned char result = r.process();
  if (result) {    
    if (result == DIR_CW){rx=rx+increment;}
    else {rx=rx-increment;};       
      if (rx >=30000010){rx=rx2;}; // UPPER VFO LIMIT
      if (rx <=100000){rx=rx2;}; // LOWER VFO LIMIT
  } 
}

// frequency calc from datasheet page 8 = <sys clock> * <frequency tuning word>/2^32
void sendFrequency(double frequency) {  
  if (GoIF == 1){frequency=frequency+iffreq;}; //If pin = low, Add the IF frequency.
  int32_t freq = frequency * 4294967295/125000000;  // note 125 MHz clock on 9850.  You can make 'slight' tuning variations here by adjusting the clock frequency.
  for (int b=0; b<4; b++, freq>>=8) {
    tfr_byte(freq & 0xFF);
  }
  tfr_byte(0x000);   // Final control byte, all 0 for 9850 chip
  pulseHigh(FQ_UD);  // Done!  Should see output
}
// transfers a byte, a bit at a time, LSB first to the 9850 via serial DATA line
void tfr_byte(byte data)
{
  for (int i=0; i<8; i++, data>>=1) {
    digitalWrite(DATA, data & 0x01);
    pulseHigh(W_CLK);   //after each bit sent, CLK is pulsed high
  }
}

void setincrement(){
  if(increment == 10){increment = 100; hertz = " 100Hz"; hertzPosition=12;}
  else if (increment == 100){increment = 1000; hertz="  1KHz"; hertzPosition=12;}
   else if (increment == 1000){increment = 10000; hertz=" 10KHz"; hertzPosition=12;}
  else if (increment == 10000){increment = 100000; hertz="100KHz"; hertzPosition=12;}
  else if (increment == 100000){increment = 1000000; hertz="  1MHz"; hertzPosition=12;}  
  else{increment = 10; hertz = "  10Hz"; hertzPosition=12;};  

   lcd.setCursor(14,1); 
   lcd.print(hertz); 
   delay(250); // Adjust this delay to speed up/slow down the button menu scroll speed.
};

void showFreq(){
    millions = int(rx/1000000);
    hundredthousands = ((rx/100000)%10);
    tenthousands = ((rx/10000)%10);
    thousands = ((rx/1000)%10);
    hundreds = ((rx/100)%10);
    tens = ((rx/10)%10);
    ones = ((rx/1)%10);

  
    
      if(millions <=4 ) { digitalWrite(M3,HIGH); band=3;}// Select Filter Band 3.5
   else {digitalWrite(M3,LOW) ;}
   
    if( millions>4 and millions <=9  ) { digitalWrite(M7,HIGH);band=7;}//Select Filter Band 7 MHz
   else {digitalWrite(M7,LOW) ;}
   //test

     if(millions>9  and millions <=12  ) { digitalWrite(M10,HIGH);band=10;}//Select Filter Band 10 MHZ
   else {digitalWrite(M10,LOW) ;}

    if(millions>12 and millions <=16  ) { digitalWrite(M14,HIGH); band=14;}//Select Filter Band 14 MHz
   else {digitalWrite(M14,LOW) ;}

     if(millions> 16  and millions <=19 ) { digitalWrite(M18,HIGH); ;band=18;}//Select Filter Band 18 MHz
   else {digitalWrite(M18,LOW) ;}

    if(millions> 19 and millions <=23  ) { digitalWrite(M21,HIGH); ;band=21;}//Select Filter Band 21 MHz
   else {digitalWrite(M21,LOW) ;}


     if(millions>23 and millions <=26  ) { digitalWrite(M24,HIGH);band=24;}//Select Filter Band  24 MHz
   else {digitalWrite(M24,LOW) ;}

    if(millions> 26 and millions <=30  ) { digitalWrite(M28,HIGH); band=28;}//Select Filter Band 28 MHz
   else {digitalWrite(M28,LOW) ;}

   // End band select
   
    lcd.setCursor(4,0);
    lcd.print("              ");
   if (millions > 9){lcd.setCursor(5,0);}
   else{lcd.setCursor(6,0);}
    lcd.print(millions);
    lcd.print(".");
    lcd.print(hundredthousands);
    lcd.print(tenthousands);
    lcd.print(thousands);
    lcd.print(".");
    lcd.print(hundreds);
    lcd.print(tens);
   // lcd.print(ones);
    lcd.print("MHz");
    timepassed = millis();
    memstatus = 0; // Trigger memory write
};

void storeMEM(){
  //Write each frequency section to a EPROM slot.  
   EEPROM.write(0,millions);
   EEPROM.write(1,hundredthousands);
   EEPROM.write(2,tenthousands);
   EEPROM.write(3,thousands);
   EEPROM.write(4,hundreds);       
   EEPROM.write(5,tens);
   EEPROM.write(6,ones);   
   memstatus = 1;  // Let program know memory has been written
};


