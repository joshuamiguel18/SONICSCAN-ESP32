#include "FS.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <PN5180.h>
#include <PN5180ISO15693.h>
#include <WebServer.h>
#include <EEPROM.h>
#include <TFT_eSPI.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <TinyGPSPlus.h>
#include <DNSServer.h>
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include "Free_Fonts.h"

#define RXD2 16
#define TXD2 17

TinyGPSPlus gps;



#define EEPROM_SIZE 96


HardwareSerial ss(1);


TFT_eSPI tft = TFT_eSPI();
uint16_t t_x = 0, t_y = 0;



#include "NotoSansBold15.h"
#include "NotoSansBold36.h"

#define AA_FONT_SMALL NotoSansBold15
#define AA_FONT_LARGE NotoSansBold36

int page = 0;
float subPage = 0.0;

// This is the file name used to store the calibration data
// You can change this to create new calibration files.
// The SPIFFS file name must start with "/".
#define CALIBRATION_FILE "/TouchCalData1"

// Set REPEAT_CAL to true instead of false to run calibration
// again, otherwise it will only be done once.
// Repeat calibration if you change the screen rotation.
#define REPEAT_CAL false

// long wifi_interval = 500;

int i = 0;
// int statusCode;
const char* ssid = "text";
const char* passphrase = "text";
String st;
String content;

#define TFT_GREY 0x8410
#define TFT_LIGHTBLUE 0xADD8
bool testWifi(void);
void launchWeb(void);
void setupAP(void);

// WebServer server(80);
DNSServer dnsServer;
AsyncWebServer server(80);

HTTPClient httpClient;


int radius = 10;  // Radius of the curved corners
int textSize = 2;
int variableNameSize = 1;

// Buffers to store the string representations of latitude and longitude
char latBuffer[12];
char lngBuffer[12];

// Define variables to store latitude and longitude
float latitude = 0.0;
float longitude = 0.0;



#define BUZZER 12
#define LCDSCREEN 32
#define TIME_OUT_SLEEP 180000  // 3 minutes OF INACTIVITY ESP32 WILL TURN OFF
bool pressed;

bool gotLocationAlready = false;




#define ALERT_DURATION 8000  // Alert duration in milliseconds (8 seconds)
#define BUZZER_ON_TIME 500   // Buzzer on duration in milliseconds
#define BUZZER_OFF_TIME 500  // Buzzer off duration in milliseconds

unsigned long alertStartTime = 0;
bool isAlertActive = false;
bool isBuzzerOn = false;
bool stolenBuzz = false;      // Initially set to false, will be set to true when alert starts
unsigned long previousMillis = 0;



// Each PN5180 reader requires unique NSS, BUSY, and RESET pins,
// as defined in the constructor below
#define PN5180_NSS 26  // 16 // 3 // 25
#define PN5180_BUSY 21
#define PN5180_RST 25  // 17  // 1 //26
PN5180ISO15693 nfc(PN5180_NSS, PN5180_BUSY, PN5180_RST);



char scanner_name[20] =  "Silver";



// Define button constants
#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 50
#define BUTTON_BORDER_THICKNESS 4
#define BUTTON_MARGIN 20
#define BUTTON_COLOR TFT_BLUE
#define BUTTON_TEXT_COLOR TFT_WHITE
#define BUTTON_BORDER_COLOR TFT_WHITE



// WIFI ANIMATION DEFINES
#define WIFI_ICON_X 120  // X position of the Wi-Fi icon
#define WIFI_ICON_Y 350  // Y position of the Wi-Fi icon
#define WIFI_ICON_WIDTH 80  // Width of the Wi-Fi icon
#define WIFI_ICON_HEIGHT 60  // Maximum height of the Wi-Fi icon
#define WIFI_COLOR TFT_WHITE  // Color of the Wi-Fi icon
#define WIFI_BACKGROUND TFT_BLACK  // Background color of the Wi-Fi icon


//Start of OR/CR
int id = 0;
char uidTag[50] = "";
char status[50] = "";
char field_office[50] = "";
char field_code[50] = "";
char cr_no[50] = "";
char date[50] = "";
char plate_no[50] = "";
char engine_no[50] = "";
char chassis_no[50] = "";
char vin[50] = "";
char file_no[50] = "";
char vehicle_type[50] = "";
char vehicle_category[50] = "";
char make_brand[50] = "";
char body_type[50] = "";
char series[50] = "";
char gross_weight[50] = "";
char net_weight[50] = "";
char year_model[50] = "";
char year_rebuilt[50] = "";
char piston_displacement[50] = "";
char max_power[50] = "";
char passenger_capacity[50] = "";
char color[50] = "";
char type_of_fuel[50] = "";
char registration_classification[50] = "";
char owners_name[50] = "";
char owners_address[50] = "";
char encumbered_to[50] = "";
char details_of_first_registration[50] = "";
char or_no[50] = "";
char or_date[50] = "";
char amount[50] = "";
char remarks[50] = "";
// Start of Driver's license
char first_name[50] = "";
char last_name[50] = "";
char birthday[50] = "";
char sex[30] = ""; 
char nationality[30] = "";
char licensenumber[50] = "";
char weight[20] = "";
char height[20] = "";
char expdate[50] = "";
char licensetype[50] = "";
char address[100] = "";
char agencycode[30] = "";
char blood[20] = "";
char eye[30] = "";
char condition[100] = "";

unsigned long lastButtonAndScreenPressTime = 0;  // Variable to store the time of last button press or screen touched
unsigned long lastScreenTouchTime = 0;


// Variable to store the previous UID read by this reader
int readsuccess;
String UID_Result = "";
uint8_t uid[8];
byte previousUID[8] = { 0 };
unsigned long lastUIDReadTime = 0;
const unsigned long UID_CLEAR_INTERVAL = 5000;  // 10 seconds


// variable for reading the pushbutton status
const int triggerBtnAndWakeUp = 34;
int triggerState = 0;




bool singleClickDetected = false;  // Flag to indicate a single click
bool deepSleepTriggered = false;   // Flag to indicate if deep sleep is triggered




//OLD CODE
int getUID() {
  // Variable to store the ID of any tag read by this reader
  byte uid[8];
  UID_Result = "";

  // Try to read a tag ID (or "get inventory" in ISO15693-speak)
  memset(uid, 0, sizeof(uid));
  ISO15693ErrorCode rc = nfc.getInventory(uid);
  Serial.println("RC:" + String(rc));
  Serial.println("Scanned a card 1");
  // If the result code was that a card had been read
  if (rc == ISO15693_EC_OK) {
    Serial.println("Scanned a card 2");
    // If this is the same ID as we read last frame, do nothing
    // if (memcmp(previousUID, uid, 8) == 0) {
    //   // Check if it's time to clear the previous UID

    if (millis() - lastUIDReadTime >= UID_CLEAR_INTERVAL) {
      // Serial.println("Removed a card");
      // memset(previousUID, 0, sizeof(previousUID));

      Serial.print(F("Inventory successful, UID="));
      for (int i = 0; i < 8; i++) {
        UID_Result += String(uid[7 - i], HEX);
        Serial.print(uid[7 - i], HEX);  // LSB is first
      }
      Serial.println();
      Serial.println(F("----------------------------------"));
      memset(uid, 0, sizeof(uid));
      nfc.reset();
      nfc.setupRF();
      return 1;  // New card detected
    }
    nfc.reset();
    nfc.setupRF();
    return 0;  // No new card detected

    // Update the previous UID with the current UID
    // memcpy(previousUID, uid, 8);
    // lastUIDReadTime = millis();

  } else {
    // If a card cannot be read
    nfc.reset();
    nfc.setupRF();
    return 0;
  }
}



