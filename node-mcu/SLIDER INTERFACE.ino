
#include <ESP8266WiFi.h>
WiFiServer server(80);

const char* ssid = "kartik";
const char* password = "123456789";
int redVal;
int redTemp=1;
int greTemp;
int bluTemp;
int greVal;
int bluVal;
int mattradVal;
int ledRed=13;
int ledGreen=12;
int ledBlue=14;
char colorBuff[4];


void setup() {
    Serial.begin(115200);  
    delay(50);
    Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  server.begin();  //Start the web server
    Serial.println("Server started");
  pinMode(ledGreen,OUTPUT);
  pinMode(ledRed,OUTPUT);
  pinMode(ledBlue,OUTPUT);
 
  digitalWrite(ledGreen,LOW);
  digitalWrite(ledRed,LOW);
  digitalWrite(ledBlue,LOW);
}

void loop() {
  int bufLength;

  WiFiClient client = server.available();    // Check if a client has connected
  if (!client) {
    return;  //do nothing if there is not a client for the server
  }
   
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
   
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println("Request is:");
  Serial.println(request);
  client.flush();

  if(request.indexOf("submit") != -1) {  //this is used to ignore "favicon.ico" and other requests
     int Pos_r = request.indexOf("redVal");
     int Pos_g = request.indexOf("greVal", Pos_r);  //finds a location of a string "g", after the position of "r"
     int Pos_b = request.indexOf("bluVal", Pos_g);
     int Pos_rad = request.indexOf("matrad",Pos_b);
     int End = request.indexOf("H", Pos_b);
     if(End < 0){
       End =  request.length() + 1;
     }
     //red
     bufLength = ((Pos_g) - (Pos_r+7));    //the 7 is for the "redVal=" string
     if(bufLength > 4){  //dont overflow the buffer  (we only want 3 digits)
       bufLength = 4;
     }    
     request.substring((Pos_r+7), (Pos_g-1)).toCharArray(colorBuff, bufLength);  //transfer substring to buffer  (cuts from past the "r=" to just before the "g", stores the length into a buffer)
     redVal = atoi(colorBuff);  //converts the array (3 digits) into an interger, this will be the red value
     //Green
     bufLength = ((Pos_b) - (Pos_g+7));    //the 7 is for the "greVal=" string
     if(bufLength > 4){  //dont overflow the buffer
       bufLength = 4;
     }      
     request.substring((Pos_g+7), (Pos_b-1)).toCharArray(colorBuff, bufLength);  //transfer substring to buffer
     greVal = atoi(colorBuff);
     //blue
     bufLength = ((Pos_rad) - (Pos_b+7));  //start to get the blue value
     if(bufLength > 4){  //dont overflow the buffer
       bufLength = 4;
     }      
     request.substring((Pos_b+7), (Pos_rad-1)).toCharArray(colorBuff, bufLength);  //transfer substring to buffer
     bluVal = atoi(colorBuff);



     bufLength = ((End) - (Pos_rad+7));
     if(bufLength > 4){  //dont overflow the buffer
       bufLength = 4;
     }      
     request.substring((Pos_rad+7), (End-1)).toCharArray(colorBuff, bufLength);  //transfer substring to buffer
     mattradVal = atoi(colorBuff);
    
     
    Serial.println("Red is:  ");
    Serial.println(redVal); 
    redTemp=redVal;
    Serial.println("Green is:  ");
    Serial.println(greVal); 
    Serial.println("Blue is:  ");
    Serial.println(bluVal);
     }

  // Return the response
  client.println("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
  client.println("<!DOCTYPE HTML>\r\n<html>\r\n<head>\r\n<title>Matt's test page</title>");
  client.println("<meta name='viewport' content='width=device-width', initial-scale='1'>");
  client.println("<br>");
  client.println("<body><center>Set the values of color");
  client.println("<form method=get><br>Red:  <input type='range' min='1' max='100' name=redVal value=redTemp oninput='showValue(this.value)'>");    //was onchange event
  client.println("<span id='range'>0</span>");
  client.println("<script type='text/javascript'>\r\nfunction showValue(newValue)\r\n{\r\ndocument.getElementById('range').innerHTML=newValue;\r\n}\r\n</script>\r\n");

  client.print("<br><br>Green:  <input type='range' min='1' max='100' name=greVal value=>");
  client.print("<br><br>Blue:  <input type='range' min='1' max='100' name=bluVal value=>");
  client.print("<br><br><input type=radio name='matrad' value='1' checked>flash series</>"); 
  client.print("<input type=radio name='matrad' value='2'>blink</>"); 
  client.print("<input type=radio name='matrad' value='3'>Solid On</>"); 
 
  client.println("&nbsp;<br><br><input name=H type=submit value=submit><br><br>");  //</form>
 
  client.println("</form>");
  client.println("<script type='text/javascript'></script>\r\n");
  client.println("</center>");
  client.println("</BODY>");
  client.println("</HTML>");
  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");

  analogWrite(ledGreen,(greVal*2.55));
  analogWrite(ledBlue,(bluVal*2.55));
  analogWrite(ledRed,(redVal*2.55));

  delay(10);
}
