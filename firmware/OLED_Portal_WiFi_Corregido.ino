#include <WiFi.h>
#include <WiFiManager.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <time.h>
#include <ctype.h>

// Configura solo las credenciales de la cuenta del DISPOSITIVO (lectura).
// Cambia su contrasena en Firebase antes de usar este firmware.
const char* API_KEY = "AIzaSyA95LxLIPW_IOwjwy5fkV4l6D4uOR7EnYE";
const char* DEVICE_EMAIL = "mejiv4753@gmail.com";
const char* DEVICE_PASSWORD = "CAMBIA_ESTA_CONTRASENA_EN_TU_COPIA_LOCAL";
const char* DATABASE_URL = "https://proyectoj-49e24-default-rtdb.firebaseio.com";

// Raices de confianza publicas para HTTPS (pueden requerir actualizacion
// si Google modifica la cadena de certificados de sus servicios).
static const char FIREBASE_ROOT_CA[] PROGMEM = R"CERT(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIIFVzCCAz+gAwIBAgINAgPlk28xsBNJiGuiFzANBgkqhkiG9w0BAQwFADBHMQsw
CQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExMQzEU
MBIGA1UEAxMLR1RTIFJvb3QgUjEwHhcNMTYwNjIyMDAwMDAwWhcNMzYwNjIyMDAw
MDAwWjBHMQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZp
Y2VzIExMQzEUMBIGA1UEAxMLR1RTIFJvb3QgUjEwggIiMA0GCSqGSIb3DQEBAQUA
A4ICDwAwggIKAoICAQC2EQKLHuOhd5s73L+UPreVp0A8of2C+X0yBoJx9vaMf/vo
27xqLpeXo4xL+Sv2sfnOhB2x+cWX3u+58qPpvBKJXqeqUqv4IyfLpLGcY9vXmX7w
Cl7raKb0xlpHDU0QM+NOsROjyBhsS+z8CZDfnWQpJSMHobTSPS5g4M/SCYe7zUjw
TcLCeoiKu7rPWRnWr4+wB7CeMfGCwcDfLqZtbBkOtdh+JhpFAz2weaSUKK0Pfybl
qAj+lug8aJRT7oM6iCsVlgmy4HqMLnXWnOunVmSPlk9orj2XwoSPwLxAwAtcvfaH
szVsrBhQf4TgTM2S0yDpM7xSma8ytSmzJSq0SPly4cpk9+aCEI3oncKKiPo4Zor8
Y/kB+Xj9e1x3+naH+uzfsQ55lVe0vSbv1gHR6xYKu44LtcXFilWr06zqkUspzBmk
MiVOKvFlRNACzqrOSbTqn3yDsEB750Orp2yjj32JgfpMpf/VjsPOS+C12LOORc92
wO1AK/1TD7Cn1TsNsYqiA94xrcx36m97PtbfkSIS5r762DL8EGMUUXLeXdYWk70p
aDPvOmbsB4om3xPXV2V4J95eSRQAogB/mqghtqmxlbCluQ0WEdrHbEg8QOB+DVrN
VjzRlwW5y0vtOUucxD/SVRNuJLDWcfr0wbrM7Rv1/oFB2ACYPTrIrnqYNxgFlQID
AQABo0IwQDAOBgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4E
FgQU5K8rJnEaK0gnhS9SZizv8IkTcT4wDQYJKoZIhvcNAQEMBQADggIBAJ+qQibb
C5u+/x6Wki4+omVKapi6Ist9wTrYggoGxval3sBOh2Z5ofmmWJyq+bXmYOfg6LEe
QkEzCzc9zolwFcq1JKjPa7XSQCGYzyI0zzvFIoTgxQ6KfF2I5DUkzps+GlQebtuy
h6f88/qBVRRiClmpIgUxPoLW7ttXNLwzldMXG+gnoot7TiYaelpkttGsN/H9oPM4
7HLwEXWdyzRSjeZ2axfG34arJ45JK3VmgRAhpuo+9K4l/3wV3s6MJT/KYnAK9y8J
ZgfIPxz88NtFMN9iiMG1D53Dn0reWVlHxYciNuaCp+0KueIHoI17eko8cdLiA6Ef
MgfdG+RCzgwARWGAtQsgWSl4vflVy2PFPEz0tv/bal8xa5meLMFrUKTX5hgUvYU/
Z6tGn6D/Qqc6f1zLXbBwHSs09dR2CQzreExZBfMzQsNhFRAbd03OIozUhfJFfbdT
6u9AWpQKXCBfTkBdYiJ23//OYb2MI3jSNwLgjt7RETeJ9r/tSQdirpLsQBqvFAnZ
0E6yove+7u7Y/9waLd64NnHi/Hm3lCXRSHNboTXns5lndcEZOitHTtNCjv0xyBZm
2tIMPNuzjsmhDYAPexZ3FL//2wmUspO8IFgV6dtxQ/PeEMMA3KgqlbbC1j+Qa3bb
bP6MvPJwNQzcmRk13NfIRmPVNnGuV/u3gm3c
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIICCTCCAY6gAwIBAgINAgPluILrIPglJ209ZjAKBggqhkjOPQQDAzBHMQswCQYD
VQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExMQzEUMBIG
A1UEAxMLR1RTIFJvb3QgUjMwHhcNMTYwNjIyMDAwMDAwWhcNMzYwNjIyMDAwMDAw
WjBHMQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2Vz
IExMQzEUMBIGA1UEAxMLR1RTIFJvb3QgUjMwdjAQBgcqhkjOPQIBBgUrgQQAIgNi
AAQfTzOHMymKoYTey8chWEGJ6ladK0uFxh1MJ7x/JlFyb+Kf1qPKzEUURout736G
jOyxfi//qXGdGIRFBEFVbivqJn+7kAHjSxm65FSWRQmx1WyRRK2EE46ajA2ADDL2
4CejQjBAMA4GA1UdDwEB/wQEAwIBhjAPBgNVHRMBAf8EBTADAQH/MB0GA1UdDgQW
BBTB8Sa6oC2uhYHP0/EqEr24Cmf9vDAKBggqhkjOPQQDAwNpADBmAjEA9uEglRR7
VKOQFhG/hMjqb2sXnh5GmCCbn9MN2azTL818+FsuVbu/3ZL3pAzcMeGiAjEA/Jdm
ZuVDFhOD3cffL74UOO0BzrEXGhF16b0DjyZ+hOXJYKaV11RZt+cRLInUue4X
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIICCTCCAY6gAwIBAgINAgPlwGjvYxqccpBQUjAKBggqhkjOPQQDAzBHMQswCQYD
VQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExMQzEUMBIG
A1UEAxMLR1RTIFJvb3QgUjQwHhcNMTYwNjIyMDAwMDAwWhcNMzYwNjIyMDAwMDAw
WjBHMQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2Vz
IExMQzEUMBIGA1UEAxMLR1RTIFJvb3QgUjQwdjAQBgcqhkjOPQIBBgUrgQQAIgNi
AATzdHOnaItgrkO4NcWBMHtLSZ37wWHO5t5GvWvVYRg1rkDdc/eJkTBa6zzuhXyi
QHY7qca4R9gq55KRanPpsXI5nymfopjTX15YhmUPoYRlBtHci8nHc8iMai/lxKvR
HYqjQjBAMA4GA1UdDwEB/wQEAwIBhjAPBgNVHRMBAf8EBTADAQH/MB0GA1UdDgQW
BBSATNbrdP9JNqPV2Py1PsVq8JQdjDAKBggqhkjOPQQDAwNpADBmAjEA6ED/g94D
9J+uHXqnLrmvT/aDHQ4thQEd0dlq7A/Cr8deVl5c1RxYIigL9zC2L7F8AjEA8GE8
p/SgguMh1YQdc4acLa/KNJvxn7kjNuK8YAOdgLOaVsjh4rsUecrNIdSUtUlD
-----END CERTIFICATE-----
)CERT";