const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>SonicScan Captive Portal</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
      background: linear-gradient(to right, #6a11cb, #2575fc);
      color: #fff;
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
      margin: 0;
      padding: 20px;
      box-sizing: border-box;
    }
    .container {
      text-align: center;
      width: min(calc(100% - 15px), 400px);
      background: rgba(255, 255, 255, 0.1);
      backdrop-filter: blur(10px);
      padding: 30px;
      border-radius: 12px;
      box-shadow: 0 4px 30px rgba(0, 0, 0, 0.1);
    }
    h1 {
      font-size: 28px;
      margin-bottom: 10px;
    }
    h2 {
      font-size: 20px;
      margin-bottom: 20px;
      font-weight: normal;
      color: #eee;
    }
    form {
      text-align: left;
    }
    label {
      display: block;
      margin-bottom: 8px;
      font-weight: bold;
    }
    input[type="text"], input[type="password"] {
      width: 100%;
      padding: 12px;
      margin-bottom: 20px;
      border: none;
      border-radius: 6px;
      box-sizing: border-box;
      font-size: 16px;
    }
    input[type="submit"] {
      background-color: #4CAF50;
      color: white;
      padding: 14px;
      margin-top: 10px;
      border: none;
      border-radius: 6px;
      cursor: pointer;
      font-size: 16px;
      transition: background-color 0.3s ease;
      width: 100%;
    }
    input[type="submit"]:hover {
      background-color: #45a049;
    }
  </style>
</head><body>
  <div class="container">
    <h1>Welcome to SonicScan Captive Portal</h1>
    <h2>Enter your WiFi credentials</h2>
    <form action="/save">
      <label for="ssid">SSID:</label>
      <input type="text" id="ssid" name="ssid" required>
      <label for="password">Password:</label>
      <input type="password" id="password" name="password" required>
      <input type="submit" value="Submit">
    </form>
  </div>
</body></html>)rawliteral";




class CaptiveRequestHandler : public AsyncWebHandler {
public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request) {
    return true;
  }

  void handleRequest(AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  }
};





void startBuzzer() {
  digitalWrite(BUZZER, HIGH); // Turn on the buzzer
  delay(100); // Wait for a short duration
  digitalWrite(BUZZER, LOW); // Turn off the buzzer
}


void successSound() {
  tone(BUZZER, 1000, 100);  // Play a tone at 4000 Hz for 100 milliseconds
  delay(100);               // Add a short delay between tones for a smoother sound
  tone(BUZZER, 500, 100);
  noTone(BUZZER);  // Turn off the tone
}

void failSound() {
  tone(BUZZER, 1000, 300);  // Play a tone at 4000 Hz for 300 milliseconds

  tone(BUZZER, 500, 200);
  noTone(BUZZER);  // Turn off the tone
}

#define WIFI_TIMEOUT 20000

char esid[32];
char epass[64];

bool testWifi(void) {
}
void ConnectToWifi() {


  for (int i = 0; i < 32; ++i) {
    esid[i] = EEPROM.read(i);
  }
  esid[31] = '\0'; // Ensure the SSID string is null-terminated

  for (int i = 0; i < 64; ++i) {
    epass[i] = EEPROM.read(32 + i);
  }
  epass[63] = '\0'; // Ensure the password string is null-terminated

  Serial.print("SSID: ");
  Serial.println(esid);
  Serial.print("PASS: ");
  Serial.println(epass);

  WiFi.mode(WIFI_STA);
  WiFi.begin(esid, epass);
  //WiFi.begin(ssid, password);
  tft.fillScreen(TFT_BLACK);
  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < WIFI_TIMEOUT) {

    printTextCenteredNotoSansLarge("Connecting", TFT_WHITE, 130);
    printTextCenteredNotoSansLarge("to WiFi", TFT_WHITE, 170);
    printTextCenteredNotoSansSmall("Please wait...", TFT_WHITE, 220);

    animateWiFiIcon(WIFI_ICON_X, WIFI_ICON_Y, WIFI_ICON_WIDTH, WIFI_ICON_HEIGHT);

    Serial.println("Connecting to WiFi..");
    lastButtonAndScreenPressTime = millis();
    delay(100);
  }
  if (WiFi.status() != WL_CONNECTED) {
    tft.fillScreen(TFT_BLACK);

    printTextCenteredNotoSansLarge("Captive Portal", TFT_WHITE, 130);
    printTextCenteredNotoSansLarge("Now Active", TFT_GREEN, 170);

    Serial.println("Failed");
    Serial.println("Setting up AP Mode");

    WiFi.mode(WIFI_AP);
    String apName = String("SonicScan: ") + scanner_name;
    WiFi.softAP(apName.c_str());
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
    Serial.println("Setting up Async WebServer");
    setupServer();
    Serial.println("Starting DNS Server");
    dnsServer.start(53, "*", WiFi.softAPIP());
    server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);
    server.begin();
    Serial.println("All Done!");
    

    //Reconnect Button
    drawButtonItem((tft.width() - BUTTON_WIDTH) / 2, 250, "Reconnect");

    checkWifiStatus();


  } else {

    //WiFi Connected
    tft.fillScreen(TFT_WHITE);
    printTextCenteredNotoSansLarge("WiFi", TFT_BLACK, 120);
    printTextCenteredNotoSansLarge("Connected to", TFT_BLACK, 160);
    printTextCenteredNotoSansLarge(esid, TFT_GREEN, 200);
    Serial.println("Connected to the WiFi network");
  }
}

void checkWifiStatus() {
  while (true) {
    t_x = 0, t_y = 0;
    pressed = tft.getTouch(&t_x, &t_y);
    dnsServer.processNextRequest();
    triggersAndPowerSaving();
    if ((t_x >= 40) && (t_x <= 275) && (t_y >= 240) && (t_y <= 325)) {
      lastButtonAndScreenPressTime = millis();
      ConnectToWifi();
      return;
    }
  }
}




void setupServer() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
    Serial.println("Client Connected");
  });

  server.on("/save", HTTP_GET, [](AsyncWebServerRequest *request) {
    String ssidText;
    String passText;

    if (request->hasParam("ssid") && request->hasParam("password")) {
      ssidText = request->getParam("ssid")->value();
      passText = request->getParam("password")->value();

      Serial.println(ssidText);
      Serial.println("");
      Serial.println(passText);
      Serial.println("");

      Serial.println("Clearing EEPROM");
      for (int i = 0; i < EEPROM_SIZE; ++i) {
        EEPROM.write(i, 0);
      }

      Serial.println("Writing EEPROM SSID:");
      for (int i = 0; i < ssidText.length(); ++i) {
        EEPROM.write(i, ssidText[i]);
        Serial.print("Wrote: ");
        Serial.println(ssidText[i]);
      }
      EEPROM.write(ssidText.length(), '\0'); // Null-terminate the SSID

      Serial.println("Writing EEPROM Password:");
      for (int i = 0; i < passText.length(); ++i) {
        EEPROM.write(32 + i, passText[i]);
        Serial.print("Wrote: ");
        Serial.println(passText[i]);
      }
      EEPROM.write(32 + passText.length(), '\0'); // Null-terminate the Password

      EEPROM.commit();
      delay(500);
      ESP.restart();
    }

    request->send(200, "text/html", "The values entered by you have been successfully sent to the device <br><a href=\"/\">Return to Home Page</a>");
  });
}









void setup() {

  Serial.begin(57600);
  // set up pinmodes
  EEPROM.begin(EEPROM_SIZE); // 
  pinMode(LCDSCREEN, OUTPUT);           // LCDSCREEN POWER SUPPLY
  pinMode(BUZZER, OUTPUT);              // BUZZER
  pinMode(triggerBtnAndWakeUp, INPUT);  // TRIGGER BUTTON INPUT

  // TURN ON LCD LED SCREEN
  digitalWrite(LCDSCREEN, HIGH);
  startBuzzer();

  WiFi.disconnect();
  EEPROM.begin(512);  //Initialasing EEPROM

  // Init SPI bus.
  SPI.begin();

  // SET UP WHEN WAKE UP BUTTON IS CLICKED THEN WAKE UP ESP32 FROM DEEP SLEEP
  esp_sleep_enable_ext0_wakeup((gpio_num_t)triggerBtnAndWakeUp, HIGH);

  // INITIALIZE LCD
  tft.init();
  tft.setRotation(0);
  // Calibrate the touch screen and retrieve the scaling factors
  touch_calibrate();

  // FILL SCREEN
  tft.fillScreen(TFT_BLACK);

  tft.loadFont(AA_FONT_LARGE);
  

  printTextCenteredNotoSansLarge("SonicScan", TFT_BLUE, 80);
  printTextCenteredNotoSansLarge("IoT Vehicle", TFT_BLUE, 120);
  printTextCenteredNotoSansLarge("Identification", TFT_BLUE, 160);
  printTextCenteredNotoSansLarge("System", TFT_BLUE, 200);
  drawLoadingAnimation(330);
  
  delay(500);
 
  // // PN5180 INITIALIZE
  nfc.begin();
  nfc.reset();
  nfc.setupRF();



  delay(1500);

  //Initialize GPS
  ss.begin(9600, SERIAL_8N1, RXD2, TXD2);

  // CONNECT TO WIFI

  ConnectToWifi();
  tft.unloadFont();
  delay(1000);

  Serial.println();
  Serial.println("READY TO SCAN");

  displayMenu();
}



