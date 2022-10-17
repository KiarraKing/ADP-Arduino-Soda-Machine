int numberCollector;  // to build the number from serial
byte serialNumber = 0;  // to store the ASCI number from serial
int opticCount = 6;  // how many optic stations there are 
int parameterCount = 3; //number of seeting to be stored in array for each optic
int parameterSize = 2; //the required number of digit for each parameter being sent

int drinkMatrix[6][3] = {
  };

//CONTACT SWITCH!!

const int  buttonPin = 2;    // pin --> pushbutton 
int buttonState = 0;          
int lastButtonState = 0;     

//STEPPER MOTORS!!

#include <AFMotor.h>
AF_Stepper motor1(48, 1);
AF_Stepper motor2(48, 2);

//RGB LED!!

int greenPin = 10;
int bluePin = A1;
int redPin = 9;

//RUN MACHINE!!

int raftFound = false;
int drinkRequested = false;

void setup() {

//FOR THE CONTACT SWITCH  

  // initialize the button pin as a input:
  pinMode(buttonPin, INPUT);
  // initialize the LED as an output:
  pinMode(LED_BUILTIN, OUTPUT);
  // initialize serial communication:
  Serial.begin(9600);

//FOR THE STEPPER MOTORS 
  motor1.setSpeed(600);
   motor2.setSpeed(600);

//FOR LED

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  
 //VOID LOOP(){ 
  
//Lets find the location of the float:
if (raftFound == false){
  Serial.println("Raft location not known yet");
  setColor(255, 0, 0);  // red
  delay(1000);
  Serial.print("Looking for the raft...");
  buttonState = digitalRead(buttonPin); // read the pushbutton input pin:
//move the stepper until the contact switch is triggered
  while(buttonState == LOW && raftFound == false){
    motor1.step(10, FORWARD, SINGLE); 
    buttonState = digitalRead(buttonPin);  
   }
 raftFound = true;
 Serial.println("ahh! There it is. :)");
 motor1.release();
 setColor(0, 255, 0);  // blue
 delay(700); // Delay a little bit to calm everything down
} 


//working through the dispensing instructions until drink is complete and paddle is at last optic position:
if (drinkRequested == true){  
  for (int optic = 0; optic < opticCount; optic++) {
     //Move to pump
      motor1.step((drinkMatrix[optic][0] * 10), BACKWARD, SINGLE); //move the paddle according to instruciton, x10 to allow us to compress serial data transfer length
     motor1.release(); // let the motor's magnets relax
    
  //dispense what is required then resume moving to the next position:
     while(drinkMatrix[optic][2] > 0 && raftFound == true){
    delay(500);
        motor2.step(2100, BACKWARD, DOUBLE);
        delay((drinkMatrix[optic][1]) * 100);
       motor2.step(2100, FORWARD, DOUBLE);
       motor2.release();
       drinkMatrix[optic][2] = drinkMatrix[optic][2]-1;
       delay(500); 
    }
  }

//Drink complete  
  Serial.println("Drinks ready, enjoy.");
  setColor(0, 0, 255);  // green
  drinkRequested = false;
  raftFound=false;
}

 while(drinkRequested == false){
 delay(200);

   if (Serial.available()) {
    for (int optic = 0; optic < opticCount; optic++){
      for (int parameter = 0; parameter < parameterCount; parameter++){     
        for (int parameterMeasure = 0; parameterMeasure < parameterSize; parameterMeasure++){
          if (Serial.available()) {
            serialNumber = Serial.read(); /* load the number from serial buffer */
            serialNumber = serialNumber - 48; /* convert number to text number */
            numberCollector = numberCollector * 10 + serialNumber; /*store and build the number*/
          } else {
            delay(250);
            serialNumber = Serial.read(); /* load the number from serial buffer */
            serialNumber = serialNumber - 48; /* convert number to text number */
            numberCollector = numberCollector * 10 + serialNumber; /*store and build the number*/     
          }
        }
       drinkMatrix[optic][parameter] = numberCollector;   /* store the value in the array  */
       numberCollector =  0;   /* Prepare variable for next number  */
       serialNumber = Serial.read(); /* to clear the comma from the buffer */
      }
      }
    CheckArray();
    Serial.println("Done loading");
    drinkRequested = true;
    }
    
 }
    
}


void CheckArray(){
//print out the array to check it:
for(int i = 0; i < opticCount; i++) {
  for(int j = 0; j < 3; j++) {
    Serial.print(drinkMatrix[i][j]);
    Serial.print(",");
  }
  Serial.println();
}
}

void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}
