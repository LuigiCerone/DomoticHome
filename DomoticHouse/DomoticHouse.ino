/*
# Arduino code for creating a smart house where user could control stuff by using HTTP requests (through web pages or Android app).
# Author: Cerone Luigi luigiceroneaq@gmail.com
# License: GNU GPL.
*/


#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
#include <DHT.h>
#include <Servo.h>
#include <LiquidCrystal.h>
 
#define maxLength 25
 
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress ip (192,168,0,254 );
IPAddress myServer(192,168,0,252);
char serverName[] = "192.168.0.252";
int serverPort = 80;

#define delayMillis 30000UL
unsigned long thisMillis = 0;
unsigned long lastMillis = 0;
char pageAdd[64];

File htmlFile;

EthernetServer server(80);
EthernetClient client;

// All the domotic elements connected to the board .
#define livingRoom 22 // Living room's led.
#define fan 23 // Living room's fan.
#define myalarm 24 // House alarm.
#define garage 25  // Garage's led.
#define kitchen  26 // Kitchen's led.
#define stairs  27 // Stair's led.
#define bedroom1  40 // First bedroom's led.
#define bedroom2   41 // Second bedroom's led.
#define bathroom  30 // Bathroom's led.
#define outisde  31 // Garden lights.
#define entrance  32 // Entrance's led.
#define fountain 35 // Motor pin of the outside fountain.
#define garageDoor 36 // Garage door's led.
#define corridor 28 // Corridor's led.

#define DHTTYPE DHT11   // DHT 11

#define DHT11in_PIN 3
#define DHT11out_PIN 5

DHT DHTin(DHT11in_PIN, DHTTYPE);
DHT DHTout(DHT11out_PIN, DHTTYPE);

int intTemp = 0,
	outTemp = 0,
	intHum = 0,
	outHum = 0;

int raffredTemp=20; // Temperature for cooling the living room.

#define reed  33 // Reed sensor's pin for detecting illegal entry.
boolean alarmOn = false; // Is alarm on?
#define buzzer 34 // Buzzer's pin.
#define crepuscular A0 // Photoresistor pin for automatic external lights.

int brightness = 0; // External brightness.

Servo dx; 
Servo sx; // Two servoes for each garage door leaf.
int i,j;

// Servo motors degress positions.
int Adx[] = {140,135,130,125,120,115,110,105,100,95,90,85,80,75,70,65,60,55,50,45,40,35,35,35};   
int Asx[] = {40,40,40,45,50,55,60,65,70,75,80,85,90,95,100,115,120,125,130,135,140,145,150,155,160,165}; 
int Cdx[] = {35,35,35,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,125,130,135,140}; 
int Csx[] = {165,160,140,130,125,110,105,100,95,90,85,80,77,75,70,65,60,55,50,45,40,40,40,40,40}; 

int ledStatus[]={0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1};
int poscorrdx;
int poscorrsx;
int x;

/* LCD connection pinout.
* RS ->48
* E ->49
* D4 ->46
* D5 ->47
* D6 ->44
* D7 ->45
*/
LiquidCrystal lcd (48,49,46,47,44,45); // LCD.