bool screenOn = true;


void startAlert() {
  alertStartTime = millis();
  isAlertActive = true;
  isBuzzerOn = true;
  stolenBuzz = true;         // Set stolenBuzz to true when the alert starts
  digitalWrite(BUZZER, HIGH);
  previousMillis = millis();
}

void stopAlert() {
  isAlertActive = false;
  digitalWrite(BUZZER, LOW);
}



void loop() {


  getLocation();

  unsigned long currentMillis = millis();

  // Check if alert is active
  if (isAlertActive) {
    // Check if alert duration has elapsed
    if (currentMillis - alertStartTime >= ALERT_DURATION) {
      // Stop the alert
      stopAlert();
      // Set stolenBuzz to false
      stolenBuzz = false;
    } else {
      // Toggle the buzzer state based on the defined on/off time
      if (isBuzzerOn && currentMillis - previousMillis >= BUZZER_ON_TIME) {
        digitalWrite(BUZZER, LOW);
        isBuzzerOn = false;
        previousMillis = currentMillis;
      } else if (!isBuzzerOn && currentMillis - previousMillis >= BUZZER_OFF_TIME) {
        digitalWrite(BUZZER, HIGH);
        isBuzzerOn = true;
        previousMillis = currentMillis;
      }
    }
  }


  t_x = 0, t_y = 0;
  pressed = tft.getTouch(&t_x, &t_y);
  if (pressed) {
    if (screenOn == false) {
      digitalWrite(LCDSCREEN, HIGH);
      screenOn = true;
    }
    lastButtonAndScreenPressTime = millis();
    // Serial.println("Button pressed. Resetting sleep timer.");
    Serial.println("Being pressed");
  } else {
    // Serial.println("Not pressed");
  }


  //Pages
  if(page == 6) {

    if (pressed) {
      // Calculate total height needed for buttons and margins
      int totalButtonHeight = 5 * BUTTON_HEIGHT + 4 * BUTTON_MARGIN;  // Updated for 5 buttons
      int startY = (tft.height() - totalButtonHeight) / 2;

      // Check which button was pressed
      if (isButtonPressed((tft.width() - BUTTON_WIDTH) / 2, startY, BUTTON_WIDTH, BUTTON_HEIGHT)) {
        Serial.println("Home button pressed");
        page = 0;
        displayMenu();
      } else if (isButtonPressed((tft.width() - BUTTON_WIDTH) / 2, startY + BUTTON_HEIGHT + BUTTON_MARGIN, BUTTON_WIDTH, BUTTON_HEIGHT)) {
        Serial.println("OR/CR button pressed");
        page = 1;
        subPage = 1.10;
        if (id) {

            displayORCR1();
        }else {
          noDataFound();
        }
      } else if (isButtonPressed((tft.width() - BUTTON_WIDTH) / 2, startY + 2 * (BUTTON_HEIGHT + BUTTON_MARGIN), BUTTON_WIDTH, BUTTON_HEIGHT)) {
        Serial.println("License button pressed");
          page = 2;
          if (id) {
            displayLicense();
          }else {
            noDataFound();
          }
       
      } else if (isButtonPressed((tft.width() - BUTTON_WIDTH) / 2, startY + 3 * (BUTTON_HEIGHT + BUTTON_MARGIN), BUTTON_WIDTH, BUTTON_HEIGHT)) {
        Serial.println("Location button pressed");
        page = 3;
        displayLocationPage();
      } else if (id && isButtonPressed((tft.width() - BUTTON_WIDTH) / 2, startY + 4 * (BUTTON_HEIGHT + BUTTON_MARGIN), BUTTON_WIDTH, BUTTON_HEIGHT)) {
        Serial.println("Last Scan button pressed");
        // Add your code here to handle Last Scan button press
      }
    }
  }

  // SUBPAGES 
  if (page == 1) {
    //first page
    if (fabs(subPage - 1.10) < 0.0001) {
      // If within tolerance, execute the following code block
      if ((t_x >= 230) && (t_x <= 310) && (t_y >= 425) && (t_y <= 465)) {
        subPage = 1.20;
        displayORCR2();
        delay(500);
      }
    }
    // 2nd page
    else if (fabs(subPage - 1.20) < 0.0001) {
      // If within tolerance, execute the following code block
      if ((t_x >= 230) && (t_x <= 310) && (t_y >= 425) && (t_y <= 465)) {
        subPage = 1.30;
        displayORCR3();
        delay(500);

      } else if ((t_x >= 10) && (t_x <= 90) && (t_y >= 425) && (t_y <= 465)) {
        subPage = 1.10;
        displayORCR1();
        delay(500);
      }
    }
    // 3rd page
    if (fabs(subPage - 1.30) < 0.0001) {
      // If within tolerance, execute the following code block
      if ((t_x >= 10) && (t_x <= 90) && (t_y >= 425) && (t_y <= 465)) {
        subPage = 1.20;
        displayORCR2();
        delay(500);
      }
    }
  }



  //Open Menu Bar
  if ((t_x >= 10) && (t_x <= 80) && (t_y >= 0) && (t_y <= 50) && page != 6) {
      Serial.println("Nagivation Choices");
      page = 6;
      tft.fillScreen(TFT_BLACK);
      displayButtonsMenuItem();
  }



  if (page == 3) {
    // Back to Menu
    updateLocationCoordinates();
    if ((t_x >= 30) && (t_x <= 80) && (t_y >= 30) && (t_y <= 40)) {
      page = 0;
      displayMenu();
      
    }
  }
  if (page == 9) {
    if ((t_x >= 30) && (t_x <= 80) && (t_y >= 30) && (t_y <= 40)) {
      page = 0;
      displayMenu();
    }
  }
   // Scanning card and 
  if (singleClickDetected) {
    // Add your action here
    if (screenOn == false) {
      digitalWrite(LCDSCREEN, HIGH);
      screenOn = true;
    }
    scanCard();
    // Reset the flag
    singleClickDetected = false;
  }
  triggersAndPowerSaving();

 

}

void noDataFound() {
  tft.fillScreen(TFT_BLACK);
  drawBackButton();
  printTextCentered("No Data Found",TFT_WHITE, 220, 3);
  printTextCentered("Try Scanning",TFT_WHITE, 250, 2);
}

void noCardScanned() {
  tft.fillScreen(TFT_BLACK);
  printTextCenteredNotoSansLarge("NO CARD", TFT_RED, 150);
  printTextCenteredNotoSansLarge("SCANNED", TFT_RED, 190);
  printTextCenteredNotoSansLarge("TRY AGAIN.", TFT_WHITE, 250);
  delay(1500);
  page = 0;
  displayMenu();
}

void lostWifi() {
  tft.fillScreen(TFT_BLACK);
  printTextCenteredNotoSansLarge("Lost Wifi", TFT_RED, 150);
  printTextCenteredNotoSansLarge("Connection", TFT_RED, 190);
  printTextCenteredNotoSansLarge("Restart Device.", TFT_WHITE, 250);
  delay(1500);
  page = 0;
  displayMenu();
}

