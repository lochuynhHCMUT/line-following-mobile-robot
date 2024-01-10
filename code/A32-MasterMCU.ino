#include <math.h>
#include <Wire.h>
#include <stdio.h>
#include "Adafruit_TCS34725_Hshopvn.h"

//I2C ADDRESS
#define address1 9 //SET SLAVE 1 I2C ADDRESS
#define address2 10 //SET SLAVE 2 I2C ADDRESS
//#define address3 0x29 (DEFAULT COLOR SENSOR I2C ADDRESS, CANNOT CHANGE)

//Declare variables
  //Color sensor reading
Adafruit_TCS34725_Hshopvn tcs = Adafruit_TCS34725_Hshopvn(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);  

  //IR Sensor reading
int s0 = 24;
int s1 = 25;
int s2 = 26;
int s3 = 27;
int s4 = 28;
int s5 = 29;
int s6 = 30;
int ss[7];
float calib_ss1 = 0;
float calib_ss2 = 0;
float calib_ss3 = 0;
float calib_ss4 = 0;
float calib_ss5 = 0;
float calib_ss6 = 0;
float calib_ss7 = 0;
float num = 0;
float den = 0;
float distss = 17;
int t_straight =0;

  //Load and Stop point parameters
int flag = 0;

 //Controller parameters
long prevT = 0;
float u = 0, q = 0;
float eprop =0, eder = 0, ePrev =0, ePast = 0;
float eintegral = 0;
float uPrev = 0;
int speed_left=0;
int speed_right =0;
int countstraight = 0;
int countcurve = 0;
float  dt = 0.26;
// CONFIG ORIGINAL
float Kp = 233;
float Ki = 11;
float Kd = 0.8;

int color = 0;

//Declare sub function

void read_sensor();
float weightedapprox_sensor();
void sendSpeed(int speedd, int address);
void stopcar();
void color_recognition();


void setup() {
  /// put your setup code here, to run once:
  Wire.begin(); // join I2C bus as the master
  ///Begining the serial
  Serial.begin(115200);
  delay(100);
}

void loop() {
  //long currT = micros();
  read_sensor();
  float e = weightedapprox_sensor()/1000.0;
if ((calib_ss1<70)&&(calib_ss2<70)&&(calib_ss3<70)&&(calib_ss4<70)&&(calib_ss5<70)&&(calib_ss6<70)&&(calib_ss7<70) && (flag ==3)){ 
  sendSpeed(0,address1);
  sendSpeed(0,address2); 
}

else {
    if ( loadline() && flag == 0 ){
    sendSpeed(0,address1);
    sendSpeed(0,address2);
    while (color == 0){
      color_recognition();
    }
      flag = 1;
      delay(2000);
      t_straight = millis()+1000;  
      Ki = 15;
      Kd = 1;

    }
//    else if ( loadline() && flag == 1 && (countstraight <120)){
//      while (flag == 1){
//        speed_left = 88;
//        speed_right = 88;
//        sendSpeed(speed_right,address2); 
//        sendSpeed(speed_left,address1);     
//        countstraight = countstraight + 1;
//        if (countstraight == 119){
//          flag = 2;
//          Serial.print(flag);
//              Serial.print("\t");   
//        }
//        
//      }
//    }
    else if (millis() > t_straight && flag == 1) 
          { flag = 2;}    
    else if ( loadline() && flag == 2 && color == 1 && (countcurve <800)){
      while (flag == 2){
        speed_left = 70;
        speed_right = 120;
        sendSpeed(speed_right,address2); 
        sendSpeed(speed_left,address1);     
        countcurve = countcurve + 1;
        if (countcurve == 799){
          flag = 3;
          Serial.print(flag);
              Serial.print("\t");   
        }
      }
    }
    else if  ( loadline() && flag == 2 && color == 2 && (countcurve <800)){
      while (flag == 2){
        speed_right = 70;
        speed_left = 10;
        sendSpeed(speed_right,address2); 
        sendSpeed(speed_left,address1);     
        countcurve = countcurve + 1;
        if (countcurve == 799){
          flag = 3;
          Serial.print(flag);
              Serial.print("\t");   
        }
      }
    }
       eprop = e - ePrev;
       eintegral = e*dt;
       eder = (e - 2*ePrev + ePast)/dt;
       
    u = uPrev + Kp * eprop + Ki * eintegral + Kd * eder;
    speed_right = 88 + u;
    speed_left = 88 - u;
  uPrev = u;
  ePast = ePrev;
  ePrev = e;
       sendSpeed(speed_right,address2); 
    sendSpeed(speed_left,address1);

}
 
    Serial.print(flag);
    Serial.print("\t");   
    Serial.print(e,4);
    Serial.print("\t");  
    Serial.print(color);
    Serial.print("\t");         
//    Serial.print(speed_left);
//     Serial.print("\t");       
//    Serial.print(speed_right);  
//    Serial.print("\t");   
   // Serial.print(currT/1e6);
    Serial.println("");  
}
    



