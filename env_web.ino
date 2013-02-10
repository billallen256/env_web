#include <SPI.h>
#include <Ethernet.h>
#include <DHT11Interface.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x55, 0xB1 };
IPAddress ip(192,168,1, 201);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

// Initialize the DHT11 sensor
DHT11Interface sensor(2); // pin 2

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);

  Serial.println("Sensor: " + sensor.status());

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connnection: close");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head><title>Environmental Readings</title>");
          client.println("<meta http-equiv=\"refresh\" content=\"30\">"); // auto-refresh
          client.println("</head><body style=\"font-family:sans-serif;font-size:20pt;\">");
          client.println("Sensor: " + sensor.status() + "<br />");
          client.println("Humidity: " + sensor.humidityStr() + "<br />");
          client.println("Celsius: " + sensor.celsiusStr() + "<br />");
          client.println("Fahrenheit: " + sensor.fahrenheitStr() + "<br />");
          client.println("Kelvin: " + sensor.kelvinStr() + "<br />");
          client.println("Dew Point: " + sensor.dewPointStr() + "<br />");
          client.println("</body>");
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}