void fetchingError() {
  tft.fillScreen(TFT_BLACK);
  printTextCenteredNotoSansLarge("Error Fetching", TFT_RED, 150);
  printTextCenteredNotoSansLarge("Connection Lost", TFT_RED, 190);
  printTextCenteredNotoSansLarge("Try Again.", TFT_WHITE, 250);
  delay(1500);
  page = 0;
  displayMenu();
}



void triggersAndPowerSaving() {
  t_x = 0, t_y = 0;
  pressed = tft.getTouch(&t_x, &t_y);
  if (pressed) {
    if (screenOn == false) {
      digitalWrite(LCDSCREEN, HIGH);
      screenOn = true;
    }
    lastButtonAndScreenPressTime = millis();
    // Serial.println("Button pressed. Resetting sleep timer.");
    Serial.println("Being pressed");
  } else {
    // Serial.println("Not pressed");
  }

  triggerState = digitalRead(triggerBtnAndWakeUp);
  if (triggerState == HIGH) {
    lastButtonAndScreenPressTime = millis();  // Update the last button press time
    unsigned long buttonPressStartTime = millis();
    Serial.println("Button pressed. Resetting sleep timer.");

    while (millis() - buttonPressStartTime < 5000) {
      if (digitalRead(triggerBtnAndWakeUp) == LOW) {
        // Button released before 5 seconds, exit loop
        break;
      }
    }
    if (digitalRead(triggerBtnAndWakeUp) == HIGH) {
      // Button held down for 5 seconds, enter deep sleep mode
      Serial.println("Button held down for 5 seconds. Going to sleep now.");
      digitalWrite(LCDSCREEN, LOW);  // Turn off any peripherals before deep sleep if needed
      delay(4000);
      esp_deep_sleep_start();  // Enter deep sleep mode

    } else {
      // Button released before 5 seconds, reset the sleep timer
      lastButtonAndScreenPressTime = millis();
      Serial.println("Button press detected. Resetting sleep timer.");
      singleClickDetected = true;
    }
  }

  if (millis() - lastButtonAndScreenPressTime >= 60000) {

    if (screenOn) {
      digitalWrite(LCDSCREEN, LOW);
      Serial.println("Turning Off Screen");
      screenOn = false;
    }
  }

  if (millis() - lastButtonAndScreenPressTime >= TIME_OUT_SLEEP) {
    digitalWrite(LCDSCREEN, LOW);
    Serial.println("No button press for 30 seconds. Going to sleep now.");

    esp_deep_sleep_start();  // Enter deep sleep mode
  }


 
}


bool isButtonPressed(int x, int y, int width, int height) {
  return (t_x >= x && t_x <= (x + width) && t_y >= y && t_y <= (y + height));
}

void drawButtonItem(int x, int y, const char* text) {
  // Draw border
  tft.fillRect(x - BUTTON_BORDER_THICKNESS, y - BUTTON_BORDER_THICKNESS,
               BUTTON_WIDTH + 2 * BUTTON_BORDER_THICKNESS, BUTTON_HEIGHT + 2 * BUTTON_BORDER_THICKNESS,
               BUTTON_BORDER_COLOR);

  // Draw button
  tft.fillRect(x, y, BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_COLOR);


  // Draw centered text
  tft.setTextSize(1);
  tft.setTextColor(BUTTON_TEXT_COLOR);
  int16_t x1, y1;
  uint16_t w, h;

  // Calculate text width manually
  w = strlen(text) * 12;  // Assuming each character is about 12 pixels wide at text size 2
  h = 16;  // Assuming text height is about 16 pixels at text size 2

  int16_t textWidth = tft.textWidth(text, 4);
  
  // Calculate x-coordinate for centering the text
  int16_t xCoor = (tft.width() - textWidth) / 2;

  // Print the text

  tft.drawString(text, xCoor, y + (BUTTON_HEIGHT - h) / 2, 4);
  tft.drawString(text, xCoor+1, y + (BUTTON_HEIGHT - h) / 2, 4); 
 
}

// Function to display multiple buttons centered on the screen
void displayButtonsMenuItem() {
  // Calculate total height needed for buttons and margins
  int totalButtonHeight = 5 * BUTTON_HEIGHT + 4 * BUTTON_MARGIN;  // Updated for 5 buttons
  int startY = (tft.height() - totalButtonHeight) / 2;

  // Display buttons
  drawButtonItem((tft.width() - BUTTON_WIDTH) / 2, startY, "Home");
  drawButtonItem((tft.width() - BUTTON_WIDTH) / 2, startY + BUTTON_HEIGHT + BUTTON_MARGIN, "OR/CR");
  drawButtonItem((tft.width() - BUTTON_WIDTH) / 2, startY + 2 * (BUTTON_HEIGHT + BUTTON_MARGIN), "License");
  drawButtonItem((tft.width() - BUTTON_WIDTH) / 2, startY + 3 * (BUTTON_HEIGHT + BUTTON_MARGIN), "Location");

}





void scanCard() {
  if (!pressed) {
    readsuccess = getUID();
    Serial.println(readsuccess);
    if (readsuccess) {
      Serial.println("Read Success " + String(readsuccess));
      Serial.println("UID IS:" + UID_Result);
      successSound();
      if ((WiFi.status() == WL_CONNECTED)) {  //Check the current connection status
        tft.fillScreen(TFT_BLACK);
        printTextCenteredNotoSansLarge("FETCHING", TFT_WHITE, 160);
        printTextCenteredNotoSansLarge("DATA...", TFT_WHITE, 200);
        drawLoadingAnimation(300);

        // Reuse HTTPClient instance
        String fetchUrl = "https://sonicscan-iot.vercel.app/drivers/" + UID_Result;
        httpClient.begin(fetchUrl); 
        int httpCode = httpClient.GET();  

        if (httpCode > 0) {  //Check for the returning code
          String payload = httpClient.getString();
          Serial.println("JSON: " + payload);
          // Parse Data
          if (payload.indexOf("error") == -1) {  // Check if "error" field is not present
            // Parse successful, handle the payload data here
            page = 1;
            subPage = 1.10;
            int successParse = parseJSON(payload);
            Serial.println("Success? " + String(successParse));
            
            // HTTP FOR LOCATION
            String urlLocation = "https://sonicscan-iot.vercel.app/location/" + String(scanner_name) + "/" + String(latBuffer) + "/" + String(lngBuffer);
            httpClient.begin(urlLocation); 
            int httpCode3 = httpClient.GET();

            // HTTP FOR SAVING DATA
            String savingDatainHistoryUrl = "https://sonicscan-iot.vercel.app/history/add/?plate_no=" + replaceSpaces(String(plate_no)) + "&status=" + replaceSpaces(String(status)) + "&vehicle_type=" + replaceSpaces(String(vehicle_type)) + "&latitude=" + String(latBuffer) + "&longitude=" + String(lngBuffer) + "&owners_name=" + replaceSpaces(String(owners_name)) + "&uid=" + replaceSpaces(String(uidTag)) + "&scanner=" + scanner_name;
            httpClient.begin(savingDatainHistoryUrl);
            int httpCode2 = httpClient.GET();
            if (httpCode2 > 0) {
              String payload2 = httpClient.getString();
              Serial.println("Payload:" + payload2);
            } else {
              Serial.println("HTTP request failed");
            }
            if (strcmp(status, "Stolen") == 0 && !isAlertActive) {
              startAlert();
            }
            displayORCR1();
          } else {
            // Error encountered, handle the error response here
            tft.fillScreen(TFT_BLACK);
            printTextCentered("User not found", TFT_WHITE, 200, 3);
            printTextCentered("UID IS", TFT_WHITE, 230, 3);
            printTextCentered(UID_Result, TFT_WHITE, 260, 2);
            drawBackButton();
            page = 9;
            failSound();

          }
        } else {
          fetchingError();
        }

        httpClient.end();  //Free the resources
      } else {
        lostWifi();
      }
    }
    if (readsuccess == 0) {
      failSound();
      Serial.println("Error");
      noCardScanned();
    }
  }
}
String replaceSpaces(String str) {
  String result = "";
  for (int i = 0; i < str.length(); i++) {
    if (str.charAt(i) == ' ') {
      result += "%20";
    } else {
      result += str.charAt(i);
    }
  }
  return result;
}



