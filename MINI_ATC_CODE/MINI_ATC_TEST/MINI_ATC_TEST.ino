
/*
 
╔═╗╔═╦══╦═╗─╔╦══╗
║║╚╝║╠╣╠╣║╚╗║╠╣╠╝
║╔╗╔╗║║║║╔╗╚╝║║║
║║║║║║║║║║╚╗║║║║
║║║║║╠╣╠╣║─║║╠╣╠╗
╚╝╚╝╚╩══╩╝─╚═╩══╝
╔═══╦════╦═══╗
║╔═╗║╔╗╔╗║╔═╗║
║║─║╠╝║║╚╣╚══╗
║╚═╝║─║║─╚══╗║
║╔═╗║─║║─║╚═╝║
╚╝─╚╝─╚╝─╚═══╝

DATE 
2022/8/30
*/
#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//delay
int d1 = 1000;
float CalVal = 11.08433; 

//Timing variables
unsigned long previousMillis = 0;
const long interval = 200;          //Interval to read voltages

// input pins 
const int redPhaseIn = 13;
const int yellowPhaseIn = 10;


// output pins 
const int redPhaseOut = 6;
const int yellowPhaseOut = 8;


// variable for reading input phase status. initial false
bool redState = false;
bool yellowState = false;
bool allPhaseOn = false;

//variable states initial low
int redPhaseState = LOW;
int yellowPhaseState = LOW;



void setup() {
  
//set pins  
pinMode(redPhaseIn, INPUT);
pinMode(yellowPhaseIn, INPUT);


pinMode(redPhaseOut, OUTPUT);
pinMode(yellowPhaseOut, OUTPUT);


pinMode(A0,INPUT);
pinMode(A1,INPUT);
pinMode(A2,INPUT);

//Initialize serial monitor
Serial.begin(9600);
Serial.println("3 PHASE AUTOMATIC CHANGE OVER SYSTEM");
delay (2000);
Serial.println("Processing available PHASE lines...");
  // initialize the LCD
  analogWrite(8,25);
  lcd.begin(16, 2);  
  lcd.print("3PHASE POWER SYS!");
  delay(2000);
  lcd.setCursor(0,1);
  lcd.print("Initializing sys"); 
  digitalWrite ( redPhaseOut,LOW);
  digitalWrite ( yellowPhaseOut,LOW);
  delay(2000);
}

