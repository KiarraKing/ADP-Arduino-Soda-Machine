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
