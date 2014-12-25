/* Minimal Arduino Web Api
   Based on Webduino library https://github.com/sirleech/Webduino

   I'm using this script with Arduino Mega 2560 connected with Ethernet Shield on W5100 chip.

   Vasily Klenov, 2014
*/

#include "SPI.h"
#include "Ethernet.h"
#include "WebServer.h"

#define DEBUG_WEB_API 0

#define VERSION_STRING "0.0.6"

#define CRLF "\r\n"

#define SIZE(array) (sizeof(array) / sizeof(*array))

// Change this to your own unique value
static uint8_t mac[] = { 0xA9, 0x10, 0x14, 0xE7, 0xAC, 0xCC };
// Change this to match your host network
const IPAddress ip(192, 168, 2, 15);


#define PREFIX ""
WebServer webserver(PREFIX, 80);

#define PIN_ON  0
#define PIN_OFF 1

// digital pins 10, 11, 12, and 13 on the Uno and pins 50, 51, and 52 on the Mega occupied by Ethernet Shield
const int  input_pins[] = {      40,      42,      43,      44,      45,      46,      47 };
int input_pins_states[] = { PIN_OFF, PIN_OFF, PIN_OFF, PIN_OFF, PIN_OFF, PIN_OFF, PIN_OFF };

char* pin_number_as_char = "XX";

/* store the HTML in program memory using the P macro */
P(On)      = "<status>ON</status>";
P(Off)     = "<status>OFF</status>";
P(Hello) = 
    "<html>"
    "<head>"
    "<title>Minimal Arduino Web Api</title>"
    "</head>"
    "<body>"
    "<h1>Minimal Arduino Web Api</h1>"
    "based on <a href=\"https://github.com/sirleech/Webduino\">Webduino library</a><br/>"
    "<b>version:</b>"
    VERSION_STRING
    "</body></html>";

void httpNotFound(WebServer &server){
  P(failMsg) =
    "HTTP/1.1 404 Not Found" CRLF
    "Server: Webduino/" WEBDUINO_VERSION_STRING CRLF
    "Content-Type: text/html" CRLF
    CRLF
    "<h2>File Not Found</h2>";
  server.printP(failMsg);
}  

void helloCmd(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
  server.httpSuccess();

  /* if we're handling a GET or POST, we can output our data here.
     For a HEAD request, we just stop after outputting headers. */
  if (type != WebServer::HEAD)
  {
    server.printP(Hello);
  }
}

/* It handles both GET and PUT requests.  For a GET, it returns a pin
 * value.  For a PUT it set the pin value. */
void pinsCmd(WebServer &server, WebServer::ConnectionType type, char **url_path, char *url_tail, bool tail_complete)
{

  if(strncmp(url_path[0],"pins",5)!=0){
    httpNotFound(server);
    return;
  }

  char* pin_param = url_path[1];
  int PIN_index = find_index(pin_param);

  if (type == WebServer::PUT)
  {
    #ifdef DEBUG_WEB_API
      Serial.println("Processing PUT query...");
    #endif

    int new_state = PIN_OFF;

    if ( PIN_index >= 0 )
    {
      server.httpSuccess();
      /* for url /pins/40/ON url_path[2] will contain 'ON' 
         and  url_path[1] will contain '40' */
      if(strcmp(url_path[2], "ON") == 0){
        new_state = PIN_ON;
      }

      if ( input_pins_states[PIN_index] != new_state )
      {
        digitalWrite(input_pins[PIN_index], new_state);
        input_pins_states[PIN_index] = new_state;
      }

      return;
    } else
    {
      httpNotFound(server);
      return;
    }
  }

  if (type == WebServer::GET)
  { 
    #ifdef DEBUG_SWITCHER
      Serial.println("Processing GET query...");
    #endif
    int PIN_index = find_index(pin_param);
    
    if ( PIN_index >= 0 )
    {
      server.httpSuccess();
      if( input_pins_states[PIN_index] == PIN_OFF)
        server.printP( Off );
      else
        server.printP( On );
      return;
    } else
    {
      httpNotFound(server);
      return;
    }
  }

  server.httpFail();
}

void setup()
{ 
  for (int i=0; i<SIZE(input_pins); i++) {
    digitalWrite(input_pins[i], PIN_OFF);
    pinMode(input_pins[i], OUTPUT);
  }

  delay( 250 ); // avoid manual reset of ethernet shield after power up http://www.freetronics.com.au/pages/usb-power-and-reset

  #ifdef DEBUG_WEB_API
    Serial.begin(9600);
    Serial.println("Connecting to network ...");
  #endif

  // setup the Ehternet library
  Ethernet.begin(mac, ip);
  #ifdef DEBUG_WEB_API
    Serial.println("OK!");
    Serial.println(Ethernet.localIP());
  #endif
  
  webserver.setDefaultCommand(&helloCmd);
  webserver.setUrlPathCommand(&pinsCmd);

  /* start the server to wait for connections */
  webserver.begin();
}

void loop()
{
  // process incoming connections one at a time forever
  webserver.processConnection();
}

int find_index(char *value)
{
   for (int i=0; i<SIZE(input_pins); i++)
   {
     pin_number_as_char = itoa(input_pins[i], pin_number_as_char, 10); // itoa reference http://www.nongnu.org/avr-libc/user-manual/group__avr__stdlib.html#gaa571de9e773dde59b0550a5ca4bd2f00
     if (strcmp(pin_number_as_char, value) == 0)
     {
        return(i); 
     }
   }
   return(-1);
}

