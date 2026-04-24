#define BLYNK_TEMPLATE_ID "TMPL6nzk2OdQ3"
#define BLYNK_TEMPLATE_NAME "Automatic Light Project"
#define BLYNK_AUTH_TOKEN "ok3B8ZZpSKeFlO-_o3KOTh-zPFbBNnH9"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

#define IR_RECEIVE_PIN D5
#define LED_PIN D2
#define LAMPU_PIN D6
#define SENSOR_PIN A0

// KODE HEX REMOTE
unsigned long kodeManualON  = 0x45;
unsigned long kodeManualOFF = 0x46;

// SET MODE
enum Mode { OTOMATIS, MANUAL };
Mode modeSekarang = OTOMATIS;

// NILAI BATAS TERANG & GELAP
int batasTerang = 1000;
int batasGelap  = 1023;

char ssid[] = "HOME MAMAH LT2";
char pass[] = "sangkanhurip02";

// SET IR Remote
IRrecv irrecv(IR_RECEIVE_PIN);
decode_results results;

// SET STATUS
int lastStatusLampu = -1;

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(LAMPU_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  analogWriteRange(1023);

  irrecv.enableIRIn();
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  Serial.println("Sistem aktif dalam Mode OTOMATIS");

  // Status awal mode otomatis
  Blynk.virtualWrite(V2, 1);
}

void loop() {
  Blynk.run();

  // melakukan perubahan mode
  if (irrecv.decode(&results)) {
    unsigned long kode = results.command;
    Serial.printf("Kode IR: %lx\n", kode);

    if (kode == kodeManualON) {
      modeSekarang = MANUAL;
      digitalWrite(LED_PIN, HIGH);
      Serial.println("MODE MANUAL (Remote)");

      analogWrite(LAMPU_PIN, 1023);
      lastStatusLampu = 2;
      Blynk.virtualWrite(V1, 2);
      Blynk.virtualWrite(V2, 2); 
    }
    else if (kode == kodeManualOFF) {
      modeSekarang = OTOMATIS;
      digitalWrite(LED_PIN, LOW);
      Serial.println("MODE OTOMATIS (Remote)");
      Blynk.virtualWrite(V2, 1);
    }

    irrecv.resume();
  }

  // MODE OTOMATIS
  if (modeSekarang == OTOMATIS) {

    int nilai = analogRead(SENSOR_PIN);
    Serial.printf("Sensor: %d\n", nilai);
    Blynk.virtualWrite(V0, nilai);
    int pwm = 0;
    int statusLampu = 0;

    if (nilai > batasGelap) {
      pwm = 1023;
      statusLampu = 2;
    } 
    else if (nilai > batasTerang) {
      pwm = map(nilai, batasTerang, batasGelap, 0, 800);
      statusLampu = 1;
    }
    else {
      pwm = 0;
      statusLampu = 0;
    }
    analogWrite(LAMPU_PIN, pwm);
    if (statusLampu != lastStatusLampu) {
      lastStatusLampu = statusLampu;
      Blynk.virtualWrite(V1, statusLampu);
    }
  }

  //MODE MANUAL
  else {
    analogWrite(LAMPU_PIN, 1023);

    if (lastStatusLampu != 2) {
      lastStatusLampu = 2;
      Blynk.virtualWrite(V1, 2);
    }
  }
  delay(50);
}
