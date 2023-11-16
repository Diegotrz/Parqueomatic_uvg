/*************************************************************************************************
  ESP32 Web Server
  Ejemplo de creación de Web server 
  Basándose en los ejemplos de: 
  https://lastminuteengineers.com/creating-esp32-web-server-arduino-ide/
  https://electropeak.com/learn
**************************************************************************************************/
//************************************************************************************************
// Librerías
//************************************************************************************************
#include <WiFi.h>
#include <WebServer.h>
//************************************************************************************************
// Variables globales
//************************************************************************************************
// SSID & Password
const char* ssid = "S20DT";  // Enter your SSID here
const char* password = "12345678a";  //Enter your Password here

WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)
//Variables de lectura para los uart
char uartiva1;
char uartiva2;
//Variables booleanas para los parqueos
bool parqueo1 = false;
bool parqueo2 = false;
bool parqueo3 = false;
bool parqueo4 = false;
bool parqueo5 = false;
bool parqueo6 = false;
bool parqueo7 = false;
bool parqueo8 = false;
//Variables para el manejo de los parqueos
uint8_t p1;
uint8_t p2;
uint8_t p3;
uint8_t p4;
uint8_t p5;
uint8_t p6;
uint8_t p7;
uint8_t p8;
uint8_t s1;
uint8_t s2;
uint8_t valdisp;
//Pines usados para el display
int adisp = 22;
int bdisp =1;
int cdisp =32;
int ddisp =33;
int edisp= 25;
int fdisp =26;
int gdisp =27;




//************************************************************************************************
// Configuración
//************************************************************************************************
void setup() {
  Serial.begin(115200);
  
  Serial1.begin(115200, SERIAL_8N1, 18, 5); //Habilitar el UART1
  Serial2.begin(115200, SERIAL_8N1, 16, 17); //UART2

  Serial.println("Try Connecting to ");
  Serial.println(ssid);



  // Connect to your wi-fi modem
  WiFi.begin(ssid, password);

  // Check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  pinMode(adisp,OUTPUT);
  pinMode(bdisp,OUTPUT);
  pinMode(cdisp,OUTPUT);
  pinMode(ddisp,OUTPUT);
  pinMode(edisp,OUTPUT);
  pinMode(fdisp,OUTPUT);
  pinMode(gdisp,OUTPUT);
  valdisp = 0;
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());  //Show ESP32 IP on serial

  server.on("/", handle_OnConnect); // Directamente desde e.g. 192.168.0.8
  server.on("/actualizar", handle_actualizar);
  
  
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
  delay(100);
  
}
//************************************************************************************************
// loop principal
//************************************************************************************************
void loop() {
server.handleClient();

}
//************************************************************************************************
// Handler de Inicio página
//************************************************************************************************
void handle_OnConnect() {
  parqueo1= false;
  server.send(200, "text/html", SendHTML(parqueo1,parqueo2,parqueo3,parqueo4,parqueo5,parqueo6,parqueo7,parqueo8));
  
}
//************************************************************************************************
// Handler de actualizar
//************************************************************************************************
void handle_actualizar() {
    uart1(); //Llamar a función para revisar los parqueos de la tiva 1
  uart2(); //Llamar a función para revisar los parqueos de la tiva 2
 valdisp = sumparqueos();
  valdisp = p1+p2+p3+p4+p5+p6+p7+p8; //Suma de los parqueos para verificar cuantos hay disponibles

  display_f(valdisp);

  Serial.println("Actualizar pagina");
  server.send(200, "text/html", SendHTML(parqueo1,parqueo2,parqueo3,parqueo4,parqueo5,parqueo6,parqueo7,parqueo8));
  Serial.println("cadena");
}