void printText(String text, uint16_t color, int x, int y) {

  tft.setTextSize(1);
  tft.setTextColor(color);

  //tft.setTextDatum(TC_DATUM);
  tft.drawString(text, x, y, 4); 
  tft.drawString(text, x+1, y, 4);
}

void printText2(String text, uint16_t color, int x, int y) {
  tft.setTextSize(2); // Attempt to set a smaller text size. This depends on your library's support.
  tft.setTextColor(color);

  //tft.setTextDatum(TC_DATUM);
  tft.drawString(text, x, y, 1); 
  tft.drawString(text, x+1, y, 1);
}

void printText3(String text, uint16_t color, int x, int y) {
  tft.setTextSize(1); // Attempt to set a smaller text size. This depends on your library's support.
  tft.setTextColor(color);

  //tft.setTextDatum(TC_DATUM);
  tft.drawString(text, x, y, 1); 

}

//MIDDLE
void printTextCentered(String text, uint16_t color, int y, int textSize) {
  int16_t x = (tft.width() - text.length() * 6 * textSize) / 2;  // Calculate x-coordinate for centering
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.setTextSize(textSize);
  tft.setTextWrap(true);
  tft.print(text);
}





void printTextCenteredNotoSansLarge(String text, uint16_t color, int y) {
  // Set the custom font
  tft.loadFont(AA_FONT_LARGE);

  // Calculate the width of the text
  int16_t x = (tft.width() - tft.textWidth(text.c_str())) / 2;
  
  // Set text color and cursor position
  tft.setCursor(x, y);
  tft.setTextColor(color, TFT_BLACK, false);
  tft.setTextWrap(true);
  
  // Print the text
  tft.print(text);

  // Unload the custom font
  tft.unloadFont();
}
void printTextCenteredNotoSansSmall(String text, uint16_t color, int y) {
  // Set the custom font
  tft.loadFont(AA_FONT_SMALL);

  // Calculate the width of the text
  int16_t x = (tft.width() - tft.textWidth(text.c_str())) / 2;
  
  // Set text color and cursor position
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  
  // Print the text
  tft.print(text);

  // Unload the custom font
  tft.unloadFont();
}
void printTextCenteredbg(String text, uint16_t color, uint16_t bgcolor,int y, int textSize) {
  int16_t x = (tft.width() - text.length() * 6 * textSize) / 2;  // Calculate x-coordinate for centering
  tft.setCursor(x, y);
  tft.setTextColor(color, bgcolor, true);
  tft.setTextSize(textSize);
  tft.setTextWrap(true);
  tft.print(text);
  
}



  
void printTextQuarterSmall(String text, uint16_t color, int y, int textSize, bool underline) {
  // Set the text size
  tft.setTextSize(textSize);
  
  // Calculate the width of the text
  int16_t textWidth = tft.textWidth(text.c_str(), 2);
  
  // Calculate x-coordinate for 1/4 of the screen
  int16_t x = (tft.width() / 4) - (textWidth / 2);
  
  // Set text color
  tft.setTextColor(color);
  
  // Print the text
  tft.drawString(text, x, y, 2);

  // If underline is true, draw the underline
  if (underline) {
    // Position the underline just below the text
    tft.drawString(text, x+1, y, 2);
    int16_t underlineY = y + tft.fontHeight(2); // Adjust to the font height
    tft.drawLine(x, underlineY, x + textWidth, underlineY, color);
  }
}


void printTextQuarter(String text, uint16_t color, int y, int textSize, bool underline) {
  // Set the text size
  tft.setTextSize(textSize);
  
  // Calculate the width of the text
  int16_t textWidth = tft.textWidth(text.c_str(), 4);
  
  // Calculate x-coordinate for 1/4 of the screen
  int16_t x = (tft.width() / 4) - (textWidth / 2);
  
  // Set text color
  tft.setTextColor(color);
  
  // Print the text
  tft.drawString(text, x, y, 4);

  // If underline is true, draw the underline
  if (underline) {
    // Position the underline just below the text
    tft.drawString(text, x+1, y, 4);
    int16_t underlineY = y + tft.fontHeight(4); // Adjust to the font height
    tft.drawLine(x, underlineY, x + textWidth, underlineY, color);
  }
}



void printTextThreeQuarters(String text, uint16_t color, int y, int textSize, bool underline) {
  // Set the text size
  tft.setTextSize(textSize);
  
  // Calculate the width of the text
  int textWidth = tft.textWidth(text.c_str(), 4);
  
  // Calculate x-coordinate for 3/4 of the screen
  int16_t x = (3 * tft.width() / 4) - (textWidth / 2);
  
  // Set text color and cursor position
  tft.setTextColor(color);
  
  // Print the text
  tft.drawString(text, x, y, 4);

  // If underline is true, draw the underline
  if (underline) {
    // Position the underline just below the text
    tft.drawString(text, x+1, y, 4);
    int16_t underlineY = y + tft.fontHeight(4); // Adjust to the font height
    tft.drawLine(x, underlineY, x + textWidth, underlineY, color);
  }
}







void displayLocationPage() {
  tft.fillScreen(TFT_BLACK);
  drawBackButton();
  printTextCentered("SonicScan", TFT_WHITE, 60, 2);
  String fullText = "CodeName " + String(scanner_name);
  printTextCentered(fullText.c_str(), TFT_WHITE, 80, 2);
  printTextCentered("Coordinates", TFT_WHITE, 100, 2);
}



int parseJSON(String input) {
  // Stream& input;

  StaticJsonDocument<256> doc;

  DeserializationError error = deserializeJson(doc, input);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return 0;
  }

  JsonObject root_0 = doc[0];
  // Start of OR/CR Variables
  id = root_0["id"].as<int>();
  strlcpy(uidTag, root_0["uid"], sizeof(uidTag));
  strlcpy(status, root_0["status"], sizeof(status));
  strlcpy(field_office, root_0["field_office"], sizeof(field_office));
  strlcpy(field_code, root_0["field_code"], sizeof(field_code));
  strlcpy(cr_no, root_0["cr_no"], sizeof(cr_no));
  strlcpy(date, root_0["date"], sizeof(date));
  strlcpy(plate_no, root_0["plate_no"], sizeof(plate_no));
  strlcpy(engine_no, root_0["engine_no"], sizeof(engine_no));
  strlcpy(chassis_no, root_0["chassis_no"], sizeof(chassis_no));
  strlcpy(vin, root_0["vin"], sizeof(vin));
  strlcpy(file_no, root_0["file_no"], sizeof(file_no));
  strlcpy(vehicle_type, root_0["vehicle_type"], sizeof(vehicle_type));
  strlcpy(vehicle_category, root_0["vehicle_category"], sizeof(vehicle_category));
  strlcpy(make_brand, root_0["make_brand"], sizeof(make_brand));
  strlcpy(body_type, root_0["body_type"], sizeof(body_type));
  strlcpy(series, root_0["series"], sizeof(series));
  strlcpy(gross_weight, root_0["gross_weight"], sizeof(gross_weight));
  strlcpy(net_weight, root_0["net_weight"], sizeof(net_weight));
  strlcpy(year_model, root_0["year_model"], sizeof(year_model));
  strlcpy(year_rebuilt, root_0["year_rebuilt"], sizeof(year_rebuilt));
  strlcpy(piston_displacement, root_0["piston_displacement"], sizeof(piston_displacement));
  strlcpy(max_power, root_0["max_power"], sizeof(max_power));
  strlcpy(passenger_capacity, root_0["passenger_capacity"], sizeof(passenger_capacity));
  strlcpy(color, root_0["color"], sizeof(color));
  strlcpy(type_of_fuel, root_0["type_of_fuel"], sizeof(type_of_fuel));
  strlcpy(registration_classification, root_0["registration_classification"], sizeof(registration_classification));
  strlcpy(owners_name, root_0["owners_name"], sizeof(owners_name));
  strlcpy(owners_address, root_0["owners_address"], sizeof(owners_address));
  strlcpy(encumbered_to, root_0["encumbered_to"], sizeof(encumbered_to));
  strlcpy(details_of_first_registration, root_0["details_of_first_registration"], sizeof(details_of_first_registration));
  strlcpy(or_no, root_0["or_no"], sizeof(or_no));
  strlcpy(or_date, root_0["or_date"], sizeof(or_date));
  strlcpy(amount, root_0["amount"], sizeof(amount));
  strlcpy(remarks, root_0["remarks"], sizeof(remarks));

  // Start of Driver's License Variables
  strlcpy(first_name, root_0["first_name"], sizeof(first_name));
  strlcpy(last_name, root_0["last_name"], sizeof(last_name));
  strlcpy(birthday, root_0["birthday"], sizeof(birthday));
  strlcpy(sex, root_0["sex"], sizeof(sex));
  strlcpy(nationality, root_0["nationality"], sizeof(nationality));
  strlcpy(licensenumber, root_0["licensenumber"], sizeof(licensenumber));
  strlcpy(weight, root_0["weight"], sizeof(weight));
  strlcpy(height, root_0["height"], sizeof(height));
  strlcpy(expdate, root_0["expdate"], sizeof(expdate));
  strlcpy(licensetype, root_0["licensetype"], sizeof(licensetype));
  strlcpy(address, root_0["address"], sizeof(address));
  strlcpy(agencycode, root_0["agencycode"], sizeof(agencycode));
  strlcpy(blood, root_0["blood"], sizeof(blood));
  strlcpy(eye, root_0["eye"], sizeof(eye));
  strlcpy(condition, root_0["condition"], sizeof(condition));


  return 1;
}