void setup()
{
  Serial.begin(9600);
  Serial.println("Domotic with Arduino - Raspberry- Android");
  
  dx.attach(11); 
  dx.write(140); // (decrease for opening).
  x = 0;
  sx.attach(9); 
  sx.write(40); // (increase for opening).
  
  
  lcd.begin(16,2);
  lcd.clear();
  lcd.noCursor();
  lcd.noBlink();
  
  lcd.print("Domotic house!");
  
  // SD check.
  if(SD.begin(4) == 0)
  Serial.println("Error SD!");
  else
  Serial.println("SD OK!");
  
  
  Ethernet.begin(mac, ip); // Static IP.
  
  delay(2000);
  
  pinMode(53,OUTPUT);
  
  // Configuration stuff.
  pinMode(livingRoom,OUTPUT);
  pinMode(garage,OUTPUT);
  pinMode(kitchen,OUTPUT);
  pinMode(stairs,OUTPUT);
  pinMode(bathroom,OUTPUT);
  pinMode(outisde,OUTPUT);
  pinMode(bedroom1,OUTPUT);
  pinMode(bedroom2,OUTPUT);
  pinMode(entrance,OUTPUT);
  pinMode(fan,OUTPUT);
  pinMode(myalarm,OUTPUT);
  pinMode(fountain,OUTPUT);
  pinMode(reed,INPUT);
  
  digitalWrite(reed, HIGH);    // Pullup on.
  
  pinMode(crepuscular,INPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(garageDoor,OUTPUT);
  pinMode(corridor,OUTPUT);
  
  
  server.begin(); // Start server on port 80.
  
  // HTML pages checkings.
   if (!SD.exists("header.htm")) {
        Serial.println("ERROR - Can't find  header.htm !");
        return;
    }
   if (!SD.exists("footer.htm")) {
        Serial.println("ERROR - Can't find  footer.htm !");
        return;
    }
   if (!SD.exists("bagn.htm")) {
        Serial.println("ERROR - Can't find  bagn.htm !");
        return;
    }
   if (!SD.exists("cam1.htm")) {
        Serial.println("ERROR - Can't find  cam1.htm !");
        return;
    }
   if (!SD.exists("cam2.htm")) { 
        Serial.println("ERROR - Can't find  cam2.htm !");
        return;
    }
   if (!SD.exists("cuci.htm")) {
        Serial.println("ERROR - Can't find  cuci.htm !");
        return; 
    }
   if (!SD.exists("este.htm")) {
        Serial.println("ERROR - Can't find  este.htm !");
        return;
    }
    if (!SD.exists("gara.htm")) {
        Serial.println("ERROR - Can't find  gara.htm !");
        return; 
    }
    if (!SD.exists("ingr.htm")) {
        Serial.println("ERROR - Can't find  ingr.htm !");
        return;
    }
    if (!SD.exists("sala.htm")) {
        Serial.println("ERROR - Can't find  sala.htm !");
        return;
    }
    if (!SD.exists("scal.htm")) {
        Serial.println("ERROR - Can't find  scal.htm !");
        return;
    }
  Serial.print("IP address for server : ");
  Serial.println(Ethernet.localIP());  // In this case should return 192.168.0.254.

}
 
void loop()
{
  char* file_to_load = "ingr.htm"; // Default loaded page.
  String inString = String(maxLength); // Request received content.
  thisMillis = millis(); // Timer.
  
  if(thisMillis - lastMillis > delayMillis) // If more than 30s have passed refresh data on local server.
  {
    lastMillis = thisMillis;
    // void readTemp(DHT dht, int dht_pin,int temp, int umid)
     DHTin = readTemp(DHTin,DHT11in_PIN);
     DHTout = readTemp(DHTout,DHT11out_PIN);
     
    sprintf(pageAdd,"/temperature.php?tempI=%u&tempE=%u&umidI=%u&umidE=%u",DHTin.readTemperature(),DHTout.readTemperature(),DHTin.readHumidity(),DHTin.readHumidity());
    //tempInterna,tempEsterna,umidInterna,umidEsterna
    Serial.println(pageAdd);
    // sprintf(pageAdd,"/arduino.php?test=%u",totalCount);

    if(!getPage(myServer,serverPort,pageAdd)) Serial.print(F("Fail "));
  }    
  
  EthernetClient client = server.available();
  if (client) {
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (inString.length() < maxLength) {
          inString += c;
        }
       
        if (c == '\n' && currentLineIsBlank) { 
          if (inString.indexOf(".htm") > -1) { // In the request there is a .htm? Then parse the page to load.
            String new_file_load;
            int rootIndex = inString.indexOf("/");
            new_file_load = inString.substring((rootIndex+1), (rootIndex+13));
 
            int endIndex = new_file_load.indexOf(" ");
            if (endIndex > -1) {
              new_file_load = new_file_load.substring(0, endIndex);
            }
            
            int commIndex = new_file_load.indexOf("?"); 
            if (commIndex > -1) {
              new_file_load = new_file_load.substring(0, commIndex);
              Serial.println("File da caricare:"+new_file_load);
            }
            if (new_file_load != "")  {
              new_file_load.toCharArray(file_to_load,12);
            }
          } // The name of the page is in file_to_load. 
   
          client.println("HTTP/1.1 200 OK");
          
          //If the request is  http: //192.168.0.252/?domotica then Arduino has to send a XML file.
          if (inString.indexOf("?domotica") >0){
                
                    client.println("Content-Type: text/xml");
                    client.println("Connection: keep-alive");
                    client.println();
                    Serial.println("Sono passato di qui.");
                    // Use this function for create the XML file.
                    xmlResponse(client);   
           }
           else{ // If there is not ?domotica then send a stitic html page.
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          
          if(inString.indexOf("?t")>0){ // If in the request there is a t parameter (es. http://192.168.0.252/sala.htm?c=20).
                    // Then change the default value of raffredTemp.
                    int Pos = inString.indexOf("?t");
                    String ni = inString.substring(Pos+3,Pos+5);  //transfer substring to buffer
                    raffredTemp=ni.toInt();
                    //Serial.println(temp);
             }
          // Send static HTML file.
          read_file( "header.htm",client ); // l'header.
          read_file( file_to_load,client ); // la pagina richiesta.
          read_file( "footer.htm",client ); // il footer.
           }
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          currentLineIsBlank = false;
        }
        
        readURL(inString); // Check if there are other kinds of command (i.e. Light on/off etc).
      }
    }
    delay(1);
    client.stop();
  }
  outsideLight(); // Read outside brightness.
  alarm(); // Check alarm state.
}
 
