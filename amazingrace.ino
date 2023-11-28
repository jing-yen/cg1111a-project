// Code for The Amazing Race - Team 3 B06
#include <MeMCore.h>
#include <math.h>

MeBuzzer buzzer;
MeRGBLed led(0, 30);
MeLineFollower lineFinder(PORT_1);
MeDCMotor leftMotor(M1);
MeDCMotor rightMotor(M2);

// Time delay (in milliseconds):
#define LDRWait 10 // between LDR readings for same RGB colour
#define RGBWait 200 // after the next RGB colour turns ON to allow LDR to stabilize
#define TURN_DELAY 1000 // for the robot to execute a 90 degree turn
#define DOUBLE_TURN_DELAY 650 // for the robot to move straight in a double turn

// Constants for ultrasonic sensor
#define TIMEOUT 2000
#define SPEED_OF_SOUND 340
#define ULTRASONIC 10

#define LDR A3 // LDR sensor pin at A0
#define IR A2 // IR sensor pin at A0

// Define colour sensor LED pins
int decoderPin[] = {A0, A1};

//floats to hold colour arrays
bool ended = false;
int nudgeDir = 0; 
// 0: red, 1: green, 2: blue, 3: orange, 4: purple, 5: white
// find max & min
int rgbopw[6][3] = {{214,135,145},{120,168,126},{165,212,230},{241,196,182},{175,161,192},{262,259,265}};
int real_rgbopw[6][3] = {{255,0,0},{0,255,0},{0,0,255},{255,165,0},{128,0,128},{255,255,255}};
float whiteArray[] = {932.0,970.0,955.0};
float blackArray[] = {763.0,844.0,796.0};
float greyDiff[] = {169.0,126.0,159.0};


char colourStr[3][5] = {"R = ", "G = ", "B = "};

// Makes a tone with the mBot speaker
void buzz(int freq) {
  buzzer.tone(freq, 10);
  buzzer.noTone();
}

// Motor functions: Stopping, moving forward, turning, double turning, and u-turning
void stopMotor() {
  leftMotor.run(0);
  rightMotor.run(0);
}

void moveForward() {
  leftMotor.run(-300);
  rightMotor.run(300);
}

void turnRight() {
  leftMotor.run(-180);
  rightMotor.run(-180);
  delay(TURN_DELAY/2);
  stopMotor();
  delay(100);
  moveForward();
}

void turnLeft() {
  leftMotor.run(180);
  rightMotor.run(180);
  delay(TURN_DELAY/2);
  stopMotor();
  delay(100);
  moveForward();
}

void turnRightMore() {
  leftMotor.run(-200);
  rightMotor.run(-200);
  delay(TURN_DELAY/2);
  stopMotor();
  delay(100);
  moveForward();
}

void turnLeftMore() {
  leftMotor.run(200);
  rightMotor.run(200);
  delay(TURN_DELAY/2);
  stopMotor();
  delay(100);
  moveForward();
}

void uTurn() {
  leftMotor.run(140);
  rightMotor.run(140);
  delay(TURN_DELAY);
}

void doubleLeftTurn() {
  turnLeft();
  delay(DOUBLE_TURN_DELAY);
  turnLeftMore();
}

void doubleRightTurn() {
  turnRight();
  delay(DOUBLE_TURN_DELAY);
  turnRightMore();
}

// Nudging functions: subtle movements to move reorient mBot
void nudgeLeft(int speed, bool isCompensate) {
  nudgeDir = isCompensate? 0: -1;
  leftMotor.run(-speed);
  rightMotor.run(300);
}

void nudgeRight(int speed, bool isCompensate) {
  nudgeDir = isCompensate? 0: 1;
  leftMotor.run(-300);
  rightMotor.run(speed);
}

// Functions to shine each IR and RGB LEDs
// Outputs based on 2-to-4 decoder chip documentation => Y0: IR, Y1: G, Y2: B, Y3: R
void shineIR(bool isOn){
  if (isOn) {
    digitalWrite(decoderPin[0], LOW);
    digitalWrite(decoderPin[1], LOW);
  } else {
    digitalWrite(decoderPin[0], HIGH);
    digitalWrite(decoderPin[1], HIGH);
  }
}

void shineRed(){
  digitalWrite(decoderPin[0], HIGH);
  digitalWrite(decoderPin[1], HIGH);
}

void shineGreen(){
  digitalWrite(decoderPin[0], HIGH);
  digitalWrite(decoderPin[1], LOW);
}

void shineBlue(){
  digitalWrite(decoderPin[0], LOW);
  digitalWrite(decoderPin[1], HIGH);
}

// Returns the Euclidean distance between two colours
long euclidean(long delta_r, long delta_g, long delta_b) {
  return ((delta_r*delta_r)+(delta_g*delta_g)+(delta_b*delta_b));
}

