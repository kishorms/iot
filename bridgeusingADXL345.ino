
#include <math.h>
#include <Wire.h> // I2C library, gyroscope
// Accelerometer ADXL345
#define ACC (0xA7>>1)    //ADXL345 ACC address
#define A_TO_READ (6)        //num of bytes we are going to read each time (two bytes for each axis)

// Sensor pins
#define sensorPower 7
#define sensorPin A0

// Value for storing water level
int water_val = 0;


/////----- var for acc
int Bstatus=0;
int hx,hy,hz;





// the setup routine runs once when you press reset:
void setup() {
// Set D7 as an OUTPUT
  pinMode(sensorPower, OUTPUT);
  
  // Set to LOW so no power flows through the sensor
  digitalWrite(sensorPower, LOW);

  
  // initialize serial communication at 9600 bits per second:
 // for Accelerometer..
Serial.begin(9600);
// acclerometer
Wire.begin();
initAcc();
}

// the loop routine runs over and over again forever:
void loop() {

int level = readSensor();

  
  Acci_detect();
  //Serial.println("[detection : 48, water_lvl = 20, status:flase]");
  String result = "[x="+String(hx)+",y="+String(hy)+",z="+String(hz)+", water_lvl = "+String(level)+", status:"+String(Bstatus)+"]";
     // Serial.println("[x="+hx+",y="+hy+",z="+hz+", water_lvl = 20, status:"+Bstatus+"]");
     Serial.println(result);
  delay(1000);        // delay in between reads for stability
}


//----------water level
/*
s---->A0
vcc----> D7
GND---> GND
*/
//This is a function used to get the reading
int readSensor() {
  digitalWrite(sensorPower, HIGH);  // Turn the sensor ON
  delay(10);              // wait 10 milliseconds
  water_val = analogRead(sensorPin);    // Read the analog value form sensor
  digitalWrite(sensorPower, LOW);   // Turn the sensor OFF
  return water_val;             // send current reading
}


//---------------------------
/*
 * Accelerometer
 */

void initAcc() {
//Turning on the ADXL345
writeTo(ACC, 0x2D, 1<<3);     
writeTo(ACC, 0x31, 0x0B);
writeTo(ACC, 0x2C, 0x09);
//by default the device is in +-2g range reading
}

void getAccelerometerData(int * result) {
int regAddress = 0x32;    //first axis-acceleration-data register on the ADXL345
byte buff[A_TO_READ];
readFrom(ACC, regAddress, A_TO_READ, buff); //read the acceleration data from the ADXL345
//each axis reading comes in 10 bit resolution, ie 2 bytes.  Least Significat Byte first!!
//thus we are converting both bytes in to one int
result[0] = (((int)buff[1]) << 8) | buff[0];  
result[1] = (((int)buff[3])<< 8) | buff[2];
result[2] = (((int)buff[5]) << 8) | buff[4];
}


void Acci_detect()
{

/* Doc By MAC+tech
 *  // Accident Setect by Accelerometer
**********************************
interface:

Ardiuno         ADXL345
GND             GND
3.3v            VCC
3.3v            CS
A4              SDA
A5              SCL


**********************************
*/

int acc[3];
getAccelerometerData(acc);
hx = acc[0];
hy = acc[1];
hz = acc[2];
Serial.print(" X=");
Serial.print(hx);
Serial.print(" Y=");
Serial.print(hy);
Serial.print(" Z=");
Serial.println(hz);

if((hx>150) || (hy>150) || (hx<-150)||(hy<-150))
{
  //Serial.println("Accident Detected.");
    Bstatus=1;  // set status =1 
  //call to send sms
delay(3000);
 if(Bstatus == '1')            //Checks whether value of data is equal to 1
    {
      digitalWrite(13, HIGH);  //If value is 1 then LED turns ON
     // Acci_detect();           // to Detect Continue Accident....
   //  send_sms();
    }
 
}
else
{
  Bstatus=0;
}

delay(1000);
}

//---------------- Functions
//Writes val to address register on ACC
void writeTo(int DEVICE, byte address, byte val) {
 Wire.beginTransmission(DEVICE); //start transmission to ACC
 Wire.write(address);        // send register address
 Wire.write(val);        // send value to write
 Wire.endTransmission(); //end transmission
}
//reads num bytes starting from address register on ACC in to buff array
void readFrom(int DEVICE, byte address, int num, byte buff[]) {
Wire.beginTransmission(DEVICE); //start transmission to ACC
Wire.write(address);        //sends address to read from
Wire.endTransmission(); //end transmission

Wire.beginTransmission(DEVICE); //start transmission to ACC
Wire.requestFrom(DEVICE, num);    // request 6 bytes from ACC
int i = 0;
while(Wire.available())    //ACC may send less than requested (abnormal)
{
  buff[i] = Wire.read(); // receive a byte
  i++;
}
Wire.endTransmission(); //end transmission

}