void read_file( char* page_html, EthernetClient client )
{
  htmlFile = SD.open( page_html );
  if (htmlFile) {
    while (htmlFile.available()) {
        client.write(htmlFile.read());
    }
    // close the file:
    htmlFile.close();
  }
}

void xmlResponse(EthernetClient cl)
{
    cl.print("<?xml version = \"1.0\" ?>");
    cl.print("<inputs>");
    cl.print("<button1>");
    if (digitalRead(stairs)) {
        cl.print("ON");
    }
    else {
        cl.print("OFF");
    }
    cl.print("</button1>");
    cl.print("<button2>");
    if (digitalRead(bathroom)) {
        cl.print("ON");
    }
    else {
        cl.print("OFF");
    }
    cl.print("</button2>");
    cl.print("<button3>");
    if (digitalRead(bedroom1)) {
        cl.print("ON");
    }
    else {
        cl.print("OFF");
    }
    cl.print("</button3>");
    cl.print("<button4>");
    if (brightness>950) {
        cl.print("ON");
    }
    else {
        cl.print("OFF");
    }
    cl.print("</button4>");
    cl.print("<button5>");
    if (digitalRead(fountain)) {
        cl.print("ON");
    }
    else {
        cl.print("OFF");
    }
    cl.print("</button5>"); //Pompa acqua.
    cl.print("<button6>");
    if (digitalRead(livingRoom)) {
        cl.print("ON");
    }
    else {
        cl.print("OFF");
    }
    cl.print("</button6>");
    cl.print("<button7>");
    if (digitalRead(bedroom2)) {
        cl.print("ON");
    }
    else {
        cl.print("OFF");
    }
    cl.print("</button7>");
    cl.print("<button8>");
    if (digitalRead(garage)) {
        cl.print("ON");
    }
    else {
        cl.print("OFF");
    }
    cl.print("</button8>");
    cl.print("<button9>");
    if (x==1) {
        cl.print("Aperta");
    }
    else {
        cl.print("Chiusa");
    }
    cl.print("</button9>"); //Saracinesca.
    cl.print("<button10>");
    if (digitalRead(entrance)) {
        cl.print("ON");
    }
    else {
        cl.print("OFF");
    }
    cl.print("</button10>");
    cl.print("<button11>");
    if (alarmOn) {
        cl.print("ON");
    }
    else {
        cl.print("OFF");
    }
    cl.print("</button11>");//Reed.
    cl.print("<button12>");
    if (digitalRead(kitchen)) {
        cl.print("ON");
    }
    else {
        cl.print("OFF");
    }
    cl.print("</button12>");
    cl.print("<button13>");
    if (digitalRead(fan)) {
        cl.print("ON");
    }
    else {
        cl.print("OFF");
    }
    cl.print("</button13>");
    cl.print("<button14>");
    if (digitalRead(corridor)) {
        cl.print("ON");
    }
    else {
        cl.print("OFF");
    }
    cl.print("</button14>");
    
    //--------------------------//
    cl.print("<tempInt>");
    cl.print(DHTin.readTemperature());
    cl.print("</tempInt>");
    cl.print("<umidInt>");
    cl.print(DHTin.readHumidity());
    cl.print("</umidInt>");
    cl.print("<raffAttuale>");
    cl.print(raffredTemp);
    cl.print("</raffAttuale>");
    cl.print("<tempEst>");
    cl.print(DHTout.readTemperature());
    cl.print("</tempEst>");
    cl.print("<umidEst>");
    cl.print(DHTout.readHumidity());
    cl.print("</umidEst>");
    cl.print("</inputs>");
}

