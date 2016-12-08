/*
# Arduino code for creating a smart house where user could control stuff by using HTTP requests (through web pages or Android app).
# Author: Cerone Luigi luigiceroneaq@gmail.com
# License: GNU GPL.
*/


#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
#include <dht11.h>
#include <Servo.h>
#include <LiquidCrystal.h>
 
#define maxLength 25
 
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress ip (192,168,0,254 );
IPAddress mioServer(192,168,0,252);
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
#define sala 22 // Living room's led.
#define ventole 23 // Living room's fan.
#define allarme 24 // House alarm.
#define garage 25  // Garage's led.
#define cucina  26 // Kitchen's led.
#define scale  27 // Stair's led.
#define camera1  40 // First bedroom's led.
#define camera2   41 // Second bedroom's led.
#define bagno  30 // Bathroom's led.
#define esterno  31 // Garden lights.
#define ingresso  32 // Entrance's led.
#define fontana 35 // Motor pin of the outside fountain.
#define portaGarage 36 // Garage door's led.
#define corridoio 28 // Corridor's led.
dht11 DHTInterno; // Outside
dht11 DHTEsterno; // Inside.

#define DHT11Interno_PIN 3
#define DHT11Esterno_PIN 5


int raffredTemp=20; // Temperature for cooling the living room.

#define reed  33 // Reed sensor's pin for detecting illegal entry.
boolean allarmeAttivo = false; // Is alarm on?
#define cigalino 34 // Buzzer's pin.
#define crepuscolare A0 // Photoresistor pin for automati external lights.

int luminosi = 0; // External brightness.

Servo dx; 
Servo sx; // Two servoes for each garage door leaf.
int i,j;

// Servo motors degress positions.
int Adx[] = {140,135,130,125,120,115,110,105,100,95,90,85,80,75,70,65,60,55,50,45,40,35,35,35};   
int Asx[] = {40,40,40,45,50,55,60,65,70,75,80,85,90,95,100,115,120,125,130,135,140,145,150,155,160,165}; 
int Cdx[] = {35,35,35,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,125,130,135,140}; 
int Csx[] = {165,160,140,130,125,110,105,100,95,90,85,80,77,75,70,65,60,55,50,45,40,40,40,40,40}; 

