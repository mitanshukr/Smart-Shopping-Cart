#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 53
#define RST_PIN 5

#include <LiquidCrystal.h> // includes the LiquidCrystal Library 
LiquidCrystal lcd(8, 9, 10, 11,12, 13); // Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7)

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
const int buzzer = 7; // buzzer to arduino pin 7

int removeProductButton = 2;    //remove product
int generateBillButton = 3;   //generate bill
boolean removeItem = false;


String wifiName = "V40 ThinQ_3650";
String password = "mkmk78789";

String serverHost = "192.168.43.8";
String port = "80";

float totalPrice = 0.0;
String orderId = "";
String tagID = "";

void setup() {
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  
  lcd.begin(16,2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display } 

  pinMode(generateBillButton, INPUT_PULLUP);  
  pinMode(removeProductButton, INPUT_PULLUP);
  pinMode(buzzer,OUTPUT); // set buzzer pin 7 as an output
  
  Serial1.begin(115200);  //esp8266 serial begin
  sendCommand("AT", 5, "OK");
  sendCommand("AT+CWMODE=1", 5, "OK");
  sendCommand("AT+CWJAP=\"" + wifiName + "\",\"" + password + "\"", 10, "OK");

  Serial.println("Welcome to ABC SuperMarket!");
  lcd.setCursor(3,0);
  lcd.print("Welcome to");
  lcd.setCursor(0,1);
  lcd.print("ABC SuperMarket!");
  delay(1000);
  lcd.clear(); // Clears the display 
  
  Serial.println("Please Wait...");
  lcd.print("Please Wait...");
  
  //Generating Unique Order Id.
  orderId = sendToServer("GET /Test_site/generateOrderID.php?");
  int backTickIndex = orderId.indexOf('`');
  orderId = orderId.substring((backTickIndex + 1),(backTickIndex + 13));
  //Serial.println("Order ID: " + orderId);

  lcd.clear(); // Clears the display 
  lcd.print("Scan a Product...");
  Serial.println("Scan a Product to Add into cart..."); 
}

void loop(){
  int removeButtonVal = digitalRead(removeProductButton);
  int generateBillVal = digitalRead(generateBillButton);
  if(removeButtonVal == LOW)
  removeProduct();

  if(generateBillVal == LOW)
  generateBill();


  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  lcd.clear(); // Clears the display 
  lcd.print("Processing");
  lcd.setCursor(0,1);
  lcd.print("Request...");

  Serial.println("Processing Request...");
  tone(buzzer, 5000, 100);

  // Serial.print("UID tag: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    tagID += String(mfrc522.uid.uidByte[i], HEX);
  }
  tagID.toUpperCase();
  //Serial.print(tagID);
  Serial.println();

  String productdata= sendToServer("GET /Test_site/addProduct.php?orderId=" + orderId + "&productId=" + tagID);
  trimOutput(productdata);
}

String sendToServer(String url) {
  //place LCD Print("Processing...") if delay in final output.
  
  String serverHTTP = url;
  
  sendCommand("AT+CIPMUX=1", 5, "OK");
  sendCommand("AT+CIPSTART=0,\"TCP\",\"" + serverHost + "\"," + port, 10, "OK");
  sendCommand("AT+CIPSEND=0," + String(serverHTTP.length() + 2), 5, ">");
  Serial1.println(serverHTTP);
  String serverOutput = readServer();
  return serverOutput;
}

String readServer() {
  String ch = "";
  while (Serial1.available()) {
    ch = Serial1.readStringUntil('\0');
    //Serial.print(ch);
    //delay(10);
  }
  return ch;
}

void trimOutput(String ch){
  int backTick1 = ch.indexOf('`');
  int backTick2 = ch.indexOf('`', backTick1 + 1);
  int backTick3 = ch.indexOf('`', backTick2 + 1);
  
  String price = ch.substring(backTick1 + 1, backTick2);
  String product = ch.substring(backTick2 + 1, backTick3);

  Serial.println();
  if(removeItem == false){
  Serial.println("Added: Rs. " + price + "\n@ " + product);
  lcd.clear(); // Clears the display 
  lcd.print("Added: Rs." + price);
  lcd.setCursor(0,1);
  lcd.print("@" + product);
  }
  else if(removeItem == true){
    if(price.equals("0")){
    Serial.println("Product Not Found");
    lcd.clear(); // Clears the display
    lcd.print("Product Not Found");
    delay(1000);
    }
    else
    Serial.println("Removed: Rs. " + price + "\n@ " + product);
  lcd.clear(); // Clears the display 
  lcd.print("Removed:Rs" + price);
  lcd.setCursor(0,1);
  lcd.print("@" + product);
  }
  //Success Buzzer place here.
  tone(buzzer, 8000, 50);
  delay(10);
  tone(buzzer, 5000, 100);
  totalSum(price);
  tagID="";
}

