//Monitorización de deslizamientos de tierra en el Macizo Colombiano del sur del departamento del Cauca mediante IoRT
//TESIS GRANOBLE
//By: Cristian Camilo Granoble Gaviria - Luis Alejandro Granoble Gaviria

//Codigo para rangos: Modo2 y Modo 3


////// LIBRERIAS ////////

  #include <TimeLib.h>
  #include <Arduino.h>
  #include <WiFi.h>
  #include <WiFiMulti.h>
  #include <SoftwareSerial.h>
  #include "TelnetClient.h"
  #include <OneWire.h>            //Librerias para sensor de temperatura de suelo
  #include <DallasTemperature.h> //Librerias para sensor de temperatura de suelo

  
  #define pinData 32        // para sensor de temperatura de suelo
  OneWire ourWire(pinData); // para sensor de temperatura de suelo
  DallasTemperature sensors(&ourWire); // para sensor de temperatura de suelo
  #include<ArduinoJson.h>   //Librerias para Json
  #include "DHT.h"          // Para sensor de temperatura y humedad relativa

//////// DEFINICION DE PINES ////////

  #define DHTPIN 33         // Pin donde está conectado el sensor
  //#define DHTTYPE DHT11   // Descomentar si se usa el DHT 11
  #define DHTTYPE DHT22     // Sensor DHT22
  DHT dht(DHTPIN, DHTTYPE); // Para DHT

  #define CONTROL 25        // Para control de envio de mensaje

// Titulo para cada RSSI
void onDataCallBack(String data) 
{
    Serial.print("DATA: ");
    Serial.print(data);
}

////////// Configuracion Telnet ///////

TelnetConfig createTelnetConfig() 
{
    TelnetConfig cfg;
    cfg.serverURL = "192.168.4.1";
    cfg.port = 23;
    cfg.serialDbg = &Serial;
    cfg.onDataCallback = &onDataCallBack;
    return cfg;
}

TelnetConfig cfg = createTelnetConfig();
TelnetClient client(cfg);

///// Verificacion nmea: Tomado de documentacion Swarm Hive /////

String nmeaCommand(String msg) 
{
  const char one = '$';
  const char *two = "TD ";
  const char three = '\"';
  const String four = msg;
  //const char inter = '!';
  const char five = '\"';
  const char six = '*';

  //String total( String(two) + three + four + inter + five );
  String total( String(two) + three + four + five );

  const char* command = total.c_str();

  uint8_t checksum = nmeaChecksum(command, strlen(command));

  // Checksum (uint8_t) a hexadecimal como String
  char hexadecimalchecksum [5];
  sprintf(hexadecimalchecksum, "%X", checksum);

  String total2( String(one) + command + six + String(hexadecimalchecksum) );
  const char* completeCommand = total2.c_str();
  
  return String(completeCommand);

}


uint8_t nmeaChecksum (const char *sz, size_t len)
{
  size_t  i = 0;
  uint8_t cs;

  if (sz [0] == '$')
    i++;

  for (cs = 0; (i<len) && sz [i]; i++)
    cs ^= ((uint8_t) sz [i]);

  return cs;  

}