void displayLocationInfo() {
  Serial.print(F("Location: "));

  if (gps.location.isValid()) {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
    latitude = gps.location.lat();
    longitude = gps.location.lng();
    dtostrf(latitude, 9, 6, latBuffer);
    dtostrf(longitude, 9, 6, lngBuffer);
    gotLocationAlready = true;
  } else {
    if(!gotLocationAlready) {
      Serial.print(F("INVALID"));
      latitude = 17.6132;
      longitude = 121.7270;
      dtostrf(latitude, 9, 6, latBuffer);
      dtostrf(longitude, 9, 6, lngBuffer);
    }

  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid()) {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year()); 
  } else {
    Serial.print(F("INVALID"));
  }
  Serial.print(F(" "));
  if (gps.time.isValid()) {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
    
  } else {

    Serial.print(F("INVALID"));
    
  }

  Serial.println();
}

unsigned long previousDisplayTime = 0;
const unsigned long displayInterval = 2500;

void getLocation() {
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      // Display location information every second
      if (millis() - previousDisplayTime >= displayInterval) {
        //tft.fillRect(0,50,480,270, TFT_BLUE);
        displayLocationInfo();
        previousDisplayTime = millis();
      }

  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println(F("No GPS detected: check wiring."));
  }
}







#define BACK_BUTTON_WIDTH 100
#define BACK_BUTTON_HEIGHT 30
#define BACK_BUTTON_MARGIN 10
#define BACK_BUTTON_COLOR TFT_RED
#define BACK_BUTTON_TEXT_COLOR TFT_WHITE
#define BACK_BUTTON_BORDER_COLOR TFT_WHITE

void drawBackButton() {
  // Clear previous button
  tft.fillRect(BACK_BUTTON_MARGIN - 1, BACK_BUTTON_MARGIN - 1, BACK_BUTTON_WIDTH + 2, BACK_BUTTON_HEIGHT + 2, TFT_BLACK);

  // Draw border
  tft.drawRect(BACK_BUTTON_MARGIN - 1, BACK_BUTTON_MARGIN - 1, BACK_BUTTON_WIDTH + 2, BACK_BUTTON_HEIGHT + 2, BACK_BUTTON_BORDER_COLOR);

  // Draw button
  tft.fillRect(BACK_BUTTON_MARGIN, BACK_BUTTON_MARGIN, BACK_BUTTON_WIDTH, BACK_BUTTON_HEIGHT, BACK_BUTTON_COLOR);
  tft.setTextSize(2);
  tft.setTextColor(BACK_BUTTON_TEXT_COLOR);
  tft.setCursor(BACK_BUTTON_MARGIN + 10, BACK_BUTTON_MARGIN + 5);
  tft.print("< Back");
}



#define NEXT_BUTTON_WIDTH 100
#define NEXT_BUTTON_HEIGHT 30
#define NEXT_BUTTON_MARGIN 10
#define NEXT_BUTTON_COLOR TFT_BLUE
#define NEXT_BUTTON_TEXT_COLOR TFT_WHITE
#define NEXT_BUTTON_BORDER_COLOR TFT_WHITE
int buttonY = 435;  // Y position for the button at the bottom


void drawPreviousButton() {
 

  // Clear previous button
  tft.fillRect(BACK_BUTTON_MARGIN - 1, buttonY, BACK_BUTTON_WIDTH + 2, BACK_BUTTON_HEIGHT + 2, TFT_BLACK);

  // Draw border
  tft.drawRect(BACK_BUTTON_MARGIN - 1, buttonY, BACK_BUTTON_WIDTH + 2, BACK_BUTTON_HEIGHT + 2, BACK_BUTTON_BORDER_COLOR);

  // Draw button
  tft.fillRect(BACK_BUTTON_MARGIN, buttonY, BACK_BUTTON_WIDTH, BACK_BUTTON_HEIGHT, BACK_BUTTON_COLOR);
  tft.setTextSize(1);
  tft.setTextColor(BACK_BUTTON_TEXT_COLOR);
  tft.drawString("< Prev", BACK_BUTTON_MARGIN + 10, buttonY + 5, 4); 
}

void drawNextButton() {
  int buttonX = 320 - NEXT_BUTTON_WIDTH - NEXT_BUTTON_MARGIN;   // X position for the button at the bottom right

  // Clear previous button
  tft.fillRect(buttonX - 1, buttonY, NEXT_BUTTON_WIDTH + 2, NEXT_BUTTON_HEIGHT + 2, TFT_BLACK);

  // Draw border
  tft.drawRect(buttonX - 1, buttonY, NEXT_BUTTON_WIDTH + 2, NEXT_BUTTON_HEIGHT + 2, NEXT_BUTTON_BORDER_COLOR);

  // Draw button
  tft.fillRect(buttonX, buttonY, NEXT_BUTTON_WIDTH, NEXT_BUTTON_HEIGHT, NEXT_BUTTON_COLOR);
  tft.setTextSize(1);
  tft.setTextColor(NEXT_BUTTON_TEXT_COLOR);
  tft.drawString("Next >", buttonX + 10, buttonY + 5, 4); 
  

}


void updateLocationCoordinates() {


  if (gps.location.isValid()) {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
    printTextCenteredbg("Latitude", TFT_WHITE, TFT_BLACK, 200, 2);
    printTextCenteredbg(String(gps.location.lat(), 6), TFT_WHITE, TFT_BLACK, 230, 2);
    printTextCenteredbg("Longitude", TFT_WHITE, TFT_BLACK, 260, 2);
    printTextCenteredbg(String(gps.location.lng(), 6), TFT_WHITE, TFT_BLACK, 290, 2);
  } else {
    printTextCenteredbg("Latitude", TFT_WHITE, TFT_BLACK, 200, 2);
    printTextCenteredbg("INVALID", TFT_WHITE, TFT_BLACK, 230, 2);
    printTextCenteredbg("Longitude", TFT_WHITE, TFT_BLACK, 260, 2);
    printTextCenteredbg("INVALID", TFT_WHITE, TFT_BLACK, 290, 2);

  }

}


void displayMenu() {
  //menu
  tft.fillScreen(TFT_BLACK);
  drawHamburgerMenu();
  printTextCenteredNotoSansLarge("Device Ready", TFT_WHITE, 120);
  printTextCenteredNotoSansLarge("Start", TFT_WHITE, 160);
  printTextCenteredNotoSansLarge("Scanning", TFT_WHITE, 200);






}
void displayLicense() {
  //LICENSE
  tft.fillScreen(TFT_BLACK);
  displayDetailBox2();
  drawHamburgerMenu();
}