void readURL(String inString)
{
           if (inString.indexOf("?b1o") >0){
               digitalWrite(stairs, HIGH);
               printOnLCD(lcd,"Light stairs ON");
             }
           if (inString.indexOf("?b1f") >0){
               digitalWrite(stairs, LOW);
               printOnLCD(lcd,"Light stairs OFF");
           }
           if (inString.indexOf("?b2o") >0){ 
               digitalWrite(bathroom, HIGH);
               printOnLCD(lcd,"Light bathroom ON");
           }
           if (inString.indexOf("?b2f") >0){
               digitalWrite(bathroom, LOW);
               printOnLCD(lcd,"Light bathroom OFF");
           }
           
           if (inString.indexOf("?b3o") >0){ 
               digitalWrite(bedroom1, HIGH);
               printOnLCD(lcd,"Light bedroom1 ON");
           }
           if (inString.indexOf("?b3f") >0){
               digitalWrite(bedroom1, LOW);
               printOnLCD(lcd,"Light bedroom1 OFF");
           }
           if (inString.indexOf("?b4o") >0){ //pin 9
               analogWrite(fountain, 155);
               printOnLCD(lcd,"fountain ON");
           }
           if (inString.indexOf("?b4f") >0){
               analogWrite(fountain, 0);
               printOnLCD(lcd,"fountain OFF");
           }
           if (inString.indexOf("?b5o") >0){ //pin 8
               digitalWrite(livingRoom, HIGH);
               printOnLCD(lcd,"Luci salotto ON");
           }
           if (inString.indexOf("?b5f") >0){
               digitalWrite(livingRoom, LOW);
               printOnLCD(lcd,"Luci salotto OFF");
           }
           if (inString.indexOf("?b6o") >0){ //pin 8
               digitalWrite(bedroom2, HIGH);
               printOnLCD(lcd,"Luci bedroom2 ON");
           }
           if (inString.indexOf("?b6f") >0){
               digitalWrite(bedroom2 ,LOW);
               printOnLCD(lcd,"Luci bedroom2 OFF");
           }
           if (inString.indexOf("?b7o") >0){ //pin 8
               digitalWrite(garage, HIGH);
               printOnLCD(lcd,"Light garage ON");
           }
           if (inString.indexOf("?b7f") >0){
               digitalWrite(garage, LOW);
               printOnLCD(lcd,"Light garage OFF");
           }
          if (inString.indexOf("?b8o") >0){  
            printOnLCD(lcd,"Garage aperto");
            garageOpen();
            
           }
           if (inString.indexOf("?b8f") >0){ 
             printOnLCD(lcd,"Garage chiuso");
             garageClose();
           } 
           if (inString.indexOf("?b9o") >0){ //pin 8
               digitalWrite(entrance, HIGH);
               printOnLCD(lcd,"Luci entrance ON");
           }
           if (inString.indexOf("?b9f") >0){
               digitalWrite(entrance, LOW);
               printOnLCD(lcd,"Luci entrance OFF");
           }
           if (inString.indexOf("?b10o") >0){ //pin 8
               alarmOn = true;
               printOnLCD(lcd,"alarm ON");
           }
           if (inString.indexOf("?b10f") >0){
               alarmOn = false;
               printOnLCD(lcd,"alarm OFF");
           } // alarm
           if (inString.indexOf("?b11o") >0){ //pin 8
               digitalWrite(kitchen, HIGH);
               printOnLCD(lcd,"Light kitchen ON");
           }
           if (inString.indexOf("?b11f") >0){
               digitalWrite(kitchen, LOW);
               printOnLCD(lcd,"Light kitchen OFF");
           }
           if (inString.indexOf("?b12o") >0){ //pin 8
               digitalWrite(fan, HIGH);
               printOnLCD(lcd,"fan ON");
           }
           if (inString.indexOf("?b12f") >0){
               digitalWrite(fan, LOW);
               printOnLCD(lcd,"fan OFF");
           }
           if (inString.indexOf("?b13o") >0){ //pin 8
               digitalWrite(corridor, HIGH);
               printOnLCD(lcd,"Light corridor ON");
           }
           if (inString.indexOf("?b13f") >0){
               digitalWrite(corridor, LOW);
               printOnLCD(lcd,"Light corridor OFF");
           }
           
}

