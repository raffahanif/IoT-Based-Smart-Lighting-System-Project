#include <IRremote.h>

#define IR_RECEIVE_PIN 11   // Pin data dari HX1838
#define LED_PIN 7           // Pin LED

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);  // mulai receiver
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  if (IrReceiver.decode()) {
    unsigned long kode = IrReceiver.decodedIRData.command;
    Serial.print("Kode diterima: ");
    Serial.println(kode, HEX);

    // Ganti angka berikut sesuai hasil tombolmu di Serial Monitor
    if (kode == 0x45) {  // contoh tombol '1'
      digitalWrite(LED_PIN, HIGH);
      Serial.println("LED ON");
    } 
    else if (kode == 0x46) {  // contoh tombol '2'
      digitalWrite(LED_PIN, LOW);
      Serial.println("LED OFF");
    }
    else if (kode == 0x47) {
      Serial.println("LED BERKEDIP");
      for(int i = 0; i < 10; i++){
      digitalWrite(LED_PIN, HIGH);
      delay(250);
      digitalWrite(LED_PIN, LOW);
      delay(250);}
    }

    IrReceiver.resume();  // siap menerima sinyal lagi
  }
}