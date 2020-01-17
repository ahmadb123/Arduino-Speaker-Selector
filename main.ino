#include "string.h"
#include "SPI.h"
#include "Ethernet.h"

#define RELAY_ON 0
#define RELAY_OFF 1


byte mac[] = {0x00, 0x1E, 0x2A, 0x77, 0x24, 0x02 };

byte ip[] = {192, 168, 0, 234 }; 

EthernetServer server(80);

boolean showStatus = false;

boolean spk1 = true;
boolean spk2 = true;
boolean spk3 = true;
boolean spk4 = true;
boolean spk5 = true;
boolean spk6 = true;
boolean spk7 = true;
boolean spk8 = true;
boolean spk9 = true;
boolean spk10 = true;

static const int spk1p = 3; 
static const int spk2p = 4; 
static const int spk3p = 5; 
static const int spk4p = 6; 
static const int spk5p = 7; 
static const int spk6p = 8; 
static const int spk7p = 9; 
static const int spk8p = 10; 
static const int spk9p = 11;
static const int spk10p = 12;

void setupPins() {
  digitalWrite(spk1p, RELAY_ON);
  digitalWrite(spk2p, RELAY_ON);
  digitalWrite(spk3p, RELAY_ON);
  digitalWrite(spk4p, RELAY_ON);
  digitalWrite(spk5p, RELAY_ON);
  digitalWrite(spk6p, RELAY_ON);
  digitalWrite(spk7p, RELAY_ON);
  digitalWrite(spk8p, RELAY_ON);
  digitalWrite(spk9p, RELAY_ON);

  pinMode(spk1p, OUTPUT);
  pinMode(spk2p, OUTPUT);
  pinMode(spk3p, OUTPUT);
  pinMode(spk4p, OUTPUT);
  pinMode(spk5p, OUTPUT);
  pinMode(spk6p, OUTPUT);
  pinMode(spk7p, OUTPUT);
  pinMode(spk8p, OUTPUT);
  pinMode(spk9p, OUTPUT);
  delay(4000);
}

void setup()
{
   Ethernet.begin( mac, ip );
    server.begin();
    setupPins();
}

static const int bufferMax = 128;
int bufferSize;
char buffer[ bufferMax ];

void loop() {
    EthernetClient client = server.available();
    if ( client ) {
        waitForRequest( client );
        handleRequest();
        client.stop();
    }
}

void waitForRequest( EthernetClient client ) 
{
    bufferSize = 0;

    while ( client.connected() ) {
        if ( client.available() ) {
            char c = client.read();
            if ( c == '\n' ) {
                break;
            }
            else {
                if ( bufferSize < bufferMax ) {
                    buffer[ bufferSize++ ] = c;
                }
                else {
                    break;
                }
            }
        }
    }
}

void handleRequest() {
    String request = String(buffer);
    int firstSpace = request.indexOf(" ");   // right after GET
    int lastSpace = request.indexOf(" ", firstSpace + 1); // just after the query string
    request = request.substring(firstSpace, lastSpace);
    
    showStatus = request.indexOf("?") == -1;
    spk1 = request.indexOf("1") > -1;
    spk2 = request.indexOf("2") > -1;
    spk3 = request.indexOf("3") > -1;
    spk4 = request.indexOf("4") > -1;
    spk5 = request.indexOf("5") > -1;
    spk6 = request.indexOf("6") > -1;
    spk7 = request.indexOf("7") > -1;
    spk8 = request.indexOf("8") > -1;
    spk9 = request.indexOf("9") > -1;
    spk10 = request.indexOf("10") > -1;
  

    if (request.indexOf("1=on") > -1) {
      spk1 = true;
    }
    else if (request.indexOf("1=off") > -1) {
      spk1 = false;
    }
    if (request.indexOf("2=on") > -1) {
      spk2 = true;
    }
    else if (request.indexOf("2=off") > -1) {
      spk2 = false;
    }
    if (request.indexOf("3=on") > -1) {
      spk3 = true;
    }
    else if (request.indexOf("3=off") > -1) {
      spk3 = false;
    }
    if (request.indexOf("4=on") > -1) {
      spk4 = true;
    }
    else if (request.indexOf("4=off") > -1) {
      spk4 = false;
    }
    if (request.indexOf("5=on") > -1) {
      spk5 = true;
    }
    else if (request.indexOf("5=off") > -1) {
      spk5 = false;
    }
    if (request.indexOf("6=on") > -1) {
      spk6 = true;
    }
    else if (request.indexOf("6=off") > -1) {
      spk6 = false;
    }
    if (request.indexOf("7=on") > -1) {
      spk7 = true;
    }
    else if (request.indexOf("7=off") > -1) {
      spk7 = false;
    }
    if (request.indexOf("8=on") > -1) {
      spk8 = true;
    }
    else if (request.indexOf("8=off") > -1) {
      spk8 = false;
    }
      if (request.indexOf("10=on") > -1) {
      spk10 = true;
    }
    else if (request.indexOf("9=off") > -1) {
      spk10 = false;
    }
    if (request.indexOf("9=on") > -1) {
      spk9 = true;
    }
    else if (request.indexOf("9=off") > -1) {
      spk9 = false;
    }

    if (!showStatus) {
        setSpeakerState();
    }
    sendPage();
}
void setSpeakerState() {
    digitalWrite(spk1p, spk1 ? RELAY_ON : RELAY_OFF);
    digitalWrite(spk2p, spk2 ? RELAY_ON : RELAY_OFF);
    digitalWrite(spk3p, spk3 ? RELAY_ON : RELAY_OFF);
    digitalWrite(spk4p, spk4 ? RELAY_ON : RELAY_OFF);
    digitalWrite(spk5p, spk5 ? RELAY_ON : RELAY_OFF);
    digitalWrite(spk6p, spk6 ? RELAY_ON : RELAY_OFF);
    digitalWrite(spk7p, spk7 ? RELAY_ON : RELAY_OFF);
    digitalWrite(spk8p, spk8 ? RELAY_ON : RELAY_OFF);
    digitalWrite(spk10p, spk10 ? RELAY_ON : RELAY_OFF);
    digitalWrite(spk9p, spk9 ? RELAY_ON : RELAY_OFF);
}

