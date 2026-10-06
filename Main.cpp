#include <Arduino.h>
#include <IRremote.hpp>
#include <ESP32Servo.h> // Veya Standart <Servo.h>

// Pin Tanimlamalari
#define IR_RECEIVE_PIN 15
#define SERVO_PIN      18

// Tuş Kodlari (IR NEC Protokolü)
#define BUTTON_POWER_ON  0xBA45FF00
#define BUTTON_POWER_OFF 0xB847FF00

// Açı Tanimlamalari
#define SERVO_REST_ANGLE  0   // Başlangıç / Dinlenme açısı
#define SERVO_PRESS_ANGLE 60  // Düğmeye basma açısı

Servo lightSwitchServo;

void setup() {
    Serial.begin(115200);
    Serial.println(F("[INFO] IR Light Switch Automation Initializing..."));

    // IR Aliciyi Başlat
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
    Serial.print(F("[INFO] IR Receiver active on Pin: "));
    Serial.println(IR_RECEIVE_PIN);

    // Servo Motoru Bagla ve Sifirla
    lightSwitchServo.attach(SERVO_PIN);
    lightSwitchServo.write(SERVO_REST_ANGLE);
    
    Serial.println(F("[STATUS] System Ready. Awaiting IR Signals..."));
}

void triggerSwitchMechanism() {
    Serial.println(F("[ACTION] Triggering Light Switch Mechanism..."));
    
    // Düğmeye bas
    lightSwitchServo.write(SERVO_PRESS_ANGLE);
    delay(400); // Fiziksel basma süresi
    
    // Eski konumuna geri dön (Reset)
    lightSwitchServo.write(SERVO_REST_ANGLE);
    Serial.println(F("[ACTION] Mechanism Reset to Original Position."));
}

void loop() {
    // IR Sinyal Algilandi mi?
    if (IrReceiver.decode()) {
        uint32_t decodedData = IrReceiver.decodedIRData.decodedRawData;
        Serial.print(F("[IR DATA] Signal Received: 0x"));
        Serial.println(decodedData, HEX);

        // Kumanda tuşu kontrolü
        if (decodedData == BUTTON_POWER_ON || decodedData == BUTTON_POWER_OFF) {
            triggerSwitchMechanism();
        }

        // Bir sonraki sinyali almak için hazirlan
        IrReceiver.resume();
    }
    
    delay(50); // İşlemciyi yormamak için kısa gecikme
}