//************************************************************************************************
// Procesador de HTML
//************************************************************************************************
String SendHTML(bool parqueo1,bool parqueo2,bool parqueo3,bool parqueo4,bool parqueo5,bool parqueo6,bool parqueo7,bool parqueo8) {
String ptr = "<!DOCTYPE html>"; 
ptr += "<html>"; 
ptr += "<head>"; 
ptr += "<meta name=viewport content=\"width=device-width, initial-scale=1.0, user-scalable=no\">"; 
ptr += "<link rel=stylesheet href=https://stackpath.bootstrapcdn.com/bootstrap/4.5.0/css/bootstrap.min.css integrity=sha384-9aIt2nRpC12Uk9gS9baDl411NQApFmC26EwAOH8WgZl5MYYxFfc+NcPb1dKGj7Sk crossorigin=anonymous>"; 
ptr += "<title>LED Control</title>"; 
ptr += "<style>html{font-family:Helvetica;display:inline-block;margin:0 auto;text-align:center}body{margin-top:50px}h1{color:#444;margin:50px auto 30px;text-align:center}h3{color:#444;margin-bottom:50px}h2{color:#444;margin:50px auto 30px;text-align:center}h3{color:#444;margin-bottom:50px}.button{display:block;width:80px;background-color:#3498db;border:0;color:white;padding:15px 100px;text-decoration:none;font-size:25px;margin:0 auto 35px;cursor:pointer;border-radius:4px}.button-on{background-color:#3498db}.button-on:active{background-color:#2980b9}.button-off{background-color:#34495e}.button-off:active{background-color:#2c3e50}p{font-size:14px;color:#888;margin-bottom:10px}#imgpos1{position:absolute;top:900px;left:170px;width:350px;height:auto}#imgpos2{position:absolute;top:1150px;left:170px;width:350px;height:auto}#imgpos3{position:absolute;top:1370px;left:170px;width:350px;height:auto}#imgpos4{position:absolute;top:1600px;left:170px;width:350px;height:auto}#imgpos5{position:absolute;top:900px;left:900px;width:350px;height:auto}#imgpos6{position:absolute;top:1150px;left:900px;width:350px;height:auto}#imgpos7{position:absolute;top:1370px;left:900px;width:350px;height:auto}#imgpos8{position:absolute;top:1600px;left:900px;width:350px;height:auto}</style>"; 
ptr += "</head>"; 
ptr += "<body>"; 
ptr += "<h1> Parqueomatic &#128664 </h1>"; 
ptr += "<br>"; 
ptr += "<a class=\"button button-off\" href=actualizar>Actualizar</a>"; 
ptr += "<progress value=50 max=100></progress>"; 
ptr += "<br>"; 
ptr += "<table class=table>"; 
ptr += "<thead class=table-dark>"; 
ptr += "<tr>"; 
ptr += "<th scope=col># Parqueo</th>"; 
ptr += "<th scope=col>Estado</th>"; 
ptr += "</tr>"; 
ptr += "</thead>"; 
ptr += "<tbody>";
//Condicion del primer parqueo
if (parqueo1)
  {
    ptr += "<tr class=table-danger>"; 
ptr += "<th scope=row>1</th>"; 
ptr += "<td>Ocupado &#128683</td>"; 
ptr += "</tr>"; 
ptr += "<img id=imgpos1 src=prs.png alt=\"p1ocupado\">";

  }
  else
  {
    ptr += "<tr class=table-success>"; 
ptr += "<th scope=row>1</th>"; 
ptr += "<td>Disponible &#128664</td>"; 
ptr += "</tr>"; 
  }
if (parqueo2)
  {
    ptr += "<tr class=table-danger>"; 
ptr += "<th scope=row>2</th>"; 
ptr += "<td>Ocupado &#128683</td>"; 
ptr += "</tr>"; 
ptr += "<img id=imgpos2 src=prs.png alt=\"p1ocupado\">";
  }
  else
  {
    ptr += "<tr class=table-success>"; 
ptr += "<th scope=row>2</th>"; 
ptr += "<td>Disponible &#128664</td>"; 
ptr += "</tr>"; 
  }
if (parqueo3)
  {
    ptr += "<tr class=table-danger>"; 
ptr += "<th scope=row>3</th>"; 
ptr += "<td>Ocupado &#128683</td>"; 
ptr += "</tr>"; 
ptr += "<img id=imgpos3 src=prs.png alt=\"p1ocupado\">";
  }
  else
  {
    ptr += "<tr class=table-success>"; 
ptr += "<th scope=row>3</th>"; 
ptr += "<td>Disponible &#128664</td>"; 
ptr += "</tr>"; 
  }
if (parqueo4)
  {
    ptr += "<tr class=table-danger>"; 
ptr += "<th scope=row>4</th>"; 
ptr += "<td>Ocupado &#128683</td>"; 
ptr += "</tr>"; 
ptr += "<img id=imgpos4 src=prs.png alt=\"p1ocupado\">";
  }
  else
  {
    ptr += "<tr class=table-success>"; 
ptr += "<th scope=row>4</th>"; 
ptr += "<td>Disponible &#128664</td>"; 
ptr += "</tr>"; 
  }
 if (parqueo5)
  {
    ptr += "<tr class=table-danger>"; 
ptr += "<th scope=row>5</th>"; 
ptr += "<td>Ocupado &#128683</td>"; 
ptr += "</tr>"; 
ptr += "<img id=imgpos5 src=prs.png alt=\"p1ocupado\">";
  }
  else
  {
    ptr += "<tr class=table-success>"; 
ptr += "<th scope=row>5</th>"; 
ptr += "<td>Disponible &#128664</td>"; 
ptr += "</tr>"; 
  }
  if (parqueo6)
  {
    ptr += "<tr class=table-danger>"; 
ptr += "<th scope=row>6</th>"; 
ptr += "<td>Ocupado &#128683</td>"; 
ptr += "</tr>"; 
ptr += "<img id=imgpos6 src=prs.png alt=\"p1ocupado\">";
  }
  else
  {
    ptr += "<tr class=table-success>"; 
ptr += "<th scope=row>6</th>"; 
ptr += "<td>Disponible &#128664</td>"; 
ptr += "</tr>"; 
  }
if (parqueo7)
  {
    ptr += "<tr class=table-danger>"; 
ptr += "<th scope=row>7</th>"; 
ptr += "<td>Ocupado &#128683</td>"; 
ptr += "</tr>"; 
ptr += "<img id=imgpos7 src=prs.png alt=\"p1ocupado\">";
  }
  else
  {
    ptr += "<tr class=table-success>"; 
ptr += "<th scope=row>7</th>"; 
ptr += "<td>Disponible &#128664</td>"; 
ptr += "</tr>"; 
  }
if (parqueo8)
  {
    ptr += "<tr class=table-danger>"; 
ptr += "<th scope=row>8</th>"; 
ptr += "<td>Ocupado &#128683</td>"; 
ptr += "</tr>"; 
ptr += "<img id=imgpos8 src=prs.png alt=\"p1ocupado\">";
  }
  else
  {
    ptr += "<tr class=table-success>"; 
ptr += "<th scope=row>8</th>"; 
ptr += "<td>Disponible &#128664</td>"; 
ptr += "</tr>"; 
  }
  ptr += "</tbody>"; 
ptr += "</table>"; 
ptr += "<h2> Grafico del parqueo </h2>"; 
ptr += "<svg width=1525 height=30>"; 
ptr += "<rect width=\"1525\t\" height=30 style=fill:#e1b42c;stroke-width:10;stroke:#e1b42c />"; 
ptr += "</svg>"; 
ptr += "<div class=mx-auto style=width:75px>"; 
ptr += "<svg width=40 height=200>"; 
ptr += "<rect width=\"40\t\" height=200 style=fill:#e1b42c;stroke-width:10;stroke:#e1b42c />"; 
ptr += "</svg>"; 
ptr += "</div>"; 
ptr += "<svg width=1525 height=30>"; 
ptr += "<rect width=\"1525\t\" height=30 style=fill:#e1b42c;stroke-width:10;stroke:#e1b42c />"; 
ptr += "</svg>"; 
ptr += "<div class=mx-auto style=width:75px>"; 
ptr += "<svg width=40 height=200>"; 
ptr += "<rect width=\"40\t\" height=200 style=fill:#e1b42c;stroke-width:10;stroke:#e1b42c />"; 
ptr += "</svg>"; 
ptr += "</div>"; 
ptr += "<svg width=1525 height=30>"; 
ptr += "<rect width=\"1525\t\" height=30 style=fill:#e1b42c;stroke-width:10;stroke:#e1b42c />"; 
ptr += "</svg>"; 
ptr += "<div class=mx-auto style=width:75px>"; 
ptr += "<svg width=40 height=200>"; 
ptr += "<rect width=\"40\t\" height=200 style=fill:#e1b42c;stroke-width:10;stroke:#e1b42c />"; 
ptr += "</svg>"; 
ptr += "</div>"; 
ptr += "<svg width=1525 height=30>"; 
ptr += "<rect width=\"1525\t\" height=30 style=fill:#e1b42c;stroke-width:10;stroke:#e1b42c />"; 
ptr += "</svg>"; 

ptr += "<script src=https://code.jquery.com/jquery-3.5.1.slim.min.js integrity=sha384-DfXdz2htPH0lsSSs5nCTpuj/zy4C+OGpamoFVy38MVBnE+IbbVYUew+OrCXaRkfj crossorigin=anonymous></script>"; 
ptr += "<script src=https://cdn.jsdelivr.net/npm/popper.js@1.16.0/dist/umd/popper.min.js integrity=sha384-Q6E9RHvbIyZFJoft+2mJbHaEWldlvI9IOYy5n3zV9zzTtmI3UksdQRVvoxMfooAo crossorigin=anonymous></script>"; 
ptr += "<script src=https://stackpath.bootstrapcdn.com/bootstrap/4.5.0/js/bootstrap.min.js integrity=sha384-OgVRvuATP1z7JjHLkuOU7Xw704+h835Lr+6QL9UvYjZE3Ipu6Tp75j7Bh/kR0JKI crossorigin=anonymous></script>"; 
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
//************************************************************************************************
// Handler de not found
//************************************************************************************************
void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}
void display_f(uint8_t valdisp){
switch (valdisp){
case 0:
digitalWrite(adisp,HIGH);
digitalWrite(bdisp,HIGH);
digitalWrite(cdisp,HIGH);
digitalWrite(ddisp,HIGH);
digitalWrite(edisp,HIGH);
digitalWrite(fdisp,HIGH);
digitalWrite(gdisp,LOW);
break;
case 1:
digitalWrite(adisp,LOW);
digitalWrite(bdisp,HIGH);
digitalWrite(cdisp,HIGH);
digitalWrite(ddisp,LOW);
digitalWrite(edisp,LOW);
digitalWrite(fdisp,LOW);
digitalWrite(gdisp,LOW);
break;
case 2:
digitalWrite(adisp,HIGH);
digitalWrite(bdisp,HIGH);
digitalWrite(cdisp,LOW);
digitalWrite(ddisp,HIGH);
digitalWrite(edisp,HIGH);
digitalWrite(fdisp,LOW);
digitalWrite(gdisp,HIGH);
break;
case 3:
digitalWrite(adisp,HIGH);
digitalWrite(bdisp,HIGH);
digitalWrite(cdisp,HIGH);
digitalWrite(ddisp,HIGH);
digitalWrite(edisp,LOW);
digitalWrite(fdisp,LOW);
digitalWrite(gdisp,HIGH);
break;
case 4:
digitalWrite(adisp,LOW);
digitalWrite(bdisp,HIGH);
digitalWrite(cdisp,HIGH);
digitalWrite(ddisp,LOW);
digitalWrite(edisp,LOW);
digitalWrite(fdisp,HIGH);
digitalWrite(gdisp,HIGH);
break;
case 5:
digitalWrite(adisp,HIGH);
digitalWrite(bdisp,LOW);
digitalWrite(cdisp,HIGH);
digitalWrite(ddisp,HIGH);
digitalWrite(edisp,LOW);
digitalWrite(fdisp,HIGH);
digitalWrite(gdisp,HIGH);
break;
case 6:
digitalWrite(adisp,HIGH);
digitalWrite(bdisp,LOW);
digitalWrite(cdisp,HIGH);
digitalWrite(ddisp,HIGH);
digitalWrite(edisp,HIGH);
digitalWrite(fdisp,HIGH);
digitalWrite(gdisp,HIGH);
break;
case 7:
digitalWrite(adisp,HIGH);
digitalWrite(bdisp,HIGH);
digitalWrite(cdisp,HIGH);
digitalWrite(ddisp,LOW);
digitalWrite(edisp,LOW);
digitalWrite(fdisp,LOW);
digitalWrite(gdisp,LOW);
break;
case 8:
digitalWrite(adisp,HIGH);
digitalWrite(bdisp,HIGH);
digitalWrite(cdisp,HIGH);
digitalWrite(ddisp,HIGH);
digitalWrite(edisp,HIGH);
digitalWrite(fdisp,HIGH);
digitalWrite(gdisp,HIGH);
break;
}



}
//Función para chequear la información del uart1
void uart1(){
  uartiva1 = Serial1.read(); //Leer de UART1
switch (uartiva1) {
case 0:
    parqueo1 = false;
    parqueo2 = false;
    parqueo3 = false;
    parqueo4 = false;
break;
case 1:
    parqueo1 = true;
    parqueo2 = false;
    parqueo3 = false;
    parqueo4 = false;
break;
case 2:
    parqueo1 = false;
    parqueo2 = true;
    parqueo3 = false;
    parqueo4 = false;
break;
case 3:
    parqueo1 = true;
    parqueo2 = true;
    parqueo3 = false;
    parqueo4 = false;
break; 
case 4:
    parqueo1 = false;
    parqueo2 = false;
    parqueo3 = true;
    parqueo4 = false;
break;
case 5:
    parqueo1 = true;
    parqueo2 = false;
    parqueo3 = true;
    parqueo4 = false;
break;
case 6:
    parqueo1 = false;
    parqueo2 = true;
    parqueo3 = true;
    parqueo4 = false;
break;
case 7:
    parqueo1 = true;
    parqueo2 = true;
    parqueo3 = true;
    parqueo4 = false;
break; 
case 8:
      parqueo1 = false;
      parqueo2 = false;
      parqueo3 = false;
      parqueo4 = true;
break;
case 9:
    parqueo1 = true;
    parqueo2 = false;
    parqueo3 = false;
    parqueo4 = true;
break;
case 10:
    parqueo1 = false;
    parqueo2 = true;
    parqueo3 = false;
    parqueo4 = true;
break;
case 11:
    parqueo1 = true;
    parqueo2 = true;
    parqueo3 = false;
    parqueo4 = true;
break;
case 12:
     parqueo1 = false;
    parqueo2 = false;
    parqueo3 = true;
    parqueo4 = true;
break;
case 13:
    parqueo1 = true;
    parqueo2 = false;
    parqueo3 = true;
    parqueo4 = true;
break;
case 14:
    parqueo1 = false;
    parqueo2 = true;
    parqueo3 = true;
    parqueo4 = true;
break;
case 15:
    parqueo1 = true;
    parqueo2 = true;
    parqueo3 = true;
    parqueo4 = true;
break;
}
}
//Función para chequear la información del uart1
void uart2(){
  uartiva2 = Serial1.read(); //Leer de UART1
switch (uartiva2) {
case 0:
    parqueo5 = false;
    parqueo6 = false;
    parqueo7 = false;
    parqueo8 = false;
break;
case 1:
    parqueo5 = true;
    parqueo6 = false;
    parqueo7 = false;
    parqueo8 = false;
break;
case 2:
    parqueo5 = false;
    parqueo6 = true;
    parqueo7 = false;
    parqueo8 = false;
break;
case 3:
    parqueo5 = true;
    parqueo6 = true;
    parqueo7 = false;
    parqueo8 = false;
break; 
case 4:
    parqueo5 = false;
    parqueo6 = false;
    parqueo7 = true;
    parqueo8 = false;
break;
case 5:
    parqueo5 = true;
    parqueo6 = false;
    parqueo7 = true;
    parqueo8 = false;
break;
case 6:
    parqueo5 = false;
    parqueo6 = true;
    parqueo7 = true;
    parqueo8 = false;
break;
case 7:
    parqueo5 = true;
    parqueo6 = true;
    parqueo7 = true;
    parqueo8 = false;
break; 
case 8:
      parqueo5 = false;
      parqueo6 = false;
      parqueo7 = false;
      parqueo8 = true;
break;
case 9:
    parqueo5 = true;
    parqueo6 = false;
    parqueo7 = false;
    parqueo8 = true;
break;
case 10:
    parqueo5 = false;
    parqueo6 = true;
    parqueo7 = false;
    parqueo8 = true;
break;
case 11:
    parqueo5 = true;
    parqueo6 = true;
    parqueo7 = false;
    parqueo8 = true;
break;
case 12:
    parqueo5 = false;
    parqueo6 = false;
    parqueo7 = true;
    parqueo8 = true;
break;
case 13:
    parqueo5 = true;
    parqueo6 = false;
    parqueo7 = true;
    parqueo8 = true;
break;
case 14:
    parqueo5 = false;
    parqueo6 = true;
    parqueo7 = true;
    parqueo8 = true;
break;
case 15:
    parqueo5 = true;
    parqueo6 = true;
    parqueo7 = true;
    parqueo8 = true;
break;
}
}
int sumparqueos(){
if (parqueo1 == true){
    p1 = 1;
  }
  else{
    p1 = 0;
  }
  if (parqueo2 == true){
    p2 = 1;
  }
  else{
    p2 = 0;
  }
  if (parqueo3 == true){
    p3 = 1;
  }
  else{
    p3 = 0;
  }
  if (parqueo4 == true){
    p4 = 1;
  }
  else{
    p4 = 0;
  }
  if (parqueo5 == true){
    p5 = 1;
  }
  else{
    p5 = 0;
  }
  if (parqueo6 == true){
    p6 = 1;
  }
  else{
    p6 = 0;
  }
  if (parqueo7 == true){
    p7 = 1;
  }
  else{
    p7 = 0;
  }
  if (parqueo8 == true){
    p8 = 1;
  }
  else{
    p8 = 0;
  }
int parqsum = p1+p2+p3+p4+p5+p6+p7+p8;
return parqsum;
}