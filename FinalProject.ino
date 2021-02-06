//Salwa swelem 318198876
//Bahaa Bader 208996520

#include <LiquidCrystal.h> 
#include <Servo.h>

Servo myservo;  
LiquidCrystal lcd(5, 4, 3, 2, 1, 0);  
int LDRValue;   //The Photoresistor sensor !
int TempValue ; //The thermistor sensor

//Ultrasonic Ranging sensor 
int trigPin = 9;
int echoPin = 8;
int MAX_DISTANCE = 200 ;
float timeOut = MAX_DISTANCE * 60 ;  // define the timeOut according to the maximum range
int soundVelocity = 340; // define sound speed=340m/s

int lux ; 

void setup()
 {
   pinMode(trigPin,OUTPUT); // set trigPin to output mode
   pinMode(echoPin,INPUT);  // set echoPin to input mode
   lcd.begin(16, 2);        // set up the LCD's number of columns and rows
   myservo.attach(6);       // attaches the servo on pin number 6 
   pinMode(10, OUTPUT);     // initialize the LED pin as an output
   pinMode(11, OUTPUT);

  } 

  
void loop()
 { 
   TempValue = analogRead(A1);   // thermistor   = Read analog voltage value of A1 port
   LDRValue = analogRead(A0);    //Photoresistor = Read analog voltage value of A0 port
  
  //Function
  LCDPrinter();   
  LightEM();
  TempEM();

  
  myservo.write(40);          //The servo on CLOSE posistion 
 
 if (getDistance()<10) {      // Getting in the if statement when the distance from the sensor is less than 10cm.
     myservo.write(230);      // The servo on OPEN posistion 
     for(int i=0;i<300;i++){  // the for is used instead of "delay(5000)" 
       LCDPrinter(); 
       LightEM();
       TempEM();
       delay(1);
    }
    myservo.write(40);
  }
 
}


// Print on the LCD screen the Lighting and Temp Values 
void LCDPrinter(){  
  int Tc ; 
  TempValue = analogRead(A1); //thermistor value
  LDRValue = analogRead(A0);  //Photoresistor value

  Tc = ConvertToC(TempValue);       // Tc = Converted value from the thermistor sensor to Celsius .
  lux= sensorRawToLUX(LDRValue);    // lux = Converted value from the Photoresistor sensor to Lux .
  
  lcd.setCursor(0,0);
  lcd.print("LDR : ");
  lcd.print(lux);  
  lcd.write(" lux   ");
  
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(Tc);
  lcd.write(B11011111);  //display the temp icon ° 
  lcd.write("C      ");  
}



//Turns the Green led on when it gets dark .
void LightEM(){
  LDRValue = analogRead(A0);
  lux= sensorRawToLUX(LDRValue);
  if(lux<2){ 
  digitalWrite(10, HIGH); 
  }else{
  digitalWrite(10, LOW); }

}

//Shows Warning message and Red led blink when temp get higher than 29c .
void TempEM(){
  float Tc ; 
  TempValue = analogRead(A1);
  Tc = ConvertToC(TempValue);
 
  if(Tc>29){
    for(int j = 0 ; j <5 ; j++){
      digitalWrite(11, HIGH);
      delay(1000);
       lcd.setCursor(0, 0);
       lcd.print("  WARNING !!!!  ");
       lcd.setCursor(0, 1);
       lcd.print("Temp Reached ");
       lcd.print(Tc);
       lcd.write(B11011111);
       lcd.write("C");

      digitalWrite(11, LOW);
      delay(1000);
    }
  }
}

//Helping functions VVV 

// Returns the Distance from the Ultrasonic Ranging sensor
float getDistance() {
 unsigned long pingTime;
 float distance;
 digitalWrite(trigPin, HIGH); // make trigPin output high level lasting for 10μs 
 delayMicroseconds(10);
 digitalWrite(trigPin, LOW);
 pingTime = pulseIn(echoPin, HIGH, timeOut); // Wait HC-SR04 returning to the high level and measure out this waitting time
 distance = (float)pingTime * soundVelocity / 2 / 10000; // calculate the distance
 return distance; // return the distance value
}

//Convert the returned value from the thermistor sensor to Celsius .
int ConvertToC(int Vo){
  float R1 = 10000;
  float logR2, R2, T, Tf;
  int Tc ;
  float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07; 
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  Tc = T - 273.15;
  return Tc ; 
  }

//Convert the returned value from the Photoresistor sensor to Lux .
int sensorRawToLUX(int raw){
  float Vout = float(raw) * (5 / float(1023));// Conversion analog to voltage
  float RLDR = (10000 * (5 - Vout))/Vout; // Conversion voltage to resistance
  int lux=500/(RLDR/1000); // Conversion resitance to lumen
  return lux;
}
