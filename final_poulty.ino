
/*
 * MAC+tech Doc
 * Project : Samart Poultry Automation/ got form etc.
 * Programmer: Kishor M Sonawane.(Mac)
 * Language : Embeded C+java(Arduino)  
 * ------------------------------------------
 * Used Sensors  : DHT 11 Sensor,fire sensor 
 * Used Actuator : 2unit Relay Module - 1.hiter(Light) 2.cooler(AC)
 * Used Interface : GSM SIM 900A
 * final done: 
 * 
 */

 /*______________________________________________________________
  * DHT Connecton
  * DHT 11    Arduino
  * VCC       +5v
  * GND       GND
  * Data      4 (Digital)                              
  * _____________________________________________________________
  * 
  * /*______________________________________________________________
  * PIR Connecton
  * PIR    Arduino
  * VCC       +5v
  * GND       GND
  * Data      2/3/4 ( out of any digital Digital)
  * _____________________________________________________________
  *
  */

  /* 
 *  
 * 
 *  
 *  Download the App : 
 *  This program is maked by MAC+tech Software Solution. 
 *_______________________________________________________________
 * bluetooth connection:-
 * Arduino Pins           Bluetooth Pins
 * RX (Pin 0)     ———->      TX
 * TX (Pin 1)     ———->      RX
 * 5V             ———->      VCC
 * GND            ———->      GND
 * ______________________________________________________________
 * 
 * Relay Module:
 * 
 * Relay Module Work On ! logic:
 *  means This Module Work after Provied low Signal
 * s
 * 
 */
//Header files:

// DHT Libraries
#include <DHT.h>
#include <SoftwareSerial.h>
/* Create object named SIM900 of the class SoftwareSerial */
SoftwareSerial SIM900(8, 7); // rx,tx

    //Constants DHT 11
#define DHTPIN 5     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11  (AM2302)
//---------------------------------------------------------------------------
 //global variables
    //PIR var
    int n_pir=3;
    int pir_input_pin[]={2,3,4};
    boolean pir_input[]={LOW,LOW,LOW}; // initialy all pir statelow
    int flag[]={0,0,0};
    //---------light OUTPUT Pins-----
    
   //-----------------------------------

// Initialize DHT sensor for normal 16mhz Arduino
  DHT dht(DHTPIN, DHTTYPE);

  //Variables for DHT 11
  int chk=0;
  float hum;  //Stores humidity value
  float temp; //Stores temperature value
  
  
  int light_pin=9;
  int fan_pin=12;
//----------------------------------------------------------------------------
    //----------------------class-Light for on light or hiter ...
class Light{
  public:
  Light()
  {
   Serial.print("Test: Light Object Creation Successfully...");
  }
  
  void onLight(int light_no)
  {
    //write code for on light 
    //digitalWrite(pin, HIGH);
    digitalWrite(light_no, LOW);
    Serial.println("Test Light NO :"+light_no);
    delay(1000);
  }
  void offLight(int light_off)
  {
    // write code for off light
     digitalWrite(light_off, HIGH);
     Serial.print("Test Light OFF :"+light_off);
  }
}; //end of Class
//----------------class end
     Light L1;       //Object of Light
//----------------------------------
//-----------------------------------------------------------------------
// Fan Class for fan or cooler or AC On Off function
class Fan
{
public :
Fan()
{
  Serial.println("Test: Fan Class Object Created...");
}
void onfan(int on_fan)
{
  // fan on if temp geter than some value....
  // digitalWrite(pin, HIGH);
   Serial.println("Fan ON...");
   digitalWrite(on_fan, LOW);
}

void offfan(int off_fan)
{
  // off fan if temp min some value ither continue to fan...
  Serial.println("Fan OFF...");
  digitalWrite(off_fan, HIGH);
  
}

};
//-------------------------
Fan F1;
//--------------------------

//-----------------------------------------------------------------------



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //
 // init_pir_light();   // call to initialize pins....
  delay(1000);
//DHT
  dht.begin();
  pinMode(fan_pin, OUTPUT);
   digitalWrite(fan_pin, HIGH);
  Serial.println("Module TEST setup Run OK ......");
//L1=new Light();

}


void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  // call dht sensor.....
  delay(1000);
  
  dht_sense();
}


