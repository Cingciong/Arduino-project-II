#include <Arduino.h>
#include <IRremote.h>
#include <Stepper.h>

#define IR_RECEIVE_PIN 2
#define MAX_SPEED 50
#define MAX_ACCELERATION 50
#define STEPS_PER_REVOLUTION 2038

//IR
void commands();
void receiveIRData();

//Stepper
int stepCounter = 0;

//presetup

Stepper myStepper = Stepper(STEPS_PER_REVOLUTION, 8, 10, 9, 11);

void motor(){
    myStepper.setSpeed(10);
    myStepper.step(500);
}

void setup() {


    Serial.begin(9600); // Start the serial communication
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start the receiver

}



void loop() {

    receiveIRData();
}

void commands() {
    // Define the values for the IR commands
#define FAN_SPEED 0x0F807
#define TURINGING_OFF 0x0FC03
#define TIMER 0x0F40B
#define OPERATING_STATE 0x0F00F
#define POWER 0xBA45FF00
#define MOVEMENT_PLUS 0xB946FF00
#define MOVEMENT_MINUS 0xEA15FF00


    // Perform some action based on the decoded data
    Serial.println("IR command received: " + String(IrReceiver.decodedIRData.decodedRawData, HEX));
    //8 = NEC protocol
    if(IrReceiver.decodedIRData.protocol == 8) {
        Serial.println("NEC protocol");
        switch (IrReceiver.decodedIRData.decodedRawData) {
            case POWER:
                Serial.println("Power command received");
                break;
            case MOVEMENT_PLUS:
                Serial.println("Movement plus command received");
                myStepper.setSpeed(10);
                myStepper.step(3000);
                break;
            case MOVEMENT_MINUS:
                Serial.println("Movement minus command received");
                myStepper.setSpeed(10);
                myStepper.step(-3000);
                break;
            default:
                Serial.println("Unknown IR command");
                Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
                break;
        }
    }
    //2 = PulseDistance protocol
    else if(IrReceiver.decodedIRData.protocol == 2) {
        Serial.println("PulseDistance protocol");
        switch (IrReceiver.decodedIRData.decodedRawData) {
            case FAN_SPEED:
                Serial.println("Fan speed command received");
                break;
            case TURINGING_OFF:
                Serial.println("Turning off command received");
                break;
            case TIMER:
                Serial.println("Timer command received");
                break;
            case OPERATING_STATE:
                Serial.println("Operating state command received");
                break;
            default:
                Serial.println("Unknown IR command");
                Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
                break;
        }
    }
}

void receiveIRData() {
    if (IrReceiver.decode()) {
        Serial.println("xxxxxxxxxxxxxxxxxxxxxxx");
       // IrReceiver.printIRResultShort(&Serial); // Print complete received data in one line
       // Serial.println("--------");
       // IrReceiver.printIRSendUsage(&Serial);   // Print the statement required to send this data
        commands();
        IrReceiver.resume();
    }
}