void setup() 
{
    Serial.begin(9600);
    Serial.println();
    Serial.println("... Iniciando trabajo de grado GRANOBLE ...");
    WiFiMulti WiFiMulti;

    dht.begin();      // Para iniciar DHT
    sensors.begin();  // Para iniciar sensor de temperatura de suelo    
    
    
    WiFiMulti.addAP("swarm-93a0", "12345678");
    while (WiFiMulti.run() != WL_CONNECTED) {
        delay(100);
    }
    cfg.serialDbg->println("WiFi connected. IP address: ");
    cfg.serialDbg->println(WiFi.localIP());
    delay(50);
    if (!client.connect()) {
        cfg.serialDbg->print(":(");
    }
        
    pinMode(CONTROL, INPUT_PULLUP);


///////// Para inicializar la hora y fecha en el reloj. /////////

// Ponemos la hora que necesitamos que inicie

  setTime(23,00,32,14,9,2024);
  if (timeStatus()!= timeSet){
     Serial.println("No se logro sincronizar");
  }else{
     Serial.println("Tenemos el reloj sincronizado");
  }
//Para inicializar la hora y fecha en el reloj.

}

        ///////// VARIABLES PARA PROMEDIOS //////

        float promedio1;    //PROMEDIO HUMEDAD AMBIENTE
        float promedio2;    //PROMEDIO TEMPERATURA AMBIENTE
        float promedio3;    //PROMEDIO HUMEDAD DEL SUELO
        float promedio4;    //PROMEDIO TEMPERATURADEL SUELO
        float promedio5;    //PROMEDIO LLUVIA

        ///////// PARA PROMEDIO 1 ///////

        float promedio1_1;    //PROMEDIO HUMEDAD AMBIENTE
        float promedio2_1;    //PROMEDIO TEMPERATURA AMBIENTE
        float promedio3_1;    //PROMEDIO HUMEDAD DEL SUELO
        float promedio4_1;    //PROMEDIO TEMPERATURADEL SUELO
        float promedio5_1;    //PROMEDIO LLUVIA

        //////// PARA PROMEDIO 2 /////////

        float promedio1_2;    //PROMEDIO HUMEDAD AMBIENTE
        float promedio2_2;    //PROMEDIO TEMPERATURA AMBIENTE
        float promedio3_2;    //PROMEDIO HUMEDAD DEL SUELO
        float promedio4_2;   //PROMEDIO TEMPERATURADEL SUELO
        float promedio5_2;    //PROMEDIO LLUVIA

        ///////// PARA PROMEDIO 3 ////////

        float promedio1_3;    //PROMEDIO HUMEDAD AMBIENTE
        float promedio2_3;    //PROMEDIO TEMPERATURA AMBIENTE
        float promedio3_3;    //PROMEDIO HUMEDAD DEL SUELO
        float promedio4_3;    //PROMEDIO TEMPERATURADEL SUELO
        float promedio5_3;    //PROMEDIO LLUVIA

        //////// PARA PROMEDIO 4 ////////

        float promedio1_4;    //PROMEDIO HUMEDAD AMBIENTE
        float promedio2_4;    //PROMEDIO TEMPERATURA AMBIENTE
        float promedio3_4;    //PROMEDIO HUMEDAD DEL SUELO
        float promedio4_4;    //PROMEDIO TEMPERATURADEL SUELO
        float promedio5_4;    //PROMEDIO LLUVIA

        //////// PARA PROMEDIO 5 //////// 

        float promedio1_5;    //PROMEDIO HUMEDAD AMBIENTE
        float promedio2_5;    //PROMEDIO TEMPERATURA AMBIENTE
        float promedio3_5;    //PROMEDIO HUMEDAD DEL SUELO
        float promedio4_5;    //PROMEDIO TEMPERATURADEL SUELO
        float promedio5_5;    //PROMEDIO LLUVIA

        //////// PARA PROMEDIO 6 //////// 

        float promedio1_6;    //PROMEDIO HUMEDAD AMBIENTE
        float promedio2_6;    //PROMEDIO TEMPERATURA AMBIENTE
        float promedio3_6;    //PROMEDIO HUMEDAD DEL SUELO
        float promedio4_6;    //PROMEDIO TEMPERATURADEL SUELO
        float promedio5_6;    //PROMEDIO LLUVIA

        //////// PARA PROMEDIO 7 //////// 

        float promedio1_7;    //PROMEDIO HUMEDAD AMBIENTE
        float promedio2_7;    //PROMEDIO TEMPERATURA AMBIENTE
        float promedio3_7;    //PROMEDIO HUMEDAD DEL SUELO
        float promedio4_7;    //PROMEDIO TEMPERATURADEL SUELO
        float promedio5_7;    //PROMEDIO LLUVIA

        //////// PARA PROMEDIO 8 //////// 

        float promedio1_8;    //PROMEDIO HUMEDAD AMBIENTE
        float promedio2_8;    //PROMEDIO TEMPERATURA AMBIENTE
        float promedio3_8;    //PROMEDIO HUMEDAD DEL SUELO
        float promedio4_8;    //PROMEDIO TEMPERATURADEL SUELO
        float promedio5_8;    //PROMEDIO LLUVIA

        //////// PARA PROMEDIO 9 //////// 

        float promedio1_9;    //PROMEDIO HUMEDAD AMBIENTE
        float promedio2_9;    //PROMEDIO TEMPERATURA AMBIENTE
        float promedio3_9;    //PROMEDIO HUMEDAD DEL SUELO
        float promedio4_9;    //PROMEDIO TEMPERATURADEL SUELO
        float promedio5_9;    //PROMEDIO LLUVIA

        //////// PARA PROMEDIO 10 //////// 

        float promedio1_10;    //PROMEDIO HUMEDAD AMBIENTE
        float promedio2_10;    //PROMEDIO TEMPERATURA AMBIENTE
        float promedio3_10;    //PROMEDIO HUMEDAD DEL SUELO
        float promedio4_10;    //PROMEDIO TEMPERATURADEL SUELO
        float promedio5_10;    //PROMEDIO LLUVIA

        //////// PARA PROMEDIO 11 //////// 

        float promedio1_11;    //PROMEDIO HUMEDAD AMBIENTE
        float promedio2_11;    //PROMEDIO TEMPERATURA AMBIENTE
        float promedio3_11;    //PROMEDIO HUMEDAD DEL SUELO
        float promedio4_11;    //PROMEDIO TEMPERATURADEL SUELO
        float promedio5_11;    //PROMEDIO LLUVIA

        //////// PARA PROMEDIO 12 //////// 

        float promedio1_12;    //PROMEDIO HUMEDAD AMBIENTE
        float promedio2_12;    //PROMEDIO TEMPERATURA AMBIENTE
        float promedio3_12;    //PROMEDIO HUMEDAD DEL SUELO
        float promedio4_12;    //PROMEDIO TEMPERATURADEL SUELO
        float promedio5_12;    //PROMEDIO LLUVIA

        //////// PARA PROMEDIO 13 //////// 

        float promedio1_13;    //PROMEDIO HUMEDAD AMBIENTE
        float promedio2_13;    //PROMEDIO TEMPERATURA AMBIENTE
        float promedio3_13;    //PROMEDIO HUMEDAD DEL SUELO
        float promedio4_13;    //PROMEDIO TEMPERATURADEL SUELO
        float promedio5_13;    //PROMEDIO LLUVIA


        ////////  VARIABLES PARA LA SUMA DEL PROMEDIO //////// 

        // PARA SUMA 1

          float suma1 = 0;    //PROMEDIO HUMEDAD AMBIENTE
          float suma2 = 0;    //PROMEDIO TEMPERATURA AMBIENTE
          float suma3 = 0;    //PROMEDIO HUMEDAD DEL SUELO
          float suma4 = 0;    //PROMEDIO TEMPERATURADEL SUELO
          float suma5 = 0;    //PROMEDIO LLUVIA

        // PARA SUMA 1  

          float suma_1 = 0;    //PROMEDIO HUMEDAD AMBIENTE
          float suma_2 = 0;    //PROMEDIO TEMPERATURA AMBIENTE
          float suma_3 = 0;    //PROMEDIO HUMEDAD DEL SUELO
          float suma_4 = 0;    //PROMEDIO TEMPERATURADEL SUELO
          float suma_5 = 0;    //PROMEDIO LLUVIA

        // PARA SUMA 2

          float resta1_2;
          float resta2_2;
          float resta3_2;
          float resta4_2;
          float resta5_2; 

          // PARA SUMA 3

          float resta1_3;
          float resta2_3;
          float resta3_3;
          float resta4_3;
          float resta5_3; 

          // PARA SUMA 4

          float resta1_4;
          float resta2_4;
          float resta3_4;
          float resta4_4;
          float resta5_4;

          // PARA SUMA 5

          float resta1_5;
          float resta2_5;
          float resta3_5;
          float resta4_5;
          float resta5_5;

          // PARA SUMA 6

          float resta1_6;
          float resta2_6;
          float resta3_6;
          float resta4_6;
          float resta5_6;

          // PARA SUMA 7

          float resta1_7;
          float resta2_7;
          float resta3_7;
          float resta4_7;
          float resta5_7;

          // PARA SUMA 8

          float resta1_8;
          float resta2_8;
          float resta3_8;
          float resta4_8;
          float resta5_8;

          // PARA SUMA 9

          float resta1_9;
          float resta2_9;
          float resta3_9;
          float resta4_9;
          float resta5_9;

          // PARA SUMA 10

          float resta1_10;
          float resta2_10;
          float resta3_10;
          float resta4_10;
          float resta5_10;

          // PARA SUMA 2

          float resta1_11;
          float resta2_11;
          float resta3_11;
          float resta4_11;
          float resta5_11;

          // PARA SUMA 12

          float resta1_12;
          float resta2_12;
          float resta3_12;
          float resta4_12;
          float resta5_12;

          // PARA SUMA 13

          float resta1_13;
          float resta2_13;
          float resta3_13;
          float resta4_13;
          float resta5_13;

          // variable para contador de horas
          int c = 0;