// Returns the colour with lowest Euclidean distance
int find_colour(long *distance) {
  long min = distance[0];
  int index = 0;
  for (int i=1; i<6; i+=1) {      
    if (distance[i]<min) {
      index = i;
      min = distance[i];
    }
  }
  return index;
}

// Returns the average analogRead values of either IR or LDR sensor
int getAvgReading(int times, int pin){      
  int reading;
  int total = 0;
  for(int i=0; i<times; i++){
    reading = analogRead(pin);
    total = reading + total;
    delay(LDRWait);
  }
  return total/times;
}

// Functions to detect IR and detect colour
int detectIR()
{
  int voltage = getAvgReading(5,IR);
  return voltage;
}

int detectColour(int i) {
  Serial.print(colourStr[i]);
  i==0 ? shineRed(): (i==1 ? shineGreen() : shineBlue());
  delay(RGBWait);
  return (int)((getAvgReading(2,LDR) - blackArray[i])/(greyDiff[i])*255);
}

int ultrasonic() {
  // Send out ultrasonic beam
  pinMode(ULTRASONIC, OUTPUT);
  digitalWrite(ULTRASONIC, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC, LOW);

  // Receive reflected ultrasonic beam
  pinMode(ULTRASONIC, INPUT);
  long duration = pulseIn(ULTRASONIC, HIGH, TIMEOUT);

  // Decide where to move according to distance to left wall
  if (duration > 0) {
    long length = duration/2.0/1000000*SPEED_OF_SOUND*100;
    if (length>18) {
      moveForward();
    } else if (length>10) {
      nudgeLeft(255-9*(length-10), false);
      buzz(200);
    } else if (length<10) {
      nudgeRight(255-9*(10-length), false);
      buzz(300);
    } else {
      moveForward();
    }
  }
}

void setup(){
  // Set pins and begin serial.
  led.setpin(13);
  Serial.begin(9600);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(LDR, INPUT);
  pinMode(IR, INPUT);
  
  //setBalance();

  warmup(); // Do a detect colour run before running, to warmup the sensor.
}

void loop() {
   if (!ended) {
    // Do opposite nudge to straighten the robot, if needed.
    if (nudgeDir!=0) {
      nudgeDir==-1 ? nudgeRight(210, true): nudgeLeft(210, true);
    }
    // Call ultrasonic to keep robot at centre of path.
    ultrasonic();
    // Check if robot reaches the black line.
    if (lineFinder.readSensors() == S1_IN_S2_IN) {
      stopMotor();
      reach_line();
    }
  }
}

void warmup() {
  for (int c =0; c < 3; c++) {
    detectColour(c);
  }
}

void reach_line() {
  // Obtain the RGB values of colour paper.
  delay(300);
  int r = detectColour(0);  
  Serial.println(r);  
  int g = detectColour(1);  
  Serial.println(g);  
  int b = detectColour(2);
  Serial.println(b);
  shineRed();
 
  // Identify the colour, based on Euclidean distance with scanned values. 
  long distance[6];
  for(int i=0; i<6; i++){   
    distance[i] = euclidean((long)(r-rgbopw[i][0]), (long)(g-rgbopw[i][1]), (long)(b-rgbopw[i][2]));
  }
  int colour = find_colour(distance);
  led.setColor(real_rgbopw[colour][0],real_rgbopw[colour][1],real_rgbopw[colour][2]);
  led.show();

  // Make a decision on where to move.
  if (colour==0) {
    turnLeft();
  } else if (colour==1) {
    turnRight();
  } else if (colour==2) {
    doubleRightTurn();
  } else if (colour==3) {
    uTurn();
  } else if (colour==4) {
    doubleLeftTurn();
  } else {
    // White colour: Course ended. Celebrate!
    stopMotor();
    buzz(1000);
    delay(1000);
    buzz(1000);
    ended = true;
  }
}

void setBalance() {
  // Set white balance: scan white sample one colour at a time, and save to white array
  buzz(500);
  Serial.println("Put White Sample For Calibration ...");
  delay(5000);           // Delay for five seconds for getting sample ready
  buzz(300);
  for (int i=0; i<=2; i++){
    i==0 ? shineRed(): (i==1 ? shineGreen() : shineBlue());
    delay(RGBWait);
    whiteArray[i] = getAvgReading(5,LDR);         // Scan 5 times and return the average
  }
  buzz(1000);

  // Set black balance: scan black sample one colour at a time, and save to black array
  Serial.println("Put Black Sample For Calibration ...");
  delay(5000);
  buzz(300);
  for (int i=0; i<=2; i++){
    i==0 ? shineRed(): (i==1 ? shineGreen() : shineBlue());
    delay(RGBWait);
    blackArray[i] = getAvgReading(5,LDR);
    // Obtain the range of possible values, by deducting the minimums from the maximums
    greyDiff[i] = whiteArray[i] - blackArray[i];
  }
  buzz(1000);

  // Delay another 5 seconds for getting ready colour objects
  Serial.println("Colour Sensor Is Ready.");
  delay(5000);
  buzz(3000);
}