void displayDetailBox() {
  int x = 10;        // X coordinate of the top-left corner of the rectangle
  int y = 60;        // Y coordinate of the top-left corner of the rectangle
  int width = 300;   // Width of the rectangle
  int height = 355;  // Height of the rectangle
  int radius = 10;   // Radius for rounded corners
  int borderThickness = 2;

  // Shadow effect (draw a slightly offset, darker rectangle behind the main one)
  tft.fillRect(x + 5, y + 5, width, height, TFT_DARKGREY);


  // Main rounded rectangle
  tft.fillRect(x, y, width, height, TFT_WHITE);
  for (int i = 0; i < borderThickness; i++) {
    tft.drawRect(x + i, y + i, width - 2 * i, height - 2 * i, TFT_BLACK);
  }



}




// FOR LICENSE
void displayDetailBox2() {
  int x = 0;         // X coordinate of the top-left corner of the rectangle
  int y = 60;        // Y coordinate of the top-left corner of the rectangle
  int width = 320;   // Width of the rectangle
  int heightBox = 480;  // heightBox of the rectangle
  printTextCenteredNotoSansLarge("License", TFT_WHITE, 15);


  tft.fillRect(x, y, width, heightBox, TFT_WHITE);
  tft.drawRect(x, y, width, heightBox, TFT_LIGHTBLUE);

  char full_name[100];
  strcpy(full_name, first_name);

  strcat(full_name, " ");
  strcat(full_name, last_name);


  printTextCenteredLineSmall(full_name, TFT_BLACK, 80, 1, true);
  printTextCenteredLine("Full Name", TFT_GREY, 110, 1, false);

  printTextCenteredLine(birthday, TFT_BLACK, 150, 1, true);
  printTextCenteredLine("Date Of Birth", TFT_GREY, 180, 1, false);

  printTextQuarter(nationality, TFT_BLACK, 220, 1, true);
  printTextQuarter("Nationality", TFT_GREY, 250, 1, false);

  printTextThreeQuarters(sex, TFT_BLACK, 220, 1, true);
  printTextThreeQuarters("Sex", TFT_GREY, 250, 1, false);

  printTextQuarterSmall(licensenumber, TFT_BLACK, 300, 1, true);
  printTextQuarter("License No.", TFT_GREY, 330, 1, false);

  String weightStr = String(weight) + " Kg";

  // Call the function with the concatenated string
  printTextQuarter(weightStr, TFT_BLACK, 380, 1, true);
  printTextQuarter("Weight", TFT_GREY, 410, 1, false);

  printTextThreeQuarters(expdate, TFT_BLACK, 300, 1, true);
  printTextThreeQuarters("Expiry Date", TFT_GREY, 330, 1, false);

   String heightStr = String(height) + " cm";

  // Call the function with the concatenated string
  printTextThreeQuarters(heightStr, TFT_BLACK, 380, 1, true);
  printTextThreeQuarters("Height", TFT_GREY, 410, 1, false);

}


void printTextCenteredLine(String text, uint16_t color, int y, int textSize, bool underline) {
  tft.setTextSize(textSize);
  tft.setTextColor(color);

  // Calculate the width of the text
  int16_t textWidth = tft.textWidth(text.c_str(), 4);
  
  // Calculate x-coordinate for centering the text
  int16_t x = (tft.width() - textWidth) / 2;

  // Print the text
  tft.drawString(text, x, y, 4);

  // If underline is true, draw the underline
  if (underline) {
    // Calculate the underline position and dimensions
    tft.drawString(text, x+1, y, 4);
    int16_t underlineY = y + tft.fontHeight(4); // Position just below the text
    int16_t underlineWidth = textWidth; // Width of the underline

    // Draw the underline
    tft.drawLine(x, underlineY, x + underlineWidth, underlineY, color);
  }
}

void printTextCenteredLineSmall(String text, uint16_t color, int y, int textSize, bool underline) {
  // Set the text size
  tft.setTextSize(textSize);
  tft.setTextColor(color);

  // Calculate the width of the text
  int16_t textWidth = tft.textWidth(text.c_str(), 2); // Font ID 1 is used here
  
  // Calculate x-coordinate for centering the text
  int16_t x = (tft.width() - textWidth) / 2;

  // Print the text
  tft.drawString(text, x, y, 2);

  // If underline is true, draw the underline
  if (underline) {
    // Calculate the underline position and dimensions
    tft.drawString(text, x+1, y, 2);
    int16_t underlineY = y + tft.fontHeight(2);
    int16_t underlineWidth = textWidth; // Width of the underline

    // Draw the underline
    tft.drawLine(x, underlineY, x + underlineWidth, underlineY, color);
  }
}








void displayORCR1() {
  //DISPLAY THE ORCR DETAILS
  tft.fillScreen(TFT_BLACK);
  displayDetailBox();
  printTextCenteredNotoSansLarge("OR/CR", TFT_WHITE, 15);
  // x    y      width         heigh
  /* Start Display Status  */
  checkStatus();


  /* End Display Status */

  printText("PLATE#:", TFT_BLACK, 20, 140);
  printText(plate_no, TFT_BLACK, 140, 140);

  printText("CR#:", TFT_BLACK, 20, 170);
  printText(cr_no, TFT_BLACK, 95, 170);

  printText("OR#:", TFT_BLACK, 20, 200);
  printText(or_no, TFT_BLACK, 95, 200);

  printText("OR Date:", TFT_BLACK, 20, 230);
  printText(or_date, TFT_BLACK, 140, 230);

  printText("Vehicle:", TFT_BLACK, 20, 260);
  printText(vehicle_type, TFT_BLACK, 130, 260);

  printText("Color:", TFT_BLACK, 20, 290);
  printText(color, TFT_BLACK, 140, 290);

  printText("Category:", TFT_BLACK, 20, 320);
  printText(vehicle_category, TFT_BLACK, 140, 320);

  printText("Brand:", TFT_BLACK, 20, 350);
  printText(make_brand, TFT_BLACK, 140, 350);

  printText("Body Type:", TFT_BLACK, 20, 380);
  printText(make_brand, TFT_BLACK, 150, 380);
  
  drawHamburgerMenu();

  displayNextBtn();
  tft.unloadFont();
}
void displayORCR2() {
  //DISPLAY THE 2ND ORCR DETAILS
  tft.fillScreen(TFT_BLACK);
  displayDetailBox();

  printTextCenteredNotoSansLarge("OR/CR", TFT_WHITE, 15);

  printText("Date:", TFT_BLACK, 20, 80);
  printText(date, TFT_BLACK, 140, 80);
  
  printText("Engine#:", TFT_BLACK, 20, 110);
  printText2(engine_no, TFT_BLACK, 140, 115);


  printText("Chassis#:", TFT_BLACK, 20, 140);
  printText3(chassis_no, TFT_BLACK, 140, 150);

  printText("Series:", TFT_BLACK, 20, 170);
  printText(series, TFT_BLACK, 140, 170);

  printText("Gross Weight:", TFT_BLACK, 20, 200);
  printText(gross_weight, TFT_BLACK, 190, 200);

  printText("Net Weight:", TFT_BLACK, 20, 230);
  printText(net_weight, TFT_BLACK, 190, 230);

  printText("Year Model:", TFT_BLACK, 20, 260);
  printText(year_model, TFT_BLACK, 190, 260);

  printText("Year Rebuilt:", TFT_BLACK, 20, 290);
  printText(year_rebuilt, TFT_BLACK, 190, 290);

  printText("Power:", TFT_BLACK, 20, 320);
  printText(max_power, TFT_BLACK, 140, 320);

  printText("Encumbered to:", TFT_BLACK, 20, 350);
  printText2(encumbered_to, TFT_BLACK, 40, 380);

 

  drawHamburgerMenu();

  displayNextBtn();
}

