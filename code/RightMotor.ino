// Pins
#define ENCA 2
#define ENCB 3
#define PWM 5
#define IN1 7
#define IN2 6
// Include Arduino Wire library for I2C
#include <Wire.h>

// Define Slave I2C Address
#define SLAVE_ADDR2 10
// globals
long prevT = 0;
int posPrev = 0;
// Use the "volatile" directive for variables
// used in an interrupt
volatile int pos_i = 0;
volatile float velocity_i = 0;  
volatile long prevT_i = 0;

float v1Filt = 0;
float v1Prev = 0;
float eprop =0, eder = 0, ePrev =0, ePast = 0;
float eintegral = 0;
float uPrev = 0;
int pwr = 80;
int vt = 86;
void setup() {
  Serial.begin(9600);
  pinMode(ENCA,INPUT);
  pinMode(ENCB,INPUT);
  pinMode(PWM,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA),readEncoder,RISING);
  Wire.begin(SLAVE_ADDR2);
 Wire.onReceive(receiveEvent);
 
  setMotor(1,86,PWM,IN1,IN2);
}
void receiveEvent() {
  // read one character from the I2C
  vt = Wire.read();
//  // Print value of incoming data
//  Serial.println(rd);
//    
}
void loop() {

  // read the position and velocity
  int pos = 0;

  noInterrupts(); // disable interrupts temporarily while reading
  pos = pos_i;

  interrupts(); // turn interrupts back on

  // Compute velocity with method 1
  long currT = micros();
  float deltaT = ((float) (currT-prevT))/1.0e6;
  float velocity1 = (pos - posPrev)/deltaT;
  posPrev = pos;
  prevT = currT;

  // Convert count/s to RPM
  float v1 = velocity1/374*60.0;
  v1Filt = 0.854*v1Filt + 0.0728*v1 + 0.0728*v1Prev;
  v1Prev = v1;

  // Set a target


  // Compute the control signal uL
  float kp = 6.6;
  float ki = 75.55;
  float kd = 0.144144;
  float e = vt-v1Filt;
  //Proportional
   eprop = e - ePrev;
   // Integration 
  eintegral = e*deltaT;

  // Derivative 
  eder = (e - 2*ePrev + ePast)/deltaT;

  // PID calculations
  float u = uPrev + kp * eprop + ki * eintegral + kd * eder;

  // Set the motor speed and direction
  int dir = 1;
  if (u<0){
    dir = -1;
  }
  pwr = fabs(u);
  if(pwr > 255){
    pwr = 255;
  }
  
//  setMotor(dir,pwr,PWM,IN1,IN2);
  if (vt == 0){
    setMotor(dir,0,PWM,IN1,IN2);
  }
  else setMotor(dir,pwr,PWM,IN1,IN2);
  uPrev = u;
  ePast = ePrev;
  ePrev = e;
  Serial.print(vt);
  Serial.print("\t");
  Serial.print(v1Filt);
  Serial.print("\t");
  Serial.print(currT/1e6);
  Serial.println();

}

void setMotor(int dir, int pwmVal, int pwm, int in1, int in2){
  analogWrite(pwm,pwmVal); // Motor speed
  if(dir == 1){ 
    // Turn one way
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
  }
  else if(dir == -1){
    // Turn the other way
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
  }
  else{
    // Or dont turn
    digitalWrite(in1,LOW);
    digitalWrite(in2,LOW);    
  }
}

void readEncoder(){
  // Read encoder B when ENCA rises
  int b = digitalRead(ENCB);
  int increment = 0;
  if(b>0){
    // If B is high, increment forward
    increment = 1;
  }
  else{
    // Otherwise, increment backward
    increment = -1;
  }
  pos_i = pos_i + increment;
  

}