void loop() 
{
  StaticJsonDocument<256> doc;  //Declaro la funcion, 256 define la cantidad de memoria ue quiero ocupar

  //Organizamos la Hora y fecha 

    const char dospuntos = ':';

    const int hora = hour();
    const int minuto = minute();
    const int segundo = second();

    String horario( String(hora) + dospuntos + minuto + dospuntos + segundo );


    const char barra = '/';

    const int mes = month();
    const int dia = day();
    const int año = year();

    String fecha( String(dia) + barra + mes + barra + año );

  // .... Para Hora y Fecha

      // DELAY PARA HACER PRUEBAS
      //  delay(15000);
      
      // Mostramos la hora y fecha
        Serial.println();
                Serial.print("............. Hora: ");
                Serial.print(horario);
                Serial.print(" - Fecha: ");
                Serial.print(fecha);
                Serial.print("  .............");
                Serial.println();

          ////// COMENTAMOS EL FOR, ESTA BIEN PERO NECETIAMOS COMENTARLO

      // Hacemos un for para tomar las 13 horas (84/6=13)
      //cada 6 repeticion (6-12-18-24...) hay un if donde realiza el promedio de las ultimas 6 repeticiones
      //Cada 6 ciclos, es una hora
      //cada repeticion (cada ciclo k++) se demora 10 minutos (delay(60000))

      for(int K = 1; K<=84; K++)
        {
                    //  contador para las 12 horas
                      //c=c+1;
                      c++;

                    sensors.requestTemperatures(); // para sensor de temperatura de suelo  /////////////////////////////////
                    //Serial.print(sensors.getTempCByIndex(0)); // para sensor de temperatura de suelo


                    int lectura_lluvia = analogRead(35); //  para declarar y leer pin analogo A0    
                    int porcentaje_lluvia = map(lectura_lluvia, 3500, 800, 0, 100);
            
            
                    int lectura_hum_suelo = analogRead(34); //  para declarar y leer pin analogo A0
                    int porcentaje_hum_suelo = map(lectura_hum_suelo, 4500, 2000, 0, 100);
            
                    //StaticJsonDocument<256> doc;  //Declaro la funcion, 256 define la cantidad de memoria ue quiero ocupar
              
                    //INICIALIZACION DHT22

                    float h = dht.readHumidity(); //Leemos la Humedad
                    float t = dht.readTemperature(); //Leemos la temperatura en grados Celsius
                    float f = dht.readTemperature(true); //Leemos la temperatura en grados Fahrenheit

                                
                  // REALIZACION DE SUMA DE CADA SENSOR

                          suma1 = suma1 + h;
                          suma2 = suma2 + t;
                          suma3 = suma3 + porcentaje_hum_suelo;
                          suma4 = suma4 + sensors.getTempCByIndex(0);
                          suma5 = suma5 + porcentaje_lluvia;
            
                          // IMPRIMIMOS EL VALOR DE CADA SUMA Y EL VALOR DE CADA SENSOR

                          Serial.println();
                          Serial.print("................... Lectura: Numero ");
                          Serial.print(c);
                          Serial.print("  ...................");
                          Serial.println();

                          // SENSOR DE HUMEDAD RELATIVA

                          Serial.println();
                          Serial.print("----------------------------------------------------------");
                          Serial.println();

                          //client.wait4Data();

                          Serial.println();
                          Serial.print("> Valor: Humedad relativa ");
                          Serial.print(c);
                          Serial.print(" = ");
                          Serial.print(h, DEC); 
                          Serial.println(" % ");
                          //Serial.println();

                          Serial.print("> Suma: Humedad relativa ");
                          Serial.print(c);
                          Serial.print(" = ");
                          Serial.print(suma1, DEC); 
                          Serial.println(" % ");
                          Serial.println();
                
                          //client.wait4Data();
                          
                          //Serial.println();
                          Serial.print("----------------------------------------------------------");
                          Serial.println();
                          

                          // SENSOR DE TEMPERATURA AMBIENTE
                          
                          Serial.println();
                          Serial.print("> Valor: Temperatura ambiente ");
                          Serial.print(c);
                          Serial.print(" = ");
                          Serial.print(t, DEC);
                          Serial.println(" C "); 
                          //Serial.println();

                          Serial.print("> Suma: Temperatura ambiente ");
                          Serial.print(c);
                          Serial.print(" = ");
                          Serial.print(suma2, DEC); 
                          Serial.println(" C "); 
                          Serial.println();

                          //client.wait4Data();
                        
                        
                          //Serial.println();
                          Serial.print("----------------------------------------------------------");
                          Serial.println();
                          
                          // SENSOR DE HUMEDAD DEL SUELO
                          
                          Serial.println();
                          Serial.print("> Valor: Humedad superficial del suelo ");
                          Serial.print(c);
                          Serial.print(" = ");
                          Serial.print(porcentaje_hum_suelo, DEC);
                          Serial.println(" % "); 
                          //Serial.println();

                          Serial.print("> Suma: Humedad superficial del suelo ");
                          Serial.print(c);
                          Serial.print(" = ");
                          Serial.print(suma3, DEC); 
                          Serial.println(" % ");
                          Serial.println();

                          //client.wait4Data();

                          //Serial.println();
                          Serial.print("----------------------------------------------------------");
                          Serial.println();
                                      
                          // SENSOR DE TEMPERATURA SUELO
                          
                          Serial.println();
                          Serial.print("> Valor: Temperaura superficial del suelo ");
                          Serial.print(c);
                          Serial.print(" = ");
                          Serial.print(sensors.getTempCByIndex(0), DEC); 
                          Serial.println(" C ");
                          //Serial.println();

                          Serial.print("> Suma: Temperatura superficial del suelo ");
                          Serial.print(c);
                          Serial.print(" = ");
                          Serial.print(suma4, DEC); 
                          Serial.println(" C ");
                          Serial.println();

                          //client.wait4Data();

                          //Serial.println();
                          Serial.print("----------------------------------------------------------");
                          Serial.println();
                          
                          // SENSOR DE PRECIPITACION
                          
                          Serial.println();
                          Serial.print("> Valor: Precipitacion ");
                          Serial.print(c);
                          Serial.print(" = ");
                          Serial.print(porcentaje_lluvia, DEC); 
                          Serial.println(" % ");
                          //Serial.println();

                          Serial.print("> Suma: Precipitacion ");
                          Serial.print(c);
                          Serial.print(" = ");
                          Serial.print(suma5, DEC); 
                          Serial.println(" % ");
                          Serial.println();

                          //client.wait4Data();

                          //Serial.println();
                          Serial.print("----------------------------------------------------------");
                          Serial.println();
                          Serial.println();

                          // PAUSA DE 10 MINUTOS

                         Serial.println();
                          Serial.print("............... Lectura: Numero ");
                          Serial.print(K);
                          Serial.print("  ...............");
                          Serial.println();

                        // delay(600000); // 60000 ES 1 MINUTO, PERO DEBEMOS POPNER 10 MIN = 600000 milisegundos
                         /////////// delay(5000); // 10000 ms = 10 seg

                          //
                  //PARA GUARDAR EL PROMEDIO DE CADA HORA

                  // HORA 1
                  if(K==6)
                  { 
                    //Promedios
                    promedio1_1 = suma1/6;
                    promedio2_1 = suma2/6;
                    promedio3_1 = suma3/6;
                    promedio4_1 = suma4/6;
                    promedio5_1 = suma5/6;
                    
                    //Suma de cada hora
                    suma_1 = suma1;
                    suma_2 = suma2;
                    suma_3 = suma3;
                    suma_4 = suma4;
                    suma_5 = suma5;

                    //HORA Y FECHA
                    //String hora_1;
                    //String fecha_1;

                    //hora_1 = horario;
                    //fecha_1 = fecha;

                    


                    //PROMEDIO HUMEDAD RELAIVA
                    Serial.println("..........................................................");
                    Serial.print(".................... PROMEDIOS HORA 1 .....................");
                    Serial.println();
                    Serial.println();

                    Serial.print("- Promedio: Humedad relativa =  ");
                    Serial.println(promedio1_1, DEC); 
                    Serial.println();

                    // PROMEDIO TEMPERATURA AMBIENTE

                    Serial.print("- Promedio: Temperatura ambiente =  ");
                    Serial.println(promedio2_1, DEC); 
                    Serial.println();

                    // PROMEDIO HUMEDAD SUELO

                    Serial.print("- Promedio: Humedad superficial del suelo =  ");
                    Serial.println(promedio3_1, DEC); 
                    Serial.println();

                    // PROMEDIO TEMPERATURA SUELO

                    Serial.print("- Promedio: Temperatura superficial del suelo =  ");
                    Serial.println(promedio4_1, DEC); 
                    Serial.println();

                    // PROMEDIO PRECIPITACION

                    Serial.print("- Promedio: Precipitacion =  ");
                    Serial.println(promedio5_1, DEC); 
                    //Serial.println();

                    //Serial.println();
                    Serial.println("..........................................................");
                    Serial.println("..........................................................");
                    Serial.println();

                    c=0;

                    //REINICIAMOS SUMA
                      suma1=0;
                      suma2=0;
                      suma3=0;
                      suma4=0;
                      suma5=0;
                    
                    
                  }  

                  // HORA 2
                  if(K==12)
                  {
                

                    //Promedio
                    promedio1_2 = suma1/6;
                    promedio2_2 = suma2/6;
                    promedio3_2 = suma3/6;
                    promedio4_2 = suma4/6;
                    promedio5_2 = suma5/6;

                    //PROMEDIO HUMEDAD RELAIVA
                    Serial.println("..........................................................");
                    Serial.print(".................... PROMEDIOS HORA 2 .....................");
                    Serial.println();
                    Serial.println();

                    Serial.print("- Promedio: Humedad relativa =  ");
                    Serial.println(promedio1_2, DEC); 
                    Serial.println();

                    // PROMEDIO TEMPERATURA AMBIENTE

                    Serial.print("- Promedio: Temperatura ambiente =  ");
                    Serial.println(promedio2_2, DEC); 
                    Serial.println();

                    // PROMEDIO HUMEDAD SUELO

                    Serial.print("- Promedio: Humedad superficial del suelo =  ");
                    Serial.println(promedio3_2, DEC); 
                    Serial.println();

                    // PROMEDIO TEMPERATURA SUELO

                    Serial.print("- Promedio: Temperatura superficial del suelo =  ");
                    Serial.println(promedio4_2, DEC); 
                    Serial.println();

                    // PROMEDIO PRECIPITACION

                    Serial.print("- Promedio: Precipitacion =  ");
                    Serial.println(promedio5_2, DEC); 
                    //Serial.println();

                    //Serial.println();
                    Serial.println("..........................................................");
                    Serial.println("..........................................................");
                    Serial.println();

                    //Reiniciamos contador cada hora
                    c=0;

                    //REINICIAMOS SUMA
                      suma1=0;
                      suma2=0;
                      suma3=0;
                      suma4=0;
                      suma5=0;
                  }  

                  // HORA 3
                  if(K==18)
                  {
                    
                    //Promedio
                    promedio1_3 = suma1/6;
                    promedio2_3 = suma2/6;
                    promedio3_3 = suma3/6;
                    promedio4_3 = suma4/6;
                    promedio5_3 = suma5/6;

                    //IMPRIMIR PROMEDIO 
                    Serial.println("..........................................................");
                    Serial.print("................... PROMEDIOS HORA 3 ......................");
                    Serial.println();
                    Serial.println();

                    Serial.print("- Promedio: Humedad relativa =  ");
                    Serial.println(promedio1_3, DEC); 
                    Serial.println();

                    // PROMEDIO TEMPERATURA AMBIENTE

                    Serial.print("- Promedio: Temperatura ambiente =  ");
                    Serial.println(promedio2_3, DEC); 
                    Serial.println();

                    // PROMEDIO HUMEDAD SUELO

                    Serial.print("- Promedio: Humedad superficial del suelo =  ");
                    Serial.println(promedio3_3, DEC); 
                    Serial.println();

                    // PROMEDIO TEMPERATURA SUELO

                    Serial.print("- Promedio: Temperatura superficial del suelo =  ");
                    Serial.println(promedio4_3, DEC); 
                    Serial.println();

                    // PROMEDIO PRECIPITACION

                    Serial.print("- Promedio: Precipitacion =  ");
                    Serial.println(promedio5_3, DEC); 
                    //Serial.println();

                    //Serial.println();
                    Serial.println("..........................................................");
                    Serial.println("..........................................................");
                    Serial.println();

                    //Reiniciamos contador cada hora
                    c=0;

                    //REINICIAMOS SUMA
                      suma1=0;
                      suma2=0;
                      suma3=0;
                      suma4=0;
                      suma5=0;
                  }  

                  // HORA 4
                  if(K==24)
                  {
                  
                    //Promedio
                    promedio1_4 = suma1/6;
                    promedio2_4 = suma2/6;
                    promedio3_4 = suma3/6;
                    promedio4_4 = suma4/6;
                    promedio5_4 = suma5/6;

                    //IMPRIMIR PROMEDIO 
                    Serial.println("..........................................................");
                    Serial.print("................... PROMEDIOS HORA 4 ......................");
                    Serial.println();
                    Serial.println();

                    Serial.print("- Promedio: Humedad relativa =  ");
                    Serial.println(promedio1_4, DEC); 
                    Serial.println();

                    // PROMEDIO TEMPERATURA AMBIENTE

                    Serial.print("- Promedio: Temperatura ambiente =  ");
                    Serial.println(promedio2_4, DEC); 
                    Serial.println();

                    // PROMEDIO HUMEDAD SUELO

                    Serial.print("- Promedio: Humedad superficial del suelo =  ");
                    Serial.println(promedio3_4, DEC); 
                    Serial.println();

                    // PROMEDIO TEMPERATURA SUELO

                    Serial.print("- Promedio: Temperatura superficial del suelo =  ");
                    Serial.println(promedio4_4, DEC); 
                    Serial.println();

                    // PROMEDIO PRECIPITACION

                    Serial.print("- Promedio: Precipitacion =  ");
                    Serial.println(promedio5_4, DEC); 
                    //Serial.println();

                    //Serial.println();
                    Serial.println("..........................................................");
                    Serial.println("..........................................................");
                    Serial.println();
                    
                    //Reiniciamos contador cada hora
                    c=0;

                    //REINICIAMOS SUMA
                      suma1=0;
                      suma2=0;
                      suma3=0;
                      suma4=0;
                      suma5=0;
                  }  

                  // HORA 5
                  if(K==30)
                  {
                   
                    //Promedio
                    promedio1_5 = suma1/6;
                    promedio2_5 = suma2/6;
                    promedio3_5 = suma3/6;
                    promedio4_5 = suma4/6;
                    promedio5_5 = suma5/6;

                    //IMPRIMIR PROMEDIO 
                    Serial.println("..........................................................");
                    Serial.print("................... PROMEDIOS HORA 5 ......................");
                    Serial.println();
                    Serial.println();

                    Serial.print("- Promedio: Humedad relativa =  ");
                    Serial.println(promedio1_5, DEC); 
                    Serial.println();

                    // PROMEDIO TEMPERATURA AMBIENTE

                    Serial.print("- Promedio: Temperatura ambiente =  ");
                    Serial.println(promedio2_5, DEC); 
                    Serial.println();

                    // PROMEDIO HUMEDAD SUELO

                    Serial.print("- Promedio: Humedad superficial del suelo =  ");
                    Serial.println(promedio3_5, DEC); 
                    Serial.println();

                    // PROMEDIO TEMPERATURA SUELO

                    Serial.print("- Promedio: Temperatura superficial del suelo =  ");
                    Serial.println(promedio4_5, DEC); 
                    Serial.println();

                    // PROMEDIO PRECIPITACION

                    Serial.print("- Promedio: Precipitacion =  ");
                    Serial.println(promedio5_5, DEC); 
                    //Serial.println();

                    //Serial.println();
                    Serial.println("..........................................................");
                    Serial.println("..........................................................");
                    Serial.println();

                    //Reiniciamos contador cada hora
                    c=0;

                    //REINICIAMOS SUMA
                      suma1=0;
                      suma2=0;
                      suma3=0;
                      suma4=0;
                      suma5=0;
                  }  

                  // HORA 6
                  if(K==36)
                  {
                    //Promedio
                    promedio1_6 = suma1/6;
                    promedio2_6 = suma2/6;
                    promedio3_6 = suma3/6;
                    promedio4_6 = suma4/6;
                    promedio5_6 = suma5/6;

                    //IMPRIMIR PROMEDIO 
                    Serial.println("..........................................................");
                    Serial.print("................... PROMEDIOS HORA 6 ......................");
                    Serial.println();
                    Serial.println();

                    Serial.print("- Promedio: Humedad relativa =  ");
                    Serial.println(promedio1_6, DEC); 
                    Serial.println();

                    // PROMEDIO TEMPERATURA AMBIENTE

                    Serial.print("- Promedio: Temperatura ambiente =  ");
                    Serial.println(promedio2_6, DEC); 
                    Serial.println();

                    // PROMEDIO HUMEDAD SUELO

                    Serial.print("- Promedio: Humedad superficial del suelo =  ");
                    Serial.println(promedio3_6, DEC); 
                    Serial.println();

                    // PROMEDIO TEMPERATURA SUELO

                    Serial.print("- Promedio: Temperatura superficial del suelo =  ");
                    Serial.println(promedio4_6, DEC); 
                    Serial.println();

                    // PROMEDIO PRECIPITACION

                    Serial.print("- Promedio: Precipitacion =  ");
                    Serial.println(promedio5_6, DEC); 
                    //Serial.println();

                    //Serial.println();
                    Serial.println("..........................................................");
                    Serial.println("..........................................................");
                    Serial.println();

                    //Reiniciamos contador cada hora
                    c=0;

                    //REINICIAMOS SUMA
                      suma1=0;
                      suma2=0;
                      suma3=0;
                      suma4=0;
                      suma5=0;

                  }  

                  // HORA 7
                  if(K==42)
                  {
                   
                    //Promedio
                    promedio1_7 = suma1/6;
                    promedio2_7 = suma2/6;
                    promedio3_7 = suma3/6;
                    promedio4_7 = suma4/6;
                    promedio5_7 = suma5/6;

                    //IMPRIMIR PROMEDIO 
                    Serial.println("..........................................................");
                    Serial.print("................... PROMEDIOS HORA 7 ......................");
                    Serial.println();
                    Serial.println();

                    Serial.print("- Promedio: Humedad relativa =  ");
                    Serial.println(promedio1_7, DEC); 
                    Serial.println();

                    // PROMEDIO TEMPERATURA AMBIENTE

                    Serial.print("- Promedio: Temperatura ambiente =  ");
                    Serial.println(promedio2_7, DEC); 
                    Serial.println();

                    // PROMEDIO HUMEDAD SUELO

                    Serial.print("- Promedio: Humedad superficial del suelo =  ");
                    Serial.println(promedio3_7, DEC); 
                    Serial.println();

                    // PROMEDIO TEMPERATURA SUELO

                    Serial.print("- Promedio: Temperatura superficial del suelo =  ");
                    Serial.println(promedio4_7, DEC); 
                    Serial.println();

                    // PROMEDIO PRECIPITACION

                    Serial.print("- Promedio: Precipitacion =  ");
                    Serial.println(promedio5_7, DEC); 
                    //Serial.println();

                    //Serial.println();
                    Serial.println("..........................................................");
                    Serial.println("..........................................................");
                    Serial.println();

                    //Reiniciamos contador cada hora
                    c=0;

                    //REINICIAMOS SUMA
                      suma1=0;
                      suma2=0;
                      suma3=0;
                      suma4=0;
                      suma5=0;


                  }  

                  // HORA 8
                  if(K==48)
                  {
                   
                    //Promedio
                    promedio1_8 = suma1/6;
                    promedio2_8 = suma2/6;
                    promedio3_8 = suma3/6;
                    promedio4_8 = suma4/6;
                    promedio5_8 = suma5/6;


                    //IMPRIMIR PROMEDIO 
                    Serial.println("..........................................................");
                    Serial.print("................... PROMEDIOS HORA 8 ......................");
                    Serial.println();
                    Serial.println();

                    Serial.print("- Promedio: Humedad relativa =  ");
                    Serial.println(promedio1_8, DEC); 
                    Serial.println();

                    // PROMEDIO TEMPERATURA AMBIENTE

                    Serial.print("- Promedio: Temperatura ambiente =  ");
                    Serial.println(promedio2_8, DEC); 
                    Serial.println();

                    // PROMEDIO HUMEDAD SUELO

                    Serial.print("- Promedio: Humedad superficial del suelo =  ");
                    Serial.println(promedio3_8, DEC); 
                    Serial.println();

                    // PROMEDIO TEMPERATURA SUELO

                    Serial.print("- Promedio: Temperatura superficial del suelo =  ");
                    Serial.println(promedio4_8, DEC); 
                    Serial.println();

                    // PROMEDIO PRECIPITACION

                    Serial.print("- Promedio: Precipitacion =  ");
                    Serial.println(promedio5_8, DEC); 
                    //Serial.println();

                    //Serial.println();
                    Serial.println("..........................................................");
                    Serial.println("..........................................................");
                    Serial.println();

                    //Reiniciamos contador cada hora
                    c=0;

                    //REINICIAMOS SUMA
                      suma1=0;
                      suma2=0;
                      suma3=0;
                      suma4=0;
                      suma5=0;


                  }  

                  // HORA 9
                  if(K==54)
                  {
                  
                    //Promedio
                    promedio1_9 = suma1/6;
                    promedio2_9 = suma2/6;
                    promedio3_9 = suma3/6;
                    promedio4_9 = suma4/6;
                    promedio5_9 = suma5/6;

                    //IMPRIMIR PROMEDIO 
                    Serial.println("..........................................................");
                    Serial.print("................... PROMEDIOS HORA 9 ......................");
                    Serial.println();
                    Serial.println();

                    Serial.print("- Promedio: Humedad relativa =  ");
                    Serial.println(promedio1_9, DEC); 
                    Serial.println();

                    // PROMEDIO TEMPERATURA AMBIENTE

                    Serial.print("- Promedio: Temperatura ambiente =  ");
                    Serial.println(promedio2_9, DEC); 
                    Serial.println();

                    // PROMEDIO HUMEDAD SUELO

                    Serial.print("- Promedio: Humedad superficial del suelo =  ");
                    Serial.println(promedio3_9, DEC); 
                    Serial.println();

                    // PROMEDIO TEMPERATURA SUELO

                    Serial.print("- Promedio: Temperatura superficial del suelo =  ");
                    Serial.println(promedio4_9, DEC); 
                    Serial.println();

                    // PROMEDIO PRECIPITACION

                    Serial.print("- Promedio: Precipitacion =  ");
                    Serial.println(promedio5_9, DEC); 
                    //Serial.println();

                    //Serial.println();
                    Serial.println("..........................................................");
                    Serial.println("..........................................................");
                    Serial.println();

                    //Reiniciamos contador cada hora
                    c=0;

                    //REINICIAMOS SUMA
                      suma1=0;
                      suma2=0;
                      suma3=0;
                      suma4=0;
                      suma5=0;


                  }  

                  // HORA 10
                  if(K==60)
                  {
                      //Promedio
                    promedio1_10 = suma1/6;
                    promedio2_10 = suma2/6;
                    promedio3_10 = suma3/6;
                    promedio4_10 = suma4/6;
                    promedio5_10 = suma5/6;

                    //IMPRIMIR PROMEDIO 
                    Serial.println("..........................................................");
                    Serial.print("................... PROMEDIOS HORA 10 ......................");
                    Serial.println();
                    Serial.println();

                    Serial.print("- Promedio: Humedad relativa =  ");
                    Serial.println(promedio1_10, DEC); 
                    Serial.println();

                    // PROMEDIO TEMPERATURA AMBIENTE

                    Serial.print("- Promedio: Temperatura ambiente =  ");
                    Serial.println(promedio2_10, DEC); 
                    Serial.println();

                    // PROMEDIO HUMEDAD SUELO

                    Serial.print("- Promedio: Humedad superficial del suelo =  ");
                    Serial.println(promedio3_10, DEC); 
                    Serial.println();

                    // PROMEDIO TEMPERATURA SUELO

                    Serial.print("- Promedio: Temperatura superficial del suelo =  ");
                    Serial.println(promedio4_10, DEC); 
                    Serial.println();

                    // PROMEDIO PRECIPITACION

                    Serial.print("- Promedio: Precipitacion =  ");
                    Serial.println(promedio5_10, DEC); 
                    //Serial.println();

                    //Serial.println();
                    Serial.println("..........................................................");
                    Serial.println("..........................................................");
                    Serial.println();

                    //Reiniciamos contador cada hora
                    c=0;

                    //REINICIAMOS SUMA
                      suma1=0;
                      suma2=0;
                      suma3=0;
                      suma4=0;
                      suma5=0;


                  }

                  // HORA 11
                  if(K==66)
                  {
                   

                    //Promedio
                    promedio1_11 = suma1/6;
                    promedio2_11 = suma2/6;
                    promedio3_11 = suma3/6;
                    promedio4_11 = suma4/6;
                    promedio5_11 = suma5/6;

                    //IMPRIMIR PROMEDIO 
                    Serial.println("..........................................................");
                    Serial.print("................... PROMEDIOS HORA 11 ......................");
                    Serial.println();
                    Serial.println();

                    Serial.print("- Promedio: Humedad relativa =  ");
                    Serial.println(promedio1_11, DEC); 
                    Serial.println();

                    // PROMEDIO TEMPERATURA AMBIENTE

                    Serial.print("- Promedio: Temperatura ambiente =  ");
                    Serial.println(promedio2_11, DEC); 
                    Serial.println();

                    // PROMEDIO HUMEDAD SUELO

                    Serial.print("- Promedio: Humedad superficial del suelo =  ");
                    Serial.println(promedio3_11, DEC); 
                    Serial.println();

                    // PROMEDIO TEMPERATURA SUELO

                    Serial.print("- Promedio: Temperatura superficial del suelo =  ");
                    Serial.println(promedio4_11, DEC); 
                    Serial.println();

                    // PROMEDIO PRECIPITACION

                    Serial.print("- Promedio: Precipitacion =  ");
                    Serial.println(promedio5_11, DEC); 
                    //Serial.println();

                    //Serial.println();
                    Serial.println("..........................................................");
                    Serial.println("..........................................................");
                    Serial.println();

                    //Reiniciamos contador cada hora
                    c=0;

                    //REINICIAMOS SUMA
                      suma1=0;
                      suma2=0;
                      suma3=0;
                      suma4=0;
                      suma5=0;

                  } 

                  // HORA 12
                  if(K==72)
                  {
                   
                    //Promedio
                    promedio1_12 = suma1/6;
                    promedio2_12 = suma2/6;
                    promedio3_12 = suma3/6;
                    promedio4_12 = suma4/6;
                    promedio5_12 = suma5/6;

                    //IMPRIMIR PROMEDIO 
                    Serial.println("..........................................................");
                    Serial.print("................... PROMEDIOS HORA 12 ......................");
                    Serial.println();
                    Serial.println();

                    Serial.print("- Promedio: Humedad relativa =  ");
                    Serial.println(promedio1_12, DEC); 
                    Serial.println();

                    // PROMEDIO TEMPERATURA AMBIENTE

                    Serial.print("- Promedio: Temperatura ambiente =  ");
                    Serial.println(promedio2_12, DEC); 
                    Serial.println();

                    // PROMEDIO HUMEDAD SUELO

                    Serial.print("- Promedio: Humedad superficial del suelo =  ");
                    Serial.println(promedio3_12, DEC); 
                    Serial.println();

                    // PROMEDIO TEMPERATURA SUELO

                    Serial.print("- Promedio: Temperatura superficial del suelo =  ");
                    Serial.println(promedio4_12, DEC); 
                    Serial.println();

                    // PROMEDIO PRECIPITACION

                    Serial.print("- Promedio: Precipitacion =  ");
                    Serial.println(promedio5_12, DEC); 
                    //Serial.println();

                    //Serial.println();
                    Serial.println("..........................................................");
                    Serial.println("..........................................................");
                    Serial.println();

                    //Reiniciamos contador cada hora
                    c=0;

                    //REINICIAMOS SUMA
                      suma1=0;
                      suma2=0;
                      suma3=0;
                      suma4=0;
                      suma5=0;

                      // HORA 13
                  if(K==84)
                  {
                   
                    //Promedio
                    promedio1_13 = suma1/6;
                    promedio2_13 = suma2/6;
                    promedio3_13 = suma3/6;
                    promedio4_13 = suma4/6;
                    promedio5_13 = suma5/6;

                    //IMPRIMIR PROMEDIO 
                    Serial.println("..........................................................");
                    Serial.print("................... PROMEDIOS HORA 13 ......................");
                    Serial.println();
                    Serial.println();

                    Serial.print("- Promedio: Humedad relativa =  ");
                    Serial.println(promedio1_13, DEC); 
                    Serial.println();

                    // PROMEDIO TEMPERATURA AMBIENTE

                    Serial.print("- Promedio: Temperatura ambiente =  ");
                    Serial.println(promedio2_13, DEC); 
                    Serial.println();

                    // PROMEDIO HUMEDAD SUELO

                    Serial.print("- Promedio: Humedad superficial del suelo =  ");
                    Serial.println(promedio3_13, DEC); 
                    Serial.println();

                    // PROMEDIO TEMPERATURA SUELO

                    Serial.print("- Promedio: Temperatura superficial del suelo =  ");
                    Serial.println(promedio4_13, DEC); 
                    Serial.println();

                    // PROMEDIO PRECIPITACION

                    Serial.print("- Promedio: Precipitacion =  ");
                    Serial.println(promedio5_13, DEC); 
                    //Serial.println();

                    //Serial.println();
                    Serial.println("..........................................................");
                    Serial.println("..........................................................");
                    Serial.println();

                    //Reiniciamos contador cada hora
                    c=0;

                    //REINICIAMOS SUMA
                      suma1=0;
                      suma2=0;
                      suma3=0;
                      suma4=0;
                      suma5=0;

                  }

                  }  

                  delay(600000); // delay de 10 minutos ( toma datos de los sensorres cada 10 minutos)
              
        }      

         
 
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//NORA IMPORTANTE: Aqui cambiamos el delay dependiendo al modo en que stemos  (Rango o Hora)

//MODO2: PONEMOS UN DELAY PARA RANGO DE HORA (2AM- 3AM)   

          delay(3600000*19);      //1 hora = 3.600.000 milisegundo - y cada hora lo multiplicamos por las horas que necesitamos
                                  //en este caso configuramos la hora para qu empiece a las 18:00 por tanto, multiplicamos por 19, para que
                                  //solo envie a las 2 am del dia siguiente

//MODO3: PONEMOS UN DELAY PARA RANGO DE HORA (1AM- 10AM) 

         //delay(3600000*18);      //1 hora = 3.600.000 milisegundo - y cada hora lo multiplicamos por las horas que necesitamos
                                  //en este caso configuramos la hora para qu empiece a las 18:00 por tanto, multiplicamos por 18, para que
                                  //solo envie a las 1 am del dia siguiente


  // ********** SERIALIZAMOS (CONCEPTO USADO PARA PASAR DATOS A FORMATO JSON) **********  //

    
            //PARA PROMEDIO 1 - HORA 1
          {
            doc["Hum.R: "] = promedio1_1 ; //////
            doc["Temp.A: "] = promedio2_1 ; //////
            //doc["TEMP.RELATIVA (Fº): "] = f ;
            doc["Hum.S: "] = promedio3_1; //////
            doc["Temp.S: "] = promedio4_1; /////
            doc["Lluvia: "] = promedio5_1; ////////
            //doc["Hora:"] = hora_1;
            //doc["Fecha:"] = fecha_1;   
            doc["Hora:"] = horario;
            doc["Fecha:"] = fecha; 
          }

           char out[256]; // Defino una variable de salida y le asigno cantidade de memoria en bytes
           int L = serializeJson(doc, out);    //L contiene la cantidad de bytes, out ya está en JSON


 

            //PARA PROMEDIO 2 - HORA 2
          {
            doc["Hum.R: "] = promedio1_2 ; //////
            doc["Temp.A: "] = promedio2_2 ; //////
            //doc["TEMP.RELATIVA (Fº): "] = f ;
            doc["Hum.S: "] = promedio3_2; //////
            doc["Temp.S: "] = promedio4_2; /////
            doc["Lluvia: "] = promedio5_2; ////////
            doc["Hora:"] = horario;
            doc["Fecha:"] = fecha;   

          }
          
          //L2 = pone en tipo Json los adatos anteriores  
          //char out[256]; // Defino una variable de salida y le asigno cantidade de memoria en bytes
          int L2 = serializeJson(doc, out);    //L contiene la cantidad de bytes, out ya está en JSON


            //PARA PROMEDIO 3 - HORA 3
          {
            doc["Hum.R: "] = promedio1_3 ; //////
            doc["Temp.A: "] = promedio2_3 ; //////
            //doc["TEMP.RELATIVA (Fº): "] = f ;
            doc["Hum.S: "] = promedio3_3; //////
            doc["Temp.S: "] = promedio4_3; /////
            doc["Lluvia: "] = promedio5_3; ////////
            //doc["Hora:"] = hora_3;
            //doc["Fecha:"] = fecha_3; 
            doc["Hora:"] = horario;
            doc["Fecha:"] = fecha;   
          }

          //L2 = pone en tipo Json los adatos anteriores  
         //char out[256]; // Defino una variable de salida y le asigno cantidade de memoria en bytes
          int L3 = serializeJson(doc, out);    //L contiene la cantidad de bytes, out ya está en JSON



            //PARA PROMEDIO 4 - HORA 4
          {
            doc["Hum.R: "] = promedio1_4 ; //////
            doc["Temp.A: "] = promedio2_4 ; //////
            //doc["TEMP.RELATIVA (Fº): "] = f ;
            doc["Hum.S: "] = promedio3_4; //////
            doc["Temp.S: "] = promedio4_4; /////
            doc["Lluvia: "] = promedio5_4; ////////
            //doc["Hora:"] = hora_4;
            //doc["Fecha:"] = fecha_4;
            doc["Hora:"] = horario;
            doc["Fecha:"] = fecha;    
          }

          //L2 = pone en tipo Json los adatos anteriores  
           //char out[256]; // Defino una variable de salida y le asigno cantidade de memoria en bytes
          int L4 = serializeJson(doc, out);    //L contiene la cantidad de bytes, out ya está en JSON


            //PARA PROMEDIO 5 - HORA 5

          {
            doc["Hum.R: "] = promedio1_5 ; //////
            doc["Temp.A: "] = promedio2_5 ; //////
            //doc["TEMP.RELATIVA (Fº): "] = f ;
            doc["Hum.S: "] = promedio3_5; //////
            doc["Temp.S: "] = promedio4_5; /////
            doc["Lluvia: "] = promedio5_5; ////////
            //doc["Hora:"] = hora_5;
            //doc["Fecha:"] = fecha_5; 
            doc["Hora:"] = horario;
            doc["Fecha:"] = fecha;   
          }

          //L2 = pone en tipo Json los adatos anteriores  
          //char out[256]; // Defino una variable de salida y le asigno cantidade de memoria en bytes
          int L5 = serializeJson(doc, out);    //L contiene la cantidad de bytes, out ya está en JSON


            //PARA PROMEDIO 6 - HORA 6
          { 
            doc["Hum.R: "] = promedio1_6 ; //////
            doc["Temp.A: "] = promedio2_6 ; //////
            //doc["TEMP.RELATIVA (Fº): "] = f ;
            doc["Hum.S: "] = promedio3_6; //////
            doc["Temp.S: "] = promedio4_6; /////
            doc["Lluvia: "] = promedio5_6; ////////
            //doc["Hora:"] = hora_6;
            //doc["Fecha:"] = fecha_6;   
            doc["Hora:"] = horario;
            doc["Fecha:"] = fecha; 
          }

          //L2 = pone en tipo Json los adatos anteriores  
         //char out[256]; // Defino una variable de salida y le asigno cantidade de memoria en bytes
          int L6 = serializeJson(doc, out);    //L contiene la cantidad de bytes, out ya está en JSON


            //PARA PROMEDIO 7 - HORA 7
          {
            doc["Hum.R: "] = promedio1_7 ; //////
            doc["Temp.A: "] = promedio2_7 ; //////
            //doc["TEMP.RELATIVA (Fº): "] = f ;
            doc["Hum.S: "] = promedio3_7; //////
            doc["Temp.S: "] = promedio4_7; /////
            doc["Lluvia: "] = promedio5_7; ////////
            //doc["Hora:"] = hora_7;
            //doc["Fecha:"] = fecha_7;
            doc["Hora:"] = horario;
            doc["Fecha:"] = fecha;    
          }

          //L2 = pone en tipo Json los adatos anteriores  
         //char out[256]; // Defino una variable de salida y le asigno cantidade de memoria en bytes
          int L7 = serializeJson(doc, out);    //L contiene la cantidad de bytes, out ya está en JSON


            //PARA PROMEDIO 8 - HORA 8
          {
            doc["Hum.R: "] = promedio1_8 ; //////
            doc["Temp.A: "] = promedio2_8 ; //////
            //doc["TEMP.RELATIVA (Fº): "] = f ;
            doc["Hum.S: "] = promedio3_8; //////
            doc["Temp.S: "] = promedio4_8; /////
            doc["Lluvia: "] = promedio5_8; ////////
            //doc["Hora:"] = hora_8;
            //doc["Fecha:"] = fecha_8;   
            doc["Hora:"] = horario;
            doc["Fecha:"] = fecha; 
          }

          //L2 = pone en tipo Json los adatos anteriores  
           //char out[256]; // Defino una variable de salida y le asigno cantidade de memoria en bytes
          int L8 = serializeJson(doc, out);    //L contiene la cantidad de bytes, out ya está en JSON



          //PARA PROMEDIO 9 - HORA 9
          {
            doc["Hum.R: "] = promedio1_9 ; //////
            doc["Temp.A: "] = promedio2_9 ; //////
            //doc["TEMP.RELATIVA (Fº): "] = f ;
            doc["Hum.S: "] = promedio3_9; //////
            doc["Temp.S: "] = promedio4_9; /////
            doc["Lluvia: "] = promedio5_9; ////////
            //doc["Hora:"] = hora_9;
            //doc["Fecha:"] = fecha_9;  
            doc["Hora:"] = horario;
            doc["Fecha:"] = fecha;  
          }

          //L2 = pone en tipo Json los adatos anteriores  
           //char out[256]; // Defino una variable de salida y le asigno cantidade de memoria en bytes
          int L9 = serializeJson(doc, out);    //L contiene la cantidad de bytes, out ya está en JSON



         //PARA PROMEDIO 10 - HORA 10 - 03
          {
            doc["Hum.R: "] = promedio1_10 ; //////
            doc["Temp.A: "] = promedio2_10 ; //////
            //doc["TEMP.RELATIVA (Fº): "] = f ;
            doc["Hum.S: "] = promedio3_10; //////
            doc["Temp.S: "] = promedio4_10; /////
            doc["Lluvia: "] = promedio5_10; ////////
            //doc["Hora:"] = hora_10;
            //doc["Fecha:"] = fecha_10; 
            doc["Hora:"] = horario;
            doc["Fecha:"] = fecha;   
          }

          //L2 = pone en tipo Json los adatos anteriores  
           //char out[256]; // Defino una variable de salida y le asigno cantidade de memoria en bytes
          int L10 = serializeJson(doc, out);    //L contiene la cantidad de bytes, out ya está en JSON


        //PARA PROMEDIO 11 - HORA 11 - 04
          {
            doc["Hum.R: "] = promedio1_11 ; //////
            doc["Temp.A: "] = promedio2_11 ; //////
            //doc["TEMP.RELATIVA (Fº): "] = f ;
            doc["Hum.S: "] = promedio3_11; //////
            doc["Temp.S: "] = promedio4_11; /////
            doc["Lluvia: "] = promedio5_11; ////////
            //doc["Hora:"] = hora_11;
            //doc["Fecha:"] = fecha_11;   
            doc["Hora:"] = horario;
            doc["Fecha:"] = fecha; 
          }

          //L2 = pone en tipo Json los adatos anteriores  
           //char out[256]; // Defino una variable de salida y le asigno cantidade de memoria en bytes
          int L11 = serializeJson(doc, out);    //L contiene la cantidad de bytes, out ya está en JSON



          //PARA PROMEDIO 12 - HORA 12 - 05
          {
            doc["Hum.R: "] = promedio1_12 ; //////
            doc["Temp.A: "] = promedio2_12 ; //////
            //doc["TEMP.RELATIVA (Fº): "] = f ;
            doc["Hum.S: "] = promedio3_12; //////
            doc["Temp.S: "] = promedio4_12; /////
            doc["Lluvia: "] = promedio5_12; ////////
            //doc["Hora:"] = hora_12;
            //doc["Fecha:"] = fecha_12;   
            doc["Hora:"] = horario;
            doc["Fecha:"] = fecha; 
          }

          //L2 = pone en tipo Json los adatos anteriores  
           //char out[256]; // Defino una variable de salida y le asigno cantidade de memoria en bytes
          int L12 = serializeJson(doc, out);    //L contiene la cantidad de bytes, out ya está en JSON




           //PARA PROMEDIO 13 - HORA 06
          {
            doc["Hum.R: "] = promedio1_13 ; //////
            doc["Temp.A: "] = promedio2_13 ; //////
            //doc["TEMP.RELATIVA (Fº): "] = f ;
            doc["Hum.S: "] = promedio3_13; //////
            doc["Temp.S: "] = promedio4_13; /////
            doc["Lluvia: "] = promedio5_13; ////////
            //doc["Hora:"] = hora_13;
            //doc["Fecha:"] = fecha_13; 
            doc["Hora:"] = horario;
            doc["Fecha:"] = fecha;   
          }

          //L2 = pone en tipo Json los adatos anteriores  
           // char out[256]; // Defino una variable de salida y le asigno cantidade de memoria en bytes
          int L13 = serializeJson(doc, out);    //L contiene la cantidad de bytes, out ya está en JSON





// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 


          
            //DELAY PARA HACER PRUEBAS

            //delay(15000);
            
            //if(lectura_lluvia == 1 ){ doc["LLUVIA: "] = "0"; } else doc["LLUVIA: "] = "1";   

            // %%%%% PARA HORA 1 - 18 %%%%
            
       //     char out[256]; // Defino una variable de salida y le asigno cantidade de memoria en bytes
      //      int L = serializeJson(doc, out);    //L contiene la cantidad de bytes, out ya está en JSON

            

  
            int entrada = digitalRead(CONTROL);   
          // int k = 0;  
            
            if(entrada == LOW)
            {

                //  String command = nmeaCommand("Prueba inicial!");
                //  client.sendData(command); 

                // %%%%%%%%%% PARA HORA 1 %%%%%%%%%%%%%%%%%%5

                  String command = nmeaCommand(out);
                  client.sendData(command); 

                  Serial.print("Comando Enviado: ");          
                  Serial.println(command);  
                  Serial.print("Tamaño (En Bytes) del mensaje =  ");
                  Serial.println(L, DEC);   

                  // %%%%%%%%%% PARA HORA 2 %%%%%%%%%%%%%%%%%%5

                  //String command = nmeaCommand(out);
                  client.sendData(command); 

                  Serial.print("Comando Enviado: ");          
                  Serial.println(command);  
                  Serial.print("Tamaño (En Bytes) del mensaje =  ");
                  Serial.println(L2, DEC); 

                  // %%%%%%%%%% PARA HORA 3 %%%%%%%%%%%%%%%%%%5

                  //String command = nmeaCommand(out);
                  client.sendData(command); 

                  Serial.print("Comando Enviado: ");          
                  Serial.println(command);  
                  Serial.print("Tamaño (En Bytes) del mensaje =  ");
                  Serial.println(L3, DEC); 

                  // %%%%%%%%%% PARA HORA 4 %%%%%%%%%%%%%%%%%%5

                  //String command = nmeaCommand(out);
                  client.sendData(command); 

                  Serial.print("Comando Enviado: ");          
                  Serial.println(command);  
                  Serial.print("Tamaño (En Bytes) del mensaje =  ");
                  Serial.println(L4, DEC); 

                  // %%%%%%%%%% PARA HORA 5 %%%%%%%%%%%%%%%%%%5

                 //String command = nmeaCommand(out);
                  client.sendData(command); 

                  Serial.print("Comando Enviado: ");          
                  Serial.println(command);  
                  Serial.print("Tamaño (En Bytes) del mensaje =  ");
                  Serial.println(L5, DEC); 

                  // %%%%%%%%%% PARA HORA 6 %%%%%%%%%%%%%%%%%%5

                  //String command = nmeaCommand(out);
                  client.sendData(command); 

                  Serial.print("Comando Enviado: ");          
                  Serial.println(command);  
                  Serial.print("Tamaño (En Bytes) del mensaje =  ");
                  Serial.println(L6, DEC); 

                  // %%%%%%%%%% PARA HORA 7 %%%%%%%%%%%%%%%%%%5

                  //String command = nmeaCommand(out);
                  client.sendData(command); 

                  Serial.print("Comando Enviado: ");          
                  Serial.println(command);  
                  Serial.print("Tamaño (En Bytes) del mensaje =  ");
                  Serial.println(L7, DEC); 

                  // %%%%%%%%%% PARA HORA 8 %%%%%%%%%%%%%%%%%%5

                  //String command = nmeaCommand(out);
                  client.sendData(command); 

                  Serial.print("Comando Enviado: ");          
                  Serial.println(command);  
                  Serial.print("Tamaño (En Bytes) del mensaje =  ");
                  Serial.println(L8, DEC); 

                  // %%%%%%%%%% PARA HORA 9 %%%%%%%%%%%%%%%%%%5

                  //String command = nmeaCommand(out);
                  client.sendData(command); 

                  Serial.print("Comando Enviado: ");          
                  Serial.println(command);  
                  Serial.print("Tamaño (En Bytes) del mensaje =  ");
                  Serial.println(L9, DEC); 

                  // %%%%%%%%%% PARA HORA 10 %%%%%%%%%%%%%%%%%%5

                  //String command = nmeaCommand(out);
                  client.sendData(command); 

                  Serial.print("Comando Enviado: ");          
                  Serial.println(command);  
                  Serial.print("Tamaño (En Bytes) del mensaje =  ");
                  Serial.println(L10, DEC); 

                  // %%%%%%%%%% PARA HORA 11 %%%%%%%%%%%%%%%%%%5

                  //String command = nmeaCommand(out);
                  client.sendData(command); 

                  Serial.print("Comando Enviado: ");          
                  Serial.println(command);  
                  Serial.print("Tamaño (En Bytes) del mensaje =  ");
                  Serial.println(L11, DEC); 

                  // %%%%%%%%%% PARA HORA 12 %%%%%%%%%%%%%%%%%%5

                 // String command = nmeaCommand(out);
                  client.sendData(command); 

                  Serial.print("Comando Enviado: ");          
                  Serial.println(command);  
                  Serial.print("Tamaño (En Bytes) del mensaje =  ");
                  Serial.println(L12, DEC); 

                  // %%%%%%%%%% PARA HORA 13 %%%%%%%%%%%%%%%%%%5

                  //String command = nmeaCommand(out);
                  client.sendData(command); 

                  Serial.print("Comando Enviado: ");          
                  Serial.println(command);  
                  Serial.print("Tamaño (En Bytes) del mensaje =  ");
                  Serial.println(L13, DEC);    

              
                          
                    }   
    
       
}