void totalSum(String price){
   if(removeItem == false){
  totalPrice += price.toFloat();
   }else if(removeItem == true){
      totalPrice -= price.toFloat();
      Serial.println("Remove Mode OFF...");
      lcd.clear(); // Clears the display 
      lcd.print("Remove Mode OFF...");
      delay(1000);
   }
  Serial.println("\nTotal Amount: Rs. " + String(totalPrice));
  Serial.println("Scan a Product to add...");
  
   lcd.clear(); // Clears the display 
      lcd.print("Tot Amt:Rs"+ String(totalPrice));
      lcd.setCursor(0,1);
      lcd.print("Scan a Product...");
}

void removeProduct(){
  Serial.println("\nRemove Mode ON...");
  Serial.println("Scan a product to remove...");

  lcd.clear(); // Clears the display 
  lcd.print("Remove Mode ON...");
  lcd.setCursor(0,1);
  lcd.print("Scan to Remove...");
    
  while(!mfrc522.PICC_IsNewCardPresent());
  while(!mfrc522.PICC_ReadCardSerial());
  
  lcd.clear(); // Clears the display 
  lcd.print("Processing");
  lcd.setCursor(0,1);
  lcd.print("Request...");

  Serial.println("Processing Request...");
  tone(buzzer, 5000, 100);

  //mk Serial.print("UID tag: ");
  tagID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    tagID += String(mfrc522.uid.uidByte[i], HEX);
  }
  tagID.toUpperCase();
  //mk Serial.print(tagID);
  Serial.println();
  String deleteResult = sendToServer("GET /Test_site/removeProduct.php?orderId=" + orderId + "&productId=" + tagID);
  removeItem = true;
  //Serial.println(deleteResult);
  trimOutput(deleteResult);
  removeItem = false;
}

void generateBill(){
  Serial.println("Generating Bill...");

  lcd.clear(); // Clears the display 
  lcd.print("Generating Bill...");
  delay(1000);

   lcd.clear(); // Clears the display 
   lcd.print("Tot Amt:Rs"+ String(totalPrice));
   delay(500);
   lcd.clear(); // Clears the display
   delay(200); 
   lcd.print("Tot Amt:Rs"+ String(totalPrice));
   delay(500);
  //Try LCD Blink.

  Serial.println("Order Id:" + orderId + "\n@ Total amount: Rs." + totalPrice);
  lcd.clear(); // Clears the display
  lcd.print("Order Id:");
  lcd.setCursor(0,1);
  lcd.print(orderId);
  //delay(1000);
  tone(buzzer, 10000, 1000);
  Serial.println("Thank you for Shopping.");
  while(1);
}

void sendCommand(String command, int waitCount, char readReply[]) {
  boolean flag = true;
  Serial1.println(command);
  int count = 0;
  while (!Serial1.find(readReply)) {
    count++;
    if (count == waitCount) {
      Serial.println("Fail: " + command);
      flag = false;
      
      if(command.equals("AT+CWJAP=\"" + wifiName + "\",\"" + password + "\"")
      || command.equals("AT+CIPSTART=0,\"TCP\",\"" + serverHost + "\"," + port)){
        Serial.println("Server Down. Please press RESET...");
        while(1){
          tone(buzzer, 1000);
          delay(500);
          noTone(buzzer);
          delay(500);
        }
        //LCD Print here.
      }
      else{
        Serial.println("Error: Please Scan Again.");
        //LCD Print here.
        //Failure Buzzer place here.
      tone(buzzer, 10000, 1000);
      }
      break;
    }
  }
  if(flag){
  //mk Serial.println("Command Success: " + command);
  }
}