void loop() {
  //millis for multi taskig
   unsigned long currentMillis = millis();
  //check state of each input
     redPhaseState = digitalRead(redPhaseIn);
     yellowPhaseState = digitalRead(yellowPhaseIn);
     
/*
╔═══╦╗╔╗─╔═══╦╗
║╔═╗║║║║─║╔═╗║║
║║─║║║║║─║╚═╝║╚═╦══╦══╦══╗
║╚═╝║║║║─║╔══╣╔╗║╔╗║══╣║═╣
║╔═╗║╚╣╚╗║║──║║║║╔╗╠══║║═╣
╚╝─╚╩═╩═╝╚╝──╚╝╚╩╝╚╩══╩══╝
*/   
  if ( redPhaseState == HIGH && yellowPhaseState == HIGH && currentMillis - previousMillis >= interval && allPhaseOn == false )
  {
    allPhaseOn = checkallPhase();
    previousMillis = currentMillis;
    Serial.println("All Phase are On");
    lcd.clear();
    lcd.setCursor (0,0); // set to line 1, char 0  
    lcd.print("3PHASE POWER SYS");

    delay (d1);
    lcd.setCursor (0,1); // set to line 1, char 0  
    lcd.print("All phase are ON!");
    digitalWrite ( redPhaseOut,HIGH);
    digitalWrite ( yellowPhaseOut,LOW);
    
    delay (d1);
    lcd.clear();
    while(allPhaseOn == true){
        
        lcd.setCursor (0,0); // set to line 1, char 0  
        lcd.print("CEB IN USE");
        lcd.setCursor (0,1); // set to line 1, char 0  
        lcd.print(voltageReadingCeb(A2));
        lcd.setCursor(6,1);
        lcd.print("V input");
        allPhaseOn = checkallPhase();
    }   
   }
   else{
/*
╔═══╗────╔╗╔═══╦╗
║╔═╗║────║║║╔═╗║║
║╚═╝╠══╦═╝║║╚═╝║╚═╦══╦══╦══╗
║╔╗╔╣║═╣╔╗║║╔══╣╔╗║╔╗║══╣║═╣
║║║╚╣║═╣╚╝║║║──║║║║╔╗╠══║║═╣
╚╝╚═╩══╩══╝╚╝──╚╝╚╩╝╚╩══╩══╝
*/
 
    if ( redPhaseState == HIGH)
      {
      Serial.println("CEB is on");
      
    lcd.clear();
    lcd.setCursor (0,0); // set to line 1, char 0  
    lcd.print("3PHASE POWER SYS");
    
    delay (d1);
   
    digitalWrite ( redPhaseOut,HIGH);
    digitalWrite ( yellowPhaseOut,LOW);
   
    delay (d1);
    lcd.clear();
    
    while( checkRedPhase()== true && checkallPhase() == false ){
      
      lcd.setCursor (0,0); // set to line 1, char 0  
      lcd.print("CEB IN USE");
      lcd.setCursor (0,1); // set to line 1, char 0  
      lcd.print(voltageReadingCeb(A2));
      lcd.setCursor(6,1);
      lcd.print("V input");
      redState = checkRedPhase();
    }
      }
/*
╔╗──╔╗─╔╗╔╗─────────╔═══╦╗
║╚╗╔╝║─║║║║─────────║╔═╗║║
╚╗╚╝╔╩═╣║║║╔══╦╗╔╗╔╗║╚═╝║╚═╦══╦══╦══╗
─╚╗╔╣║═╣║║║║╔╗║╚╝╚╝║║╔══╣╔╗║╔╗║══╣║═╣
──║║║║═╣╚╣╚╣╚╝╠╗╔╗╔╝║║──║║║║╔╗╠══║║═╣
──╚╝╚══╩═╩═╩══╝╚╝╚╝─╚╝──╚╝╚╩╝╚╩══╩══╝
*/
      else if ( yellowPhaseState == HIGH  )
      {
      
       Serial.println("Solar is on"); 
        lcd.clear();
    lcd.setCursor (0,0); // set to line 1, char 0  
    lcd.print("3PHASE POWER SYS");
    
    delay (d1);
    lcd.setCursor (0,1); // set to line 1, char 0  
    lcd.print("Solar ON!");
    digitalWrite ( redPhaseOut,LOW);
    digitalWrite ( yellowPhaseOut,HIGH);
    
    delay (d1);
    lcd.clear();
    
    while(checkYellowPhase() == true && checkRedPhase()==false ){
      lcd.setCursor (0,0); // set to line 1, char 0  
      lcd.print("Solar ON!");
      lcd.setCursor (0,1); // set to line 1, char 0  
      lcd.print(voltageReadingSolar(A1));
      lcd.setCursor(6,1);
      lcd.print("V input");
      yellowState = checkYellowPhase();
    }
    
      }


/*
╔═══╦╗──╔╗───╔═══╦╗─╔╦═══╦═══╦═══╗╔═══╦═══╦═══╗
║╔═╗║║──║║───║╔═╗║║─║║╔═╗║╔═╗║╔══╝║╔═╗║╔══╣╔══╝
║║─║║║──║║───║╚═╝║╚═╝║║─║║╚══╣╚══╗║║─║║╚══╣╚══╗
║╚═╝║║─╔╣║─╔╗║╔══╣╔═╗║╚═╝╠══╗║╔══╝║║─║║╔══╣╔══╝
║╔═╗║╚═╝║╚═╝║║║──║║─║║╔═╗║╚═╝║╚══╗║╚═╝║║──║║
╚╝─╚╩═══╩═══╝╚╝──╚╝─╚╩╝─╚╩═══╩═══╝╚═══╩╝──╚╝
*/ 
      else
      {
        Serial.println("All phases are off");
         lcd.clear();
    lcd.setCursor (0,0); // set to line 1, char 0  
    lcd.print("3PHASE POWER SYS");
    
    delay (d1);
    
    digitalWrite ( redPhaseOut,LOW);
    digitalWrite ( yellowPhaseOut,LOW);
    

    while(checkRedPhase()==false && checkYellowPhase()==false){
    lcd.setCursor (0,1); // set to line 1, char 0  
    lcd.print("ALL PHASE OFF!");
    }
      }
      }
      
}
/*
╔═══╦╗─╔╦═╗─╔╦═══╦════╦══╦═══╦═╗─╔╦═══╗
║╔══╣║─║║║╚╗║║╔═╗║╔╗╔╗╠╣╠╣╔═╗║║╚╗║║╔═╗║
║╚══╣║─║║╔╗╚╝║║─╚╩╝║║╚╝║║║║─║║╔╗╚╝║╚══╗
║╔══╣║─║║║╚╗║║║─╔╗─║║──║║║║─║║║╚╗║╠══╗║
║║──║╚═╝║║─║║║╚═╝║─║║─╔╣╠╣╚═╝║║─║║║╚═╝║
╚╝──╚═══╩╝─╚═╩═══╝─╚╝─╚══╩═══╩╝─╚═╩═══╝
*/

//check all state
bool checkallPhase(){
  bool nstate;
  if( checkRedPhase() == true && checkYellowPhase() == true ){nstate = true;}
  else{nstate = false;}
  delay(50);  
  return nstate;//return bool value
  
}

//check red state 
bool checkRedPhase(){
  bool nstate;
  redPhaseState = digitalRead(redPhaseIn);
  if(redPhaseState == HIGH){nstate = true;}
  else{nstate = false;}
  delay(50);  
  return nstate;//return bool value
  
  }

//check yellow state 
bool checkYellowPhase(){
  bool nstate;
  yellowPhaseState = digitalRead(yellowPhaseIn);
  if( yellowPhaseState == HIGH && (voltageReadingSolar(A2)>=10)){nstate = true;}
  else{nstate = false;}
  delay(50);
  return nstate;//return bool value
    
  }


/*
bool checkYellowPhase(){
  bool nstate;
  yellowPhaseState = digitalRead(yellowPhaseIn);
  if( yellowPhaseState == HIGH){nstate = true;}
  else{nstate = false;}
  delay(50);
  return nstate;//return bool value
    
  }
*/

//voltage reding function
float voltageReadingCeb(int analogPin){
    float VIn = (analogRead(analogPin)* 5) / 1024.00;     //Convert 10bit input to an actual voltage
    float Voltage = (VIn * CalVal)+0.90;
    float AC_LOW_VOLT = Voltage;
    float AC_HIGH_VOLT = ( AC_LOW_VOLT * 12.3284);
    delay(50);
    return AC_HIGH_VOLT;
    
}

float voltageReadingSolar(int analogPin){
    float VIn = (analogRead(analogPin)* 5) / 1024.00;     //Convert 10bit input to an actual voltage
    float Voltage = (VIn * CalVal)+0.90;
    float AC_LOW_VOLT = Voltage;
    delay(50);
    return AC_LOW_VOLT;
    
}