int StatoLed[]={0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1};
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
  pinMode(sala,OUTPUT);
  pinMode(garage,OUTPUT);
  pinMode(cucina,OUTPUT);
  pinMode(scale,OUTPUT);
  pinMode(bagno,OUTPUT);
  pinMode(esterno,OUTPUT);
  pinMode(camera1,OUTPUT);
  pinMode(camera2,OUTPUT);
  pinMode(ingresso,OUTPUT);
  pinMode(ventole,OUTPUT);
  pinMode(allarme,OUTPUT);
  pinMode(fontana,OUTPUT);
  pinMode(reed,INPUT);
  
  digitalWrite(reed, HIGH);    // Pullup on.
  
  pinMode(crepuscolare,INPUT);
  pinMode(cigalino,OUTPUT);
  pinMode(portaGarage,OUTPUT);
  pinMode(corridoio,OUTPUT);
  
  
  server.begin(); // Start server on port 80.
  
  // HTML pages checkings.
   if (!SD.exists("header.htm")) {
        Serial.println("ERRORE - Non ho trovato  header.htm !");
        return;
    }
   if (!SD.exists("footer.htm")) {
        Serial.println("ERRORE - Non ho trovato  footer.htm !");
        return;
    }
   if (!SD.exists("bagn.htm")) {
        Serial.println("ERRORE - Non ho trovato  bagn.htm !");
        return;
    }
   if (!SD.exists("cam1.htm")) {
        Serial.println("ERRORE - Non ho trovato  cam1.htm !");
        return;
    }
   if (!SD.exists("cam2.htm")) { 
        Serial.println("ERRORE - Non ho trovato  cam2.htm !");
        return;
    }
   if (!SD.exists("cuci.htm")) {
        Serial.println("ERRORE - Non ho trovato  cuci.htm !");
        return; 
    }
   if (!SD.exists("este.htm")) {
        Serial.println("ERRORE - Non ho trovato  este.htm !");
        return;
    }
    if (!SD.exists("gara.htm")) {
        Serial.println("ERRORE - Non ho trovato  gara.htm !");
        return; 
    }
    if (!SD.exists("ingr.htm")) {
        Serial.println("ERRORE - Non ho trovato  ingr.htm !");
        return;
    }
    if (!SD.exists("sala.htm")) {
        Serial.println("ERRORE - Non ho trovato  sala.htm !");
        return;
    }
    if (!SD.exists("scal.htm")) {
        Serial.println("ERRORE - Non ho trovato  scal.htm !");
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
    // void leggiTemp(dht11 dht, int dht_pin,int temp, int umid)
     DHTInterno = leggiTemp(DHTInterno,DHT11Interno_PIN,tempInterna,umidInterna);
     DHTEsterno = leggiTemp(DHTEsterno,DHT11Esterno_PIN,tempEsterna,umidEsterna);
     
    sprintf(pageAdd,"/temperature.php?tempI=%u&tempE=%u&umidI=%u&umidE=%u",DHTInterno.temperature,DHTEsterno.temperature,DHTInterno.humidity,DHTInterno.humidity);
    //tempInterna,tempEsterna,umidInterna,umidEsterna
    Serial.println(pageAdd);
    // sprintf(pageAdd,"/arduino.php?test=%u",totalCount);

    if(!getPage(mioServer,serverPort,pageAdd)) Serial.print(F("Fail "));
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
        
        leggiURL(inString); // Check if there are other kinds of command (i.e. Light on/off etc).
      }
    }
    delay(1);
    client.stop();
  }
  luciEsterne(); // Read outside brightness.
  allarmeIngresso(); // Check alarm state.
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
    if (digitalRead(scale)) {
        cl.print("ON");
    }
    else {
        cl.print("OFF");
    }
    cl.print("</button1>");
    cl.print("<button2>");
    if (digitalRead(bagno)) {
        cl.print("ON");
    }
    else {
        cl.print("OFF");
    }
    cl.print("</button2>");
    cl.print("<button3>");
    if (digitalRead(camera1)) {
        cl.print("ON");
    }
    else {
        cl.print("OFF");
    }
    cl.print("</button3>");
    cl.print("<button4>");
    if (luminosi>950) {
        cl.print("ON");
    }
    else {
        cl.print("OFF");
    }
    cl.print("</button4>");
    cl.print("<button5>");
    if (digitalRead(fontana)) {
        cl.print("ON");
    }
    else {
        cl.print("OFF");
    }
    cl.print("</button5>"); //Pompa acqua.
    cl.print("<button6>");
    if (digitalRead(sala)) {
        cl.print("ON");
    }
    else {
        cl.print("OFF");
    }
    cl.print("</button6>");
    cl.print("<button7>");
    if (digitalRead(camera2)) {
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
    if (digitalRead(ingresso)) {
        cl.print("ON");
    }
    else {
        cl.print("OFF");
    }
    cl.print("</button10>");
    cl.print("<button11>");
    if (allarmeAttivo) {
        cl.print("ON");
    }
    else {
        cl.print("OFF");
    }
    cl.print("</button11>");//Reed.
    cl.print("<button12>");
    if (digitalRead(cucina)) {
        cl.print("ON");
    }
    else {
        cl.print("OFF");
    }
    cl.print("</button12>");
    cl.print("<button13>");
    if (digitalRead(ventole)) {
        cl.print("ON");
    }
    else {
        cl.print("OFF");
    }
    cl.print("</button13>");
    cl.print("<button14>");
    if (digitalRead(corridoio)) {
        cl.print("ON");
    }
    else {
        cl.print("OFF");
    }
    cl.print("</button14>");
    
    //--------------------------//
    cl.print("<tempInt>");
    cl.print(DHTInterno.temperature);//Temperatura Intera.
    cl.print("</tempInt>");
    cl.print("<umidInt>");
    cl.print(DHTInterno.humidity);//Umidità interna.
    cl.print("</umidInt>");
    cl.print("<raffAttuale>");
    cl.print(raffredTemp); //Temperatura raffreddamento attuale.
    cl.print("</raffAttuale>");
    cl.print("<tempEst>");
    cl.print(DHTEsterno.temperature);//Temperatura esterna.
    cl.print("</tempEst>");
    cl.print("<umidEst>");
    cl.print(DHTEsterno.humidity);//Umidità esterna.
    cl.print("</umidEst>");
    cl.print("</inputs>");
}

