/* The program below is for a beverage vending machine that dispenses two kinds of beer i.e Tusker and Guinnes. 
 *  The vending unit utilizes a single dispensing unit that dispenses from the two storage sections.
When a servo motor is rotated clockwise, the tusker brand is dispensed and when rotated counter clockwise, the guinnes brand is dispensed.*/


#include <Servo.h>
#include <Wire.h>                                            // Library for I2C communication
#include <LiquidCrystal_I2C.h>                               // Library for LCD
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);      

Servo myservo;                                               // servo object to control a servo
int TuskerPin = 12;
int GuinnesPin = 13;
int servoPin =  6;
int CoinPin = 0;

volatile float CoinValue = 0.00;                            //assigning float for constant changes


int TuskerPinStatus = 0;
int GuinnesPinStatus = 0;
//flags 
int CoinChange = 0;                                        // coin insert flag. indicates when the coin has been inserted
int Customer_waiting = 1;                                  // flag for customer waiting
int beer_choice1 = 1;                                      // flag for brand 1 if chosen
int beer_choice2 = 1;                                      // flag for brand 2
int finished = 0;                                          // flag for complete dispensing

int angle = 0;                                            //variable to store servo position in degrees


void payment()
{
  
  CoinValue = CoinValue + 5.00;
  CoinChange = 1;
  
 }

void setup() {
  
  Serial.begin(9600);
  
  pinMode( TuskerPin, INPUT);
  pinMode( GuinnesPin, INPUT);
  pinMode( servoPin, OUTPUT);


  //payment part
  digitalWrite(CoinPin, HIGH);  //pull up
  attachInterrupt(0 , payment, RISING);
  
  // Initiate the LCD:
  lcd.init();
  lcd.backlight(); 
  
  myservo.attach(servoPin);

}


void loop() {
  
  
  if(Customer_waiting == 1)
      {
          lcd.clear();
          CoinValue = 0;
          //lcd.setCursor(0, 0);
          lcd.print("VENDING MACHINE");
          
          lcd.setCursor(0,1);
          lcd.print("SELECT BEER");
          Customer_waiting = 0;
      }
   
    TuskerPinStatus = digitalRead(TuskerPin);
    if(TuskerPinStatus == HIGH && beer_choice2==1)
        {
          beer_choice1 = 0;
          lcd.clear();
          lcd.print("TUSKER=KSH:30");
          delay(1000);
          lcd.setCursor(0,1);
          lcd.print("INSERT COINS");
        
          
        }

          if(CoinValue == 30 && beer_choice1==0)
        {
          CoinValue=0;
          lcd.clear();
          lcd.print("AMOUNT ENOUGH");
          delay(3000);
          lcd.clear();
          lcd.print("PRODUCT IS BEING");
          lcd.setCursor(0,5);
          lcd.print("DISPENSED");
          delay(500);
          
          /*dispensing from top storae section. involves roatating the dispensing mechanism
          clockwise hence dispensing and then counter clockwise to return the dispensing unit to its home position*/
          myservo.write(135);              // rotate clockwise full speed
          delay(3000);
          myservo.write(100);                  // stop
          delay(2000);
          myservo.write(45);              // rotate counterclockwise full speed
          delay(700);                     
          myservo.write(100);                  // stop
         
          
          finished = 1;
 
        }
        
        GuinnesPinStatus = digitalRead(GuinnesPin);
         if(GuinnesPinStatus ==  HIGH && beer_choice1==1)
        {
          beer_choice2 = 0;
          lcd.clear();
          lcd.print("GUINNES=KSH:40");
          delay(1000);
          lcd.setCursor(0,1);
          lcd.print("INSERT COINS");
          delay(500);
        }
          

          if(CoinValue == 40 && beer_choice2==0)
        {
          CoinValue = 0;
          lcd.clear();
          lcd.print("AMOUNT ENOUGH");
          delay(3000);
          lcd.clear();
          lcd.print("PRODUCT IS BEING");
          lcd.setCursor(1,5);
          lcd.print("DISPENSED");
          delay(500);
          
          /*dispensing from bottom storage section. involves roatating the dispensing mechanism
          counter clockwise hence dispensing and then clockwise to return the dispensing unit to its home position*/   
           myservo.write(45);                // rotate clockwise full speed
           delay(2000);
           myservo.write(100);                  // stop
           delay(2000);
           myservo.write(135);                // rotate counterclockwise full speed
           delay(800);                   
           myservo.write(100);                  // stop
           
           finished = 1;
        }
            
             if (finished == 1)
            {
             lcd.clear();
            lcd.print("THANK YOU");
            delay(4000);
            lcd.clear();
            // Reset flags
            Customer_waiting = 1;
            beer_choice1 = 1;
            beer_choice2 = 1;
            finished = 0;
            }
            
             if (CoinChange == 1 )
            {
              
            CoinChange = 0;
            delay(200);
            lcd.clear();
            lcd.print("INSERTED KSH:");
            lcd.setCursor(0,1);
            lcd.print(CoinValue);
            
           }
}