//--------------DHT 11-----------------
void dht_sense()
{
    //Read data and store it to variables hum and temp
    hum = dht.readHumidity();
    temp= dht.readTemperature();
    //Print temp and humidity values to serial monitor
    Serial.println("Humidity: ");
    Serial.print(hum);
    Serial.println(" %, Temp: ");
    Serial.print(temp);
    Serial.print(" Celsius");

    gsmInternet(temp,hum);
    
      if(temp>=29)  //intigret with pir
      {
          if(chk==0)
          {
            Serial.print(" FAN ON");
            L1.offLight(light_pin);
            F1.onfan(fan_pin);
            chk=1; 
          }
      }
      if(temp<15)
      {
        F1.offfan(fan_pin);
         L1.onLight(light_pin);
         chk=0;
      }
    
   
    delay(2000); //Delay 2 sec. 
}
//---------------------------------

void gsmInternet(float temp, float hum)
{
Serial.println("HTTP get method :");
  Serial.print("AT\\r\\n");
  SIM900.println("AT");  /* Check Communication */
  delay(5000);
  ShowSerialData(); /* Print response on the serial monitor */
  delay(5000);
  /* Configure bearer profile 1 */
  Serial.print("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\\r\\n");    
  SIM900.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");  /* Connection type GPRS */
  delay(5000);
  ShowSerialData();
  delay(5000);
  Serial.print("AT+SAPBR=3,1,\"APN\",\"internet\"\\r\\n");  
  SIM900.println("AT+SAPBR=3,1,\"APN\",\"internet\"");  /* APN of the provider */
  delay(5000);
  ShowSerialData();
  delay(5000);
  Serial.print("AT+SAPBR=1,1\\r\\n");
  SIM900.println("AT+SAPBR=1,1"); /* Open GPRS context */
  delay(5000);
  ShowSerialData();
  delay(5000);
  Serial.print("AT+SAPBR=2,1\\r\\n");
  SIM900.println("AT+SAPBR=2,1"); /* Query the GPRS context */
  delay(5000);
  ShowSerialData();
  delay(5000);
  Serial.print("AT+HTTPINIT\\r\\n");
  SIM900.println("AT+HTTPINIT"); /* Initialize HTTP service */
  delay(5000); 
  ShowSerialData();
  delay(5000);
  Serial.print("AT+HTTPPARA=\"CID\",1\\r\\n");
  SIM900.println("AT+HTTPPARA=\"CID\",1");  /* Set parameters for HTTP session */
  delay(5000);
  ShowSerialData();

/*
   * https://smartpoultry1.000webhostapp.com/api/pushapi.php?user_id=1&temp=15.2&humidity=10.2&utm1=null&utm2=null&utm3=null&utm4=null
   */
  
  delay(5000);
  Serial.print("AT+HTTPPARA=\"URL\",\"smartpoultry1.000webhostapp.com/api/pushapi.php?user_id=1&temp=30.99&humidity=33.99&utm1=null&utm2=null&utm3=null&utm4=null\"\\r\\n");
  SIM900.println("AT+HTTPPARA=\"URL\",\"smartpoultry1.000webhostapp.com/api/pushapi.php?user_id=1&temp="+ (String)temp +"&humidity="+ (String)hum +"&utm1=null&utm2=null&utm3=null&utm4=null\"");  /* Set parameters for HTTP session */
  delay(5000);
  ShowSerialData();
  delay(5000);
  Serial.print("AT+HTTPACTION=0\\r\\n");
  SIM900.println("AT+HTTPACTION=0");  /* Start GET session */
  delay(10000);
  ShowSerialData();
  delay(10000);
  Serial.print("AT+HTTPREAD\\r\\n");
  SIM900.println("AT+HTTPREAD");  /* Read data from HTTP server */
  delay(8000);
  ShowSerialData();
  delay(8000);
  Serial.print("AT+HTTPTERM\\r\\n");  
  SIM900.println("AT+HTTPTERM");  /* Terminate HTTP service */
  delay(5000);
  ShowSerialData();
  delay(5000);
  Serial.print("AT+SAPBR=0,1\\r\\n");
  SIM900.println("AT+SAPBR=0,1"); /* Close GPRS context */
  delay(5000);
  ShowSerialData();
  delay(5000);
}

void ShowSerialData()
{
  while(SIM900.available()!=0)  /* If data is available on serial port */
  Serial.write(char (SIM900.read())); /* Print character received on to the serial monitor */
}