void leggiURL(String inString)
{
           if (inString.indexOf("?b1o") >0){
               digitalWrite(scale, HIGH);
               scriviSuLCD(lcd,"Luce scala ON");
             }
           if (inString.indexOf("?b1f") >0){
               digitalWrite(scale, LOW);
               scriviSuLCD(lcd,"Luce scala OFF");
           }
           if (inString.indexOf("?b2o") >0){ 
               digitalWrite(bagno, HIGH);
               scriviSuLCD(lcd,"Luce bagno ON");
           }
           if (inString.indexOf("?b2f") >0){
               digitalWrite(bagno, LOW);
               scriviSuLCD(lcd,"Luce bagno OFF");
           }
           
           if (inString.indexOf("?b3o") >0){ 
               digitalWrite(camera1, HIGH);
               scriviSuLCD(lcd,"Luce camera1 ON");
           }
           if (inString.indexOf("?b3f") >0){
               digitalWrite(camera1, LOW);
               scriviSuLCD(lcd,"Luce camera1 OFF");
           }
           if (inString.indexOf("?b4o") >0){ //pin 9
               analogWrite(fontana, 155);
               scriviSuLCD(lcd,"Fontana ON");
           }
           if (inString.indexOf("?b4f") >0){
               analogWrite(fontana, 0);
               scriviSuLCD(lcd,"Fontana OFF");
           }
           if (inString.indexOf("?b5o") >0){ //pin 8
               digitalWrite(sala, HIGH);
               scriviSuLCD(lcd,"Luci salotto ON");
           }
           if (inString.indexOf("?b5f") >0){
               digitalWrite(sala, LOW);
               scriviSuLCD(lcd,"Luci salotto OFF");
           }
           if (inString.indexOf("?b6o") >0){ //pin 8
               digitalWrite(camera2, HIGH);
               scriviSuLCD(lcd,"Luci camera2 ON");
           }
           if (inString.indexOf("?b6f") >0){
               digitalWrite(camera2 ,LOW);
               scriviSuLCD(lcd,"Luci camera2 OFF");
           }
           if (inString.indexOf("?b7o") >0){ //pin 8
               digitalWrite(garage, HIGH);
               scriviSuLCD(lcd,"Luce garage ON");
           }
           if (inString.indexOf("?b7f") >0){
               digitalWrite(garage, LOW);
               scriviSuLCD(lcd,"Luce garage OFF");
           }
          if (inString.indexOf("?b8o") >0){  
            scriviSuLCD(lcd,"Garage aperto");
            aperturaGarage();
            
           }
           if (inString.indexOf("?b8f") >0){ 
             scriviSuLCD(lcd,"Garage chiuso");
             chiusuraGarage();
           } 
           if (inString.indexOf("?b9o") >0){ //pin 8
               digitalWrite(ingresso, HIGH);
               scriviSuLCD(lcd,"Luci ingresso ON");
           }
           if (inString.indexOf("?b9f") >0){
               digitalWrite(ingresso, LOW);
               scriviSuLCD(lcd,"Luci ingresso OFF");
           }
           if (inString.indexOf("?b10o") >0){ //pin 8
               allarmeAttivo = true;
               scriviSuLCD(lcd,"Allarme ON");
           }
           if (inString.indexOf("?b10f") >0){
               allarmeAttivo = false;
               scriviSuLCD(lcd,"Allarme OFF");
           } // Allarme
           if (inString.indexOf("?b11o") >0){ //pin 8
               digitalWrite(cucina, HIGH);
               scriviSuLCD(lcd,"Luce cucina ON");
           }
           if (inString.indexOf("?b11f") >0){
               digitalWrite(cucina, LOW);
               scriviSuLCD(lcd,"Luce cucina OFF");
           }
           if (inString.indexOf("?b12o") >0){ //pin 8
               digitalWrite(ventole, HIGH);
               scriviSuLCD(lcd,"Ventole ON");
           }
           if (inString.indexOf("?b12f") >0){
               digitalWrite(ventole, LOW);
               scriviSuLCD(lcd,"Ventole OFF");
           }
           if (inString.indexOf("?b13o") >0){ //pin 8
               digitalWrite(corridoio, HIGH);
               scriviSuLCD(lcd,"Luce corridoio ON");
           }
           if (inString.indexOf("?b13f") >0){
               digitalWrite(corridoio, LOW);
               scriviSuLCD(lcd,"Luce corridoio OFF");
           }
           
}