DHT readTemp(DHT dht, int dht_pin)
{
  // DISPLAY DATA
  Serial.print(dht.readHumidity(),1);
  Serial.print(",\n");
  Serial.println(dht.readTemperature(),1);
  return dht;
}


byte getPage(IPAddress ipBuf,int thisPort, char *page)
{
  int inChar;
  char outBuf[128];

  Serial.print(F("connecting..."));

  if(client.connect(ipBuf,thisPort) == 1)
  {
    Serial.println(F("connected"));

    sprintf(outBuf,"GET %s HTTP/1.1",page);
    client.println(outBuf);
    sprintf(outBuf,"Host: %s",serverName);
    client.println(outBuf);
    client.println(F("Connection: close\r\n"));
  } 
  else
  {
    Serial.println(F("failed"));
    return 0;
  }

  // connectLoop controls the hardware fail timeout
  int connectLoop = 0;

  while(client.connected())
  {
    while(client.available())
    {
      inChar = client.read();
      Serial.write(inChar);
      // set connectLoop to zero if a packet arrives
      connectLoop = 0;
    }

    connectLoop++;

    // if more than 10000 milliseconds since the last packet
    if(connectLoop > 3000)
    {
      // then close the connection from this end.
      Serial.println();
      Serial.println(F("Timeout"));
      client.stop();
    }
    // this is a delay for the connectLoop timing
    delay(1);
  }

  Serial.println();

  Serial.println(F("disconnecting."));
  // close client end
  client.stop();

  return 1;
}

void outsideLight()
{
  brightness = analogRead(crepuscular);
  //Serial.println(brightness);
  if (brightness>950)
    digitalWrite(outisde,HIGH);
  else
    digitalWrite(outisde,LOW);
}

void garageOpen()
{
  if (x==0){
  blink(garageDoor);
  for(i=0;i<24;i++)
  {
    poscorrdx = Adx[i];
    poscorrsx = Asx[i];
    
    dx.write(poscorrdx);
    sx.write(poscorrsx);
    delay(150);
    digitalWrite(garageDoor,ledStatus[i]);
    Serial.print("Indice");
    Serial.print(i);
    Serial.print(" DX:");
    Serial.print(poscorrdx);
    Serial.print(" SX:");
    Serial.println(poscorrsx);
  }
  blink(garageDoor);
  delay(450);
  digitalWrite(garageDoor,0);
  x=1;}
}

void  garageClose()
{
  if (x==1){
  blink(garageDoor);
  for(j=0;j<25;j++)
  {
    poscorrdx = Cdx[j];
    poscorrsx = Csx[j];
    
    dx.write(poscorrdx);
    sx.write(poscorrsx);
    delay(150);
    digitalWrite(garageDoor,ledStatus[j]);
    Serial.print("Indice");
    Serial.print(j);
    Serial.print(" DX:");
    Serial.print(poscorrdx);
    Serial.print(" SX:");
    Serial.println(poscorrsx);
     }
     blink(garageDoor);
     delay(450);
    digitalWrite(garageDoor,0);
    x=0;}
}  
void blink(int led)
{
  delay(450);
  digitalWrite(led,0);
  delay(450);
  digitalWrite(led,1);
  delay(450);
  digitalWrite(led,0);
  delay(450);
  digitalWrite(led,1);
}

void alarm()
{
  if(alarmOn == true ){
      if(digitalRead(reed) == 1)
      {
      tone(buzzer,3622);
      
      delay(300);
      tone(buzzer,3622);
      blink(alarm);
      
      noTone(buzzer);
      digitalWrite(alarm,LOW);
      }
  }
}

void printOnLCD(LiquidCrystal lcd, String mess)
{
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print(mess);
}