/////////////////////////////////
bool loadline (){
  if ( (calib_ss3 > 350) && (calib_ss4 > 250) && (calib_ss5 > 250) || \
       (calib_ss2 > 350) && (calib_ss3 > 250) && (calib_ss4 > 250) || \
       (calib_ss4 > 350) && (calib_ss5 > 250) && (calib_ss6 > 250) || \
       (calib_ss1 > 350) && (calib_ss2 > 250) && (calib_ss3 > 250) || \
       (calib_ss5 > 350) && (calib_ss6 > 250) && (calib_ss7 > 250) ){
    return true;
  }
  return false;
}
void read_sensor(){
  ss[0] = analogRead(s0);
  ss[1] = analogRead(s1);
  ss[2] = analogRead(s2);
  ss[3] = analogRead(s3);
  ss[4] = analogRead(s4);
  ss[5] = analogRead(s5);
  ss[6] = analogRead(s6); 

}

float weightedapprox_sensor(){
   calib_ss1 = 26 +(423.0/528.0)*(ss[0]-26);
   calib_ss2 = 26 +(423.0/401.0)*(ss[1]-26);
   calib_ss3 = 26 +(423.0/390.0)*(ss[2]-26);
   calib_ss4 = 26 +(423.0/427.0)*(ss[3]-26);
   calib_ss5 = 26 +(423.0/444.0)*(ss[4]-26);
   calib_ss6 = 26 +(423.0/323.0)*(ss[5]-26);
   calib_ss7 = 26 +(423.0/447.0)*(ss[6]-26);
   num = 3*(calib_ss7-calib_ss1)+2*(calib_ss6-calib_ss2)+(calib_ss5-calib_ss3);
    den = calib_ss1+calib_ss2+calib_ss3+calib_ss4+calib_ss5+calib_ss6+calib_ss7;

  float w_approx = distss*num/den;
  float w_error = (w_approx -0.2915)/0.7198;
  return w_error;
}
void stopcar(){
  sendSpeed(0,address1);
  sendSpeed(0,address2);    
}
void color_recognition(){
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c); 
//  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
//  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
//  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" "); 
//  Serial.println(" ");  
  float rb = float(r)/float(b);
float rg = float(r)/float(g);
float br = float(b)/float(r);
float bg = float(b)/float(g);
//  Serial.print(rb);
//  Serial.print("\t");
//  Serial.print(rg);
//  Serial.print("\t");
//    Serial.print(br);
//  Serial.print("\t");
//    Serial.print(bg);
//  Serial.print("\t");
// 
//  Serial.println(" "); 

 if ((rb > 1.23 ) && (rg > 1.25)) {
  color = 1; //red
 } 
 else if ((br > 1.4) && (bg > 1.08)) {
  color = 2; //blue
 }
 else color = 0;
}

void sendSpeed(int speedd, int address) {
  Wire.beginTransmission(address); // informs the bus that we will be sending data to slave device 8 (0x08)
	Wire.write(speedd);        // send value_pot
	Wire.endTransmission();       // informs the bus and the slave device that we have finished sending data
}