dht11 leggiTemp(dht11 dht, int dht_pin,int temp, int umid)
{
	int chk;
 	chk = dht.read(dht_pin);    // READ DATA
  	switch (chk){
    case DHTLIB_OK:  
                Serial.print("OK,\n"); 
                break;
    case DHTLIB_ERROR_CHECKSUM: 
                Serial.print("Checksum error,\n"); 
                break;
    case DHTLIB_ERROR_TIMEOUT: 
                Serial.print("Time out error,\n"); 
                break;
    default: 
                Serial.print("Unknown error,\n"); 
                break;
  }
 // DISPLAY DATA
  Serial.print(dht.humidity,1);
  Serial.print(",\n");
  Serial.println(dht.temperature,1);
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

void luciEsterne()
{
  luminosi = analogRead(crepuscolare);
  //Serial.println(luminosi);
  if (luminosi>950)
    digitalWrite(esterno,HIGH);
  else
    digitalWrite(esterno,LOW);
}

void aperturaGarage()
{
  if (x==0){
  blink(portaGarage);
  for(i=0;i<24;i++)
  {
    poscorrdx = Adx[i];
    poscorrsx = Asx[i];
    
    dx.write(poscorrdx);
    sx.write(poscorrsx);
    delay(150);
    digitalWrite(portaGarage,StatoLed[i]);
    Serial.print("Indice");
    Serial.print(i);
    Serial.print(" DX:");
    Serial.print(poscorrdx);
    Serial.print(" SX:");
    Serial.println(poscorrsx);
  }
  blink(portaGarage);
  delay(450);
  digitalWrite(portaGarage,0);
  x=1;}
}

void  chiusuraGarage()
{
  if (x==1){
  blink(portaGarage);
  for(j=0;j<25;j++)
  {
    poscorrdx = Cdx[j];
    poscorrsx = Csx[j];
    
    dx.write(poscorrdx);
    sx.write(poscorrsx);
    delay(150);
    digitalWrite(portaGarage,StatoLed[j]);
    Serial.print("Indice");
    Serial.print(j);
    Serial.print(" DX:");
    Serial.print(poscorrdx);
    Serial.print(" SX:");
    Serial.println(poscorrsx);
     }
     blink(portaGarage);
     delay(450);
    digitalWrite(portaGarage,0);
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

void allarmeIngresso()
{
  if(allarmeAttivo == true ){
      if(digitalRead(reed) == 1)
      {
      tone(cigalino,3622);
      
      delay(300);
      tone(cigalino,3622);
      blink(allarme);
      
      noTone(cigalino);
      digitalWrite(allarme,LOW);
      }
  }
}

void scriviSuLCD(LiquidCrystal lcd, String mess)
{
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print(mess);
}