Adafruit_SSD1306 display(128,64,&Wire,-1);
WiFiManager wm;
String idToken, refreshToken, lastHex;
unsigned long tokenExpires=0,lastPoll=0,lastLoginAttempt=0,lastWiFiLost=0;
bool clockReady=false;
bool ntpStarted=false;
unsigned long lastNtpReport=0;

void statusOLED(const String &a,const String &b="",const String &c="") {
  display.clearDisplay();display.setTextSize(1);display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);display.println(a);
  display.setCursor(0,20);display.println(b);
  display.setCursor(0,40);display.println(c);display.display();
}
void configureTLS(WiFiClientSecure &client) {
  client.setCACert(FIREBASE_ROOT_CA); // Verificacion del certificado ACTIVADA
  client.setTimeout(8000);
}
void setupWiFi() {
  WiFi.mode(WIFI_STA);
  wm.setConnectTimeout(15);
  wm.setConfigPortalTimeout(0); // Portal disponible hasta completar configuracion
  statusOLED("Configura Wi-Fi", "Red: OLED-Config", "192.168.4.1");

  Serial.println("WiFiManager: intentando red guardada o portal OLED-Config");
  // Modo bloqueante: mas estable para el aprovisionamiento inicial.
  // La pantalla mostrara las instrucciones mientras espera.
  wm.autoConnect("OLED-Config","ConfigOLED123");
  if(WiFi.status()==WL_CONNECTED) {
    Serial.print("Wi-Fi conectado. IP: ");Serial.println(WiFi.localIP());
    statusOLED("Wi-Fi conectado", "Sincronizando...");
    clockReady=false;ntpStarted=false;lastNtpReport=0;idToken="";lastLoginAttempt=0;lastWiFiLost=0;
  }
}
bool postJson(const String &url,const String &body,JsonDocument &response) {
  WiFiClientSecure client;configureTLS(client);HTTPClient http;
  if(!http.begin(client,url))return false;
  http.setTimeout(8000);http.addHeader("Content-Type","application/json");
  int code=http.POST(body);String payload=http.getString();http.end();
  if(code<200||code>=300){Serial.printf("POST HTTP %d: %s\n",code,payload.c_str());return false;}
  return deserializeJson(response,payload)==DeserializationError::Ok;
}
bool login() {
  JsonDocument request,response;
  request["email"]=DEVICE_EMAIL;request["password"]=DEVICE_PASSWORD;
  request["returnSecureToken"]=true;
  String body;serializeJson(request,body);
  String url=String("https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=")+API_KEY;
  if(!postJson(url,body,response))return false;
  idToken=response["idToken"].as<String>();refreshToken=response["refreshToken"].as<String>();
  unsigned long expires=response["expiresIn"].as<unsigned long>();if(expires<120)expires=3600;
  tokenExpires=millis()+(expires-60)*1000UL;
  Serial.println("Firebase autenticado");return !idToken.isEmpty();
}
bool refreshLogin() {
  if(refreshToken.isEmpty())return false;
  WiFiClientSecure client;configureTLS(client);HTTPClient http;
  String url=String("https://securetoken.googleapis.com/v1/token?key=")+API_KEY;
  if(!http.begin(client,url))return false;
  http.setTimeout(8000);http.addHeader("Content-Type","application/x-www-form-urlencoded");
  String body="grant_type=refresh_token&refresh_token="+refreshToken;
  int code=http.POST(body);String payload=http.getString();http.end();
  if(code!=200){Serial.printf("Refresh HTTP %d\n",code);return false;}
  JsonDocument data;if(deserializeJson(data,payload))return false;
  idToken=data["id_token"].as<String>();refreshToken=data["refresh_token"].as<String>();
  unsigned long expires=data["expires_in"].as<unsigned long>();if(expires<120)expires=3600;
  tokenExpires=millis()+(expires-60)*1000UL;
  return !idToken.isEmpty();
}
bool showHex(const String &hex) {
  if(hex.length()!=2048)return false;
  uint8_t buffer[1024];
  for(int i=0;i<1024;i++) {
    char a=hex[2*i],b=hex[2*i+1];
    if(!isxdigit((unsigned char)a)||!isxdigit((unsigned char)b))return false;
    char pair[3]={a,b,0};buffer[i]=(uint8_t)strtoul(pair,nullptr,16);
  }
  display.clearDisplay();
  for(int y=0;y<64;y++)for(int x=0;x<128;x++)
    if(buffer[x+(y/8)*128]&(1<<(y%8)))display.drawPixel(x,y,SSD1306_WHITE);
  display.display();return true;
}
void pollFrame() {
  WiFiClientSecure client;configureTLS(client);HTTPClient http;
  String url=String(DATABASE_URL)+"/oled/frame.json?auth="+idToken;
  if(!http.begin(client,url))return;
  http.setTimeout(8000);
  int code=http.GET();String payload=http.getString();http.end();
  if(code==401||code==403){idToken="";Serial.println("Token rechazado");return;}
  if(code!=200){Serial.printf("GET HTTP %d\n",code);return;}
  JsonDocument data;
  if(deserializeJson(data,payload)){Serial.println("JSON invalido");return;}
  String hex=data["data"].as<String>();
  if(hex.length()==2048&&hex!=lastHex)
    if(showHex(hex)){lastHex=hex;Serial.println("OLED actualizada");}
}
void setup() {
  Serial.begin(115200);Wire.begin(21,22);
  if(!display.begin(SSD1306_SWITCHCAPVCC,0x3C)){
    Serial.println("OLED no detectada");while(true)delay(1000);
  }
  statusOLED("Iniciando...");
  setupWiFi();
}
void loop() {
  if(WiFi.status()!=WL_CONNECTED) {
    if(lastWiFiLost==0){lastWiFiLost=millis();Serial.println("Wi-Fi perdido");}
    if(millis()-lastWiFiLost>30000UL) {
      lastWiFiLost=0;
      setupWiFi(); // Si no puede conectar, portal de configuracion
    } else {WiFi.reconnect();delay(1000);}
    return;
  }
  lastWiFiLost=0;
  if(!clockReady) {
    if(!ntpStarted) {
      Serial.println("Iniciando sincronizacion NTP...");
      statusOLED("Wi-Fi conectado", "Sincronizando hora...");
      configTime(0, 0, "pool.ntp.org", "time.google.com", "time.cloudflare.com");
      ntpStarted=true;
    }
    time_t now=time(nullptr);
    if(now>=1700000000) {
      clockReady=true;
      Serial.println("Hora sincronizada correctamente");
      Serial.println(ctime(&now));
      statusOLED("Wi-Fi conectado", "Hora sincronizada", "Conectando Firebase");
    } else {
      if(lastNtpReport==0 || millis()-lastNtpReport>=5000UL) {
        lastNtpReport=millis();
        Serial.printf("Esperando hora NTP... Timestamp actual: %ld\n", (long)now);
      }
      delay(100);
      return;
    }
  }
  if(idToken.isEmpty()||(long)(millis()-tokenExpires)>=0) {
    if(lastLoginAttempt==0||millis()-lastLoginAttempt>5000UL) {
      lastLoginAttempt=millis();
      if(!refreshLogin()){idToken="";login();}
    }
    delay(50);return;
  }
  if(millis()-lastPoll>=1000UL){lastPoll=millis();pollFrame();}
  delay(10);
}