void displayORCR3() {
  //DISPLAY THE 2ND ORCR DETAILS
  tft.fillScreen(TFT_BLACK);
  displayDetailBox();

  printText("Field Code:", TFT_BLACK, 20, 80);
  printText(field_code, TFT_BLACK, 160, 80);
  printTextCenteredNotoSansLarge("OR/CR", TFT_WHITE, 15);

  printText("Field Office:", TFT_BLACK, 20, 110);
  printText2(field_office, TFT_BLACK, 30, 140);

  printText("Piston Displacement:", TFT_BLACK, 20, 170);
  printText(piston_displacement, TFT_BLACK, 30, 200);

  printText("Registration Class:", TFT_BLACK, 20, 230);
  printText(registration_classification, TFT_BLACK, 30, 260);

  printText("Fuel Type:", TFT_BLACK, 20, 290);
  printText(type_of_fuel, TFT_BLACK, 160, 290);

  printText2("Owners Name:", TFT_BLACK, 20, 330);
  printText3(owners_name, TFT_BLACK, 30, 355);

  printText2("Owners Address:", TFT_BLACK, 20, 370);
  printText3(owners_address, TFT_BLACK, 30, 395);


  drawHamburgerMenu();

  displayNextBtn();
}
void displayNextBtn() {
  //BACK
  if (fabs(subPage - 1.10) > 0.0001) {
    drawPreviousButton();
  }

  // NEXT
  if (fabs(subPage - 1.30) > 0.0001) {
    drawNextButton();
  }
}




#define HAMBURGER_MENU_X 10
#define HAMBURGER_MENU_Y 10
#define HAMBURGER_MENU_WIDTH 35
#define HAMBURGER_MENU_HEIGHT 20
#define HAMBURGER_MENU_COLOR TFT_WHITE
#define HAMBURGER_MENU_LINE_THICKNESS 6
#define HAMBURGER_MENU_LINE_SPACING 6
void drawHamburgerMenu() {
  int y = HAMBURGER_MENU_Y;
  for (int i = 0; i < 3; i++) {
    tft.fillRect(HAMBURGER_MENU_X, y, HAMBURGER_MENU_WIDTH, HAMBURGER_MENU_LINE_THICKNESS, HAMBURGER_MENU_COLOR);
    y += HAMBURGER_MENU_LINE_THICKNESS + HAMBURGER_MENU_LINE_SPACING;
  } 
}



void drawWiFiBar(int x, int y, int width, int height, uint16_t color) {
  tft.fillRect(x, y - height, width, height, color);
}

// Draw the full Wi-Fi icon
void drawWiFiIcon(int x, int y, int width, int height, int strength) {
  int barWidth = width / 5;  // Adjust bar width for four bars
  int spacing = barWidth / 2;

  // Clear the previous Wi-Fi icon with extra width
  tft.fillRect(x - spacing * 2, y - height - 5, width + 4 * spacing, height + 10, WIFI_BACKGROUND);

  // Draw bars based on the signal strength
  if (strength > 0) drawWiFiBar(x, y, barWidth, height / 4, WIFI_COLOR);
  if (strength > 1) drawWiFiBar(x + barWidth + spacing, y, barWidth, 2 * height / 4, WIFI_COLOR);
  if (strength > 2) drawWiFiBar(x + 2 * (barWidth + spacing), y, barWidth, 3 * height / 4, WIFI_COLOR);
  if (strength > 3) drawWiFiBar(x + 3 * (barWidth + spacing), y, barWidth, height, WIFI_COLOR);
}

// Animate the Wi-Fi icon
void animateWiFiIcon(int x, int y, int width, int height) {
  for (int i = 0; i <= 4; i++) {
    drawWiFiIcon(x, y, width, height, i);
    delay(200);  // Adjust delay to control animation speed
  }
}






void checkStatus() {
  if (strcmp(status, "Stolen") == 0) {
    // If status is "stolen", do something
    tft.fillRoundRect(140, 75, 120, 50, radius, TFT_RED);
    tft.drawRoundRect(140, 75, 120, 50, radius, TFT_BLACK);
    printText("Status:", TFT_BLACK, 20, 90);
    printText(status, TFT_WHITE, 160, 90);
  } else {
    // If status is not "stolen", do nothing
    tft.fillRoundRect(140, 75, 147, 50, radius, TFT_BLUE);
    tft.drawRoundRect(140, 75, 147, 50, radius, TFT_BLACK);
    printText("Status:", TFT_BLACK, 20, 90);
    printText(status, TFT_WHITE, 150, 90);
  }
}

void touch_calibrate() {
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  // check file system exists
  if (!SPIFFS.begin()) {
    Serial.println("Formating file system");
    SPIFFS.format();
    SPIFFS.begin();
  }

  // check if calibration file exists and size is correct
  if (SPIFFS.exists(CALIBRATION_FILE)) {
    if (REPEAT_CAL) {
      // Delete if we want to re-calibrate
      SPIFFS.remove(CALIBRATION_FILE);
    } else {
      File f = SPIFFS.open(CALIBRATION_FILE, "r");
      if (f) {
        if (f.readBytes((char*)calData, 14) == 14)
          calDataOK = 1;
        f.close();
      }
    }
  }

  if (calDataOK && !REPEAT_CAL) {
    // calibration data valid
    tft.setTouch(calData);
  } else {
    // data not valid so recalibrate
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 0);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.println("Touch corners as indicated");

    tft.setTextFont(1);
    tft.println();

    if (REPEAT_CAL) {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.println("Set REPEAT_CAL to false to stop this running again!");
    }

    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibration complete!");

    // store data
    File f = SPIFFS.open(CALIBRATION_FILE, "w");
    if (f) {
      f.write((const unsigned char*)calData, 14);
      f.close();
    }
  }
}

#define SCREEN_WIDTH 320
#define CENTER_X (SCREEN_WIDTH / 2)
#define RADIUS 25
#define SEGMENT_LENGTH 15
#define NUM_SEGMENTS 12
#define LINE_THICKNESS 8

void drawLoadingAnimation(int centerY) {
  float angleStep = 360.0 / NUM_SEGMENTS;
  int delayTime = 100;  // Delay time in milliseconds
  
  for (int i = 0; i < NUM_SEGMENTS; i++) {
    float angle = i * angleStep;

    // Calculate the start and end points of the segment
    int x1 = CENTER_X + RADIUS * cos(radians(angle));
    int y1 = centerY + RADIUS * sin(radians(angle));
    int x2 = CENTER_X + (RADIUS + SEGMENT_LENGTH) * cos(radians(angle));
    int y2 = centerY + (RADIUS + SEGMENT_LENGTH) * sin(radians(angle));

    // Draw the rotating segment with thickness
    for (int j = -LINE_THICKNESS / 2; j <= LINE_THICKNESS / 2; j++) {
      tft.drawLine(x1 + j, y1, x2 + j, y2, TFT_BLUE);
      tft.drawLine(x1, y1 + j, x2, y2 + j, TFT_BLUE);
    }

    delay(delayTime);

    // Erase the segment to create the rotation effect
    for (int j = -LINE_THICKNESS / 2; j <= LINE_THICKNESS / 2; j++) {
      tft.drawLine(x1 + j, y1, x2 + j, y2, TFT_WHITE);
      tft.drawLine(x1, y1 + j, x2, y2 + j, TFT_WHITE);
    }
  }
}


// void displayImage(uint8_t* imageData, size_t imageSize) {
//   // Clear screen
//   tft.fillScreen(TFT_BLACK);

//   // Decode and display JPG image
//   JPEGDecoder decoder;
//   decoder.setJpgScale(1); // Set the scale factor (1 = original size)
//   decoder.setCallback(tft.drawPixel); // Set the drawing callback
//   decoder.decode(imageData, imageSize);
// }






// ESP-32    <--> PN5180 pin mapping:
// 3.3V      <--> 3.3V
// GND       <--> GND
// SCLK, 18   --> SCLK
// MISO, 19  <--  MISO
// MOSI, 23   --> MOSI
// SS, 16     --> NSS (=Not SS -> active LOW)
// BUSY, 5   <--  BUSY
// Reset, 17  --> RST
//
