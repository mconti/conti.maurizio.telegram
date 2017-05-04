
/*
  BotLamp.
  
  Esempio di IoT basato su telegram
  maurizio.conti@fablabromagna.org 
  e ivan.tarozzi@fablabromagna.org
  
  per
  School Maker Day 2017
  5 maggio 2017 - Opificio Golinelli
   
*/
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// parametri di connessione
char ssid[] = "IoT";     
char password[] = "Ivan2017!"; 

// Telegram bot Token preso con Botfather
#define TelegramToken "250906422:AAFBa2OI23FXqa_OK976QguOHYJtxzbddx0"  

WiFiClientSecure client;
UniversalTelegramBot bot(TelegramToken, client);

// Intervallo di polling
int POLLING_TIME = 1000; 

// prossima quota alla quale scatta il controllo
long next_tick;   
#define LED_PIN 4

void setup() {
  Serial.begin(115200);

  // Forziamo la procedura di disconnessione dall'AP
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Ci colleghiamo
  Serial.print("Mi connetto alla rete: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connesso");
  Serial.print("Ho avuto questo IP address: ");
  Serial.println(WiFi.localIP());
  
  // volendo si può usare LED_BUILTIN
  // ma ricordarsi che lavora in logica negativa...
  
  pinMode( LED_PIN, OUTPUT );
  digitalWrite( LED_PIN, HIGH);  // Accende il led
  delay(300);  
  digitalWrite( LED_PIN, LOW); // Spegne il led
  delay(300);  
  digitalWrite( LED_PIN, HIGH);  // Accende il led
  delay(300);  
  digitalWrite( LED_PIN, LOW); // Spegne il led
  delay(300);  
}

int lastId = 0;

void loop() {
  
  if ( millis() > next_tick )  {

    /// Driiiin !
    // Il timer è scattato... si parte!
    Serial.print( "L'ultimo id: " );
    Serial.println( lastId );

    // Per prima cosa, prelevo i messaggi...
    int numNewMessages = bot.getUpdates( lastId );

    Serial.print( "Ho trovato " );
    Serial.print( numNewMessages );
    Serial.println( " msg da leggere." );

    for (int i=0; i<numNewMessages; i++) {
      String msg = bot.messages[i].text;
      lastId = bot.last_message_received + 1;
      
      if( msg == "/ledoff" ) {
        bot.sendMessage(bot.messages[i].chat_id, "Spento", "");
        digitalWrite( LED_PIN, LOW); 
      }
      
      if( msg == "/ledon" ) {
        bot.sendMessage(bot.messages[i].chat_id, "Acceso", "");
        digitalWrite( LED_PIN, HIGH); 
      }

      if( msg == "/status" ) {
        if( digitalRead( LED_PIN ) == HIGH ) 
          bot.sendMessage(bot.messages[i].chat_id, "Acceso (letto dal campo)", "");
        else
          bot.sendMessage(bot.messages[i].chat_id, "Spento (letto dal campo)", "");
      }

      if( msg == "/temp" ) {
        int tempVal = analogRead( A0 ); 
        bot.sendMessage(bot.messages[i].chat_id, "Temp: " + String(tempVal), "");
      }

      if( msg == "/start" ) {
        bot.sendMessage(bot.messages[i].chat_id, "Comandi disponibili:\n/status\n/ledon\n/ledoff\n/temp", "");
      }
    }
    
    // ricarica il timer della stufa...
    next_tick = millis() + POLLING_TIME ;
  }
}