void sendPage() {
    printHead();
    printForm();
    printTail();
}
void printHead() {
    server.print("<html>\n<head>\n<title>Speaker Control</title>\n");
    server.print("<meta name='viewport' content='width=device-width, initial-scale=1'>\n");
    server.print("<link rel='stylesheet' href='http://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap.min.css'>\n");
    server.print("</head>\n<body>\n"); 
}

void printForm() {
   server.print("<div class='container'>\n");
   server.print("<h2>Speaker Control</h2><br/>\n");
   server.print("<form action='/' role='form'>\n");
   
   int pinState = digitalRead(spk1p);                                           
   server.print("<div class='checkbox'><label><input type='checkbox' name='1'");
   server.print(pinState == RELAY_ON ? " checked='checked'" : "");
   server.print(">Hallways</label></div><br/>\n");

   pinState = digitalRead(spk2p);                                           
   server.print("<div class='checkbox'><label><input type='checkbox' name='2'");
   server.print(pinState == RELAY_ON ? " checked='checked'" : "");               
   server.print(">ex1</label></div><br/>\n");       
   
   pinState = digitalRead(spk3p);                                           
   server.print("<div class='checkbox'><label><input type='checkbox' name='3'");
   server.print(pinState == RELAY_ON ? " checked='checked'" : "");               
   server.print(">ex2</label></div><br/>\n");     
   
   pinState = digitalRead(spk4p);                                           
   server.print("<div class='checkbox'><label><input type='checkbox' name='4'");
   server.print(pinState == RELAY_ON ? " checked='checked'" : "");               
   server.print(">ex3</label></div><br/>\n");     
   
   pinState = digitalRead(spk5p);                                           
   server.print("<div class='checkbox'><label><input type='checkbox' name='5'");
   server.print(pinState == RELAY_ON ? " checked='checked'" : "");               
   server.print(">ex4</label></div><br/>\n");     
   
   pinState = digitalRead(spk6p);                                           
   server.print("<div class='checkbox'><label><input type='checkbox' name='6'");
   server.print(pinState == RELAY_ON ? " checked='checked'" : "");               
   server.print(">ex5</label></div><br/>\n");     
   
   pinState = digitalRead(spk7p);                                           
   server.print("<div class='checkbox'><label><input type='checkbox' name='7'");
   server.print(pinState == RELAY_ON ? " checked='checked'" : "");               
   server.print(">ex6</label></div><br/>\n");     
   
   pinState = digitalRead(spk9p);                                           
   server.print("<div class='checkbox'><label><input type='checkbox' name='9'");
   server.print(pinState == RELAY_ON ? " checked='checked'" : "");               
   server.print(">ex7</label></div><br/>\n");    

   pinState = digitalRead(spk10p);                                           
   server.print("<div class='checkbox'><label><input type='checkbox' name='10'");
   server.print(pinState == RELAY_ON ? " checked='checked'" : "");               
   server.print(">ex8</label></div><br/>\n");        
   
   pinState = digitalRead(spk8p);                                           
   server.print("<div class='checkbox'><label><input type='checkbox' name='8'");
   server.print(pinState == RELAY_ON ? " checked='checked'" : "");               
   server.print(">ex9</label></div><br/>\n");                          
                                                                                                                                        
   server.print("<br/>\n");
   server.print("<button type='submit' class='btn btn-default'>Save</button>\n");
   server.print("</form>\n");
   
   server.print("</div>\n");
}

void printTail() {
   server.print("</body>\n</html>\n"); 
}



