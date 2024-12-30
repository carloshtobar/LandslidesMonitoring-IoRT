//Monitorización de deslizamientos de tierra en el Macizo Colombiano del sur del departamento del Cauca mediante IoRT
//TESIS GRANOBLE
//By: Cristian Camilo Granoble Gaviria - Luis Alejandro Granoble Gaviria

//MODO 1: TRANSMISION 24 HORAS (24/7)

/////// Librerias

  #include <TimeLib.h>
  #include <Arduino.h>
  #include <WiFi.h>
  #include <WiFiMulti.h>
  #include <SoftwareSerial.h>
  #include "TelnetClient.h"
  #include <OneWire.h>           //Librerias para sensor de temperatura de suelo
  #include <DallasTemperature.h> //Librerias para sensor de temperatura de suelo

  
  #define pinData 32        // para sensor de temperatura de suelo
  OneWire ourWire(pinData); // para sensor de temperatura de suelo
  DallasTemperature sensors(&ourWire); // para sensor de temperatura de suelo
  #include<ArduinoJson.h>   //Librerias para Json
  #include "DHT.h"          // Para sensor de temperatura y humedad relativa

// DEFINICION DE PINES

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

///////// Configuracion Telnet ///////////

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

    dht.begin(); // Para iniciar DHT
    sensors.begin(); // Para iniciar sensor de temperatura de suelo    
    
    
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

  setTime(14,00,05,15,9,2024);
  if (timeStatus()!= timeSet){
     Serial.println("No se logro sincronizar");
  }else{
     Serial.println("Tenemos el reloj sincronizado");
  }


}


        // VARIABLES PARA PROMEDIOS

          float promedio1;    //PROMEDIO HUMEDAD AMBIENTE
          float promedio2;    //PROMEDIO TEMPERATURA AMBIENTE
          float promedio3;    //PROMEDIO HUMEDAD DEL SUELO
          float promedio4;    //PROMEDIO TEMPERATURADEL SUELO
          float promedio5;    //PROMEDIO LLUVIA

        // VARIABLES PARA LA SUMA DEL PROMEDIO

          float suma1 = 0;    //PROMEDIO HUMEDAD AMBIENTE
          float suma2 = 0;    //PROMEDIO TEMPERATURA AMBIENTE
          float suma3 = 0;    //PROMEDIO HUMEDAD DEL SUELO
          float suma4 = 0;    //PROMEDIO TEMPERATURADEL SUELO
          float suma5 = 0;    //PROMEDIO LLUVIA
          float c = 0;

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

      // Realzamos un ciclo for, para que tome los datos de los sensores cada 10 minutos (en este caso 6 cada 10 mintuos hace 1 hora)
      // toma los datos cada 10 minutos y los prmedia. En cada hora toma 6 veces los datos y se promedia, este resultado se envia.

      for(int K = 1; K<=6; K++)
        {
  
          sensors.requestTemperatures();       // para sensor de temperatura de suelo  ////
          //Serial.print(sensors.getTempCByIndex(0)); // para sensor de temperatura de suelo


          int lectura_lluvia = analogRead(35);  //  para declarar y leer pin analogo A0    
          int porcentaje_lluvia = map(lectura_lluvia, 3500, 800, 0, 100);
  
  
          int lectura_hum_suelo = analogRead(34); //  para declarar y leer pin analogo A0
          int porcentaje_hum_suelo = map(lectura_hum_suelo, 4500, 2000, 0, 100);
  
          //StaticJsonDocument<256> doc;  //Declaro la funcion, 256 define la cantidad de memoria ue quiero ocupar
    
        //INICIALIZACION DHT22

          float h = dht.readHumidity();         //Leemos la Humedad
          float t = dht.readTemperature();      //Leemos la temperatura en grados Celsius
          float f = dht.readTemperature(true);  //Leemos la temperatura en grados Fahrenheit

                      
        // REALIZACION DE SUMA DE CADA SENSOR

                suma1 = suma1 + h;                            // Suma para Humedad
                suma2 = suma2 + t;                            //Suma para temperatura
                suma3 = suma3 + porcentaje_hum_suelo;         //Suma para humedad del suelo
                suma4 = suma4 + sensors.getTempCByIndex(0);   //Suma para Temperatura suelo
                suma5 = suma5 + porcentaje_lluvia;            //Suma para la precipitacion
                //c++
                //suma5 = suma5 + c;
  
                // IMPRIMIMOS EL VALOR DE CADA SUMA Y EL VALOR DE CADA SENSOR

                Serial.println();
                Serial.print("................... Lectura: Numero ");
                Serial.print(K);
                Serial.print("  ...................");
                Serial.println();

                // SENSOR DE HUMEDAD RELATIVA

                Serial.println();
                Serial.print("----------------------------------------------------------");
                Serial.println();

                //client.wait4Data();

                Serial.println();
                Serial.print("> Valor: Humedad relativa ");
                Serial.print(K);
                Serial.print(" = ");
                Serial.print(h, DEC); 
                Serial.println(" % ");
                //Serial.println();

                Serial.print("> Suma: Humedad relativa ");
                Serial.print(K);
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
                Serial.print(K);
                Serial.print(" = ");
                Serial.print(t, DEC);
                Serial.println(" C "); 
                //Serial.println();

                Serial.print("> Suma: Temperatura ambiente ");
                Serial.print(K);
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
                Serial.print(K);
                Serial.print(" = ");
                Serial.print(porcentaje_hum_suelo, DEC);
                Serial.println(" % "); 
                //Serial.println();

                Serial.print("> Suma: Humedad superficial del suelo ");
                Serial.print(K);
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
                Serial.print(K);
                Serial.print(" = ");
                Serial.print(sensors.getTempCByIndex(0), DEC); 
                Serial.println(" C ");
                //Serial.println();

                Serial.print("> Suma: Temperatura superficial del suelo ");
                Serial.print(K);
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
                Serial.print(K);
                Serial.print(" = 0 ");
                //Serial.print(porcentaje_lluvia, DEC); 
                Serial.println(" % ");
                //Serial.println();

                Serial.print("> Suma: Precipitacion ");
                Serial.print(K);
                Serial.print(" = 0");
                //Serial.print(K);
                //Serial.print(suma5, DEC); 
                Serial.println(" % ");
                Serial.println();

                //client.wait4Data();

                //Serial.println();
                Serial.print("----------------------------------------------------------");
                Serial.println();
                Serial.println();

                // PAUSA DE 10 MINUTOS

               /* Serial.println();
                Serial.print("............... Lectura: Numero ");
                Serial.print(K);
                Serial.print("  ...............");
                Serial.println();*/

               delay(600000); // 60000 ES 1 MINUTO, PERO DEBEMOS POPNER 10 MIN = 600000 milisegundos
                //delay(5000); // 10000 ms = 10 seg
              
        }      

        // Promedios

            promedio1 = suma1/6;
            promedio2 = suma2/6;
            promedio3 = suma3/6;
            promedio4 = suma4/6;
            promedio5 = suma5/6;
            

        //REINICIAMOS SUMA

            suma1=0;
            suma2=0;
            suma3=0;
            suma4=0;
            suma5=0;

            ////////////// IMPRIMIMOS PROMEDIO /////////////////////

            //Promedio Humedad Relativa

            Serial.println("..........................................................");
            Serial.print("....................... PROMEDIOS ........................");
            Serial.println();
            Serial.println();

            Serial.print("- Promedio: Humedad relativa =  ");
            Serial.print(promedio1, DEC); 
            Serial.println(" % ");
            Serial.println();

            // Promedio Temperatura ambiente

            Serial.print("- Promedio: Temperatura ambiente =  ");
            Serial.print(promedio2, DEC); 
            Serial.println(" C ");
            Serial.println();

            // Promedio Humeda del suelo

            Serial.print("- Promedio: Humedad superficial del suelo =  ");
            Serial.print(promedio3, DEC); 
            Serial.println(" % ");
            Serial.println();

            // Promedio emperatura del suelo

            Serial.print("- Promedio: Temperatura superficial del suelo =  ");
            Serial.print(promedio4, DEC);
            Serial.println(" C "); 
            Serial.println();

            // Promedio precipitacion

            Serial.print("- Promedio: Precipitacion =  ");
            Serial.print(promedio5, DEC);
            Serial.println(" % "); 
            //Serial.println();

            //Serial.println();
            Serial.println("..........................................................");
            Serial.println("..........................................................");
            Serial.println();


  
  // ********** SERIALIZAMOS (CONCEPTO USADO PARA PASAR DATOS A FORMATO JSON) **********  //

            doc["Hum.R: "] = promedio1 ; //////
            doc["Temp.A: "] = promedio2 ; //////
            //doc["TEMP.RELATIVA (Fº): "] = f ;
            doc["Hum.S: "] = promedio3; //////
            doc["Temp.S: "] = promedio4; /////
            doc["Lluvia: "] = promedio5; ////////
            doc["Hora:"] = horario;
            doc["Fecha:"] = fecha;   

            //DELAY PARA HACER PRUEBAS

            //delay(15000);
            
            //if(lectura_lluvia == 1 ){ doc["LLUVIA: "] = "0"; } else doc["LLUVIA: "] = "1";   
            
            char out[256];     // Defino una variable de salida y le asigno cantidade de memoria en bytes
            int L = serializeJson(doc, out);    //L contiene la cantidad de bytes, out ya está en JSON

  
            int entrada = digitalRead(CONTROL);   
          // int k = 0;  
            

            // Controlamos  el envio, cada vez que presionemos un boton 
            if(entrada == LOW)
            {

                //  String command = nmeaCommand("Prueba inicial!");
                //  client.sendData(command); 
                  String command = nmeaCommand(out);
                  client.sendData(command); 

                  Serial.print("Comando Enviado: ");          
                  Serial.println(command);  
                  Serial.print("Tamaño (En Bytes) del mensaje =  ");
                  Serial.println(L, DEC);      

              // HACEMOS LA HORA, SUMANDO CON CADA DELAY 1 MINUTOS, HAY 10 DELAY, HACEMOS 10 MIN, Y SE REPITE 6 VECES CON EL FOR, PARA COMPLETAR LA HORA
         
                  /*
                  for(int i = 0; i<60; i++)
                  { 
                    delay(1000);       //suma 1 minutos
                    client.wait4Data();
                    
                    delay(1000);       //suma 1 minutos
                    client.wait4Data();
                  
                    delay(1000);       //suma 1 minutos
                    client.wait4Data();
                    
                    delay(1000);       //suma 1 minutos
                    client.wait4Data();
                    
                  delay(1000);       //suma 1 minutos
                    client.wait4Data();
                    
                    delay(1000);       //suma 1 minutos
                    client.wait4Data();
                    
                    delay(1000);       //suma 1 minutos
                    client.wait4Data();
                    
                    delay(1000);
                    client.wait4Data();
                    
                    delay(1000);
                    client.wait4Data();
                    
                    delay(1000);
                    client.wait4Data();

                    delay(1000);       //suma 1 minutos
                    client.wait4Data();
                    
                    delay(1000);       //suma 1 minutos
                    client.wait4Data();
                  
                    delay(1000);       //suma 1 minutos
                    client.wait4Data();
                    
                    delay(1000);       //suma 1 minutos
                    client.wait4Data();
                    
                  delay(1000);       //suma 1 minutos
                    client.wait4Data();
                    
                    delay(1000);       //suma 1 minutos
                    client.wait4Data();
                    
                    delay(1000);       //suma 1 minutos
                    client.wait4Data();
                    
                    delay(1000);
                    client.wait4Data();
                    
                    delay(1000);
                    client.wait4Data();
                    
                    delay(1000);
                    client.wait4Data();
                    
                  }

                  */
            }

                    else 
                    {
                    
                      client.wait4Data();


                                                
                    }   
    
       
}



