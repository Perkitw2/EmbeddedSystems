// 5 pins trip wire

// Variables
int mode = 0; //Starts device in calibration mode 
int ambiant;
int laser = 1000; // The light value I get when using my laser
int minLight = 900;
int makeBeep = 1; //0 for no beep, 1 for beep!
int atAverage;
int tripped;
long millisCount;

// Output Pins
int laserPin = 2; //Laser emitter
int alertPin = 12; //LED that shows if laser has been tripped
int ledPin = 13; //LED that shows if the laser is working during calibration mode
int buzzerPin = 3; //Buzzer that goes off when the laser is tripped
String  modeNames[3] = {"CALIBRATION","ARMED","TRIPPED"}; // array of the different modes

// Input Pins
int modePin = 4; 
int tripPin = A0;
int ambiantPin = A1;


void setup() {
  pinMode(alertPin, OUTPUT);
  pinMode(laserPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(modePin, INPUT);
  Serial.begin(9600);
}


void loop() {

  // Changes modes when the button is pushed
  if (digitalRead(modePin) == 1) {
    laser = analogRead(tripPin);
    mode=mode + 1;
    if (mode >= 2) {
      mode = 0;
    }
    beep(1);
    delay(300);
    digitalWrite(ledPin,LOW);
    digitalWrite(alertPin,LOW);
    tripped = 0;
  }

  
  //sets up the different modes and allows for them to be switched between
  switch (mode) {
    case 0: //calibration
      digitalWrite(laserPin,HIGH);
      
      ambiant = analogRead(ambiantPin);
      laser = analogRead(tripPin);
      atAverage = ambiant + ((laser - ambiant)/2);
      stats();
      
      if (laser  >= minLight) {
        digitalWrite(ledPin,HIGH);
      } else {
        digitalWrite(ledPin,LOW);
      }
    break;

    case 1: // Armed
      digitalWrite(laserPin,HIGH);
      digitalWrite(ledPin,LOW);
      ambiant = analogRead(ambiantPin);
      atAverage = ambiant + ((laser - ambiant)/2);
      if (analogRead(tripPin) < atAverage) {
        mode = 2;
        tripped = 1;
      }
      if ((millis() - millisCount) >= 3000) {
        millisCount = millis();
        stats();
      }
      if (tripped > 0){
        digitalWrite(alertPin, HIGH);
      }
    break;
    
    case 2: //Triped
      if ((millis() - millisCount) >= 1000) {
        millisCount = millis();
        stats();
        beep(3);
        mode = 1;
        tripped = 1;
      }
      if (tripped > 0){
        digitalWrite(alertPin, HIGH);
      }
    break;
    }

    
  delay(1);
}

//Beeps while laser is broken
void beep(int qty) {
  int count;
  if (makeBeep == 1) {
    for (count = 1;count<=qty;count++) {
      digitalWrite(buzzerPin, HIGH);
      delay(50);
      digitalWrite(buzzerPin, LOW);
      delay(50);
    }
  }
}

//Writes info out to the serial port
void stats() {
  Serial.print("Ambiant:");
  Serial.print(ambiant);
  Serial.print(" Average:");
  Serial.print(atAverage);
  Serial.print(" MODE:");
  Serial.print(modeNames[mode]);
  Serial.print(" status LED:");
  Serial.print(tripped);
  Serial.println("");
}
