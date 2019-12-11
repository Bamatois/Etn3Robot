#define PIN_SENS_MOTR 8
#define PIN_SENS_MOTL 9
#define PINPWMR 10
#define PINPWML 11

#define TRIGGER_PIN_R  2 // Broche TRIGGER
#define TRIGGER_PIN_L  4 // Broche TRIGGER

#define ECHO_PIN_R 3    // Broche ECHO
#define ECHO_PIN_L 5    // Broche ECHO

#define transistor_R 6 
#define transistor_L 7

#define MAX_SPEED 60

static int State_lumfinder = 0;

/* Constantes pour le timeout */
const unsigned long MEASURE_TIMEOUT = 25000UL; // 25ms = ~8m à 340m/s

/* Vitesse du son dans l'air en mm/us */
const float SOUND_SPEED = 340.0 / 1000;

void setup() {

    /*  Setup pinouts   */
    Serial.begin(9600);
    pinMode(PINPWML,OUTPUT);
    pinMode(PINPWMR,OUTPUT);
    pinMode(PIN_SENS_MOTR,OUTPUT);
    pinMode(PIN_SENS_MOTL,OUTPUT);

    
    pinMode(TRIGGER_PIN_L, OUTPUT);
    digitalWrite(TRIGGER_PIN_L, LOW); // La broche TRIGGER doit être à LOW au repos
    pinMode(ECHO_PIN_L, INPUT);
    
    pinMode(TRIGGER_PIN_R, OUTPUT);
    digitalWrite(TRIGGER_PIN_R, LOW); // La broche TRIGGER doit être à LOW au repos
    pinMode(ECHO_PIN_R, INPUT);
    
    /* Initialise les broches */
    pinMode(transistor_R, OUTPUT);
    digitalWrite(transistor_R, HIGH);
    pinMode(transistor_L, OUTPUT);
    digitalWrite(transistor_L, HIGH);
    
    // init sens de rotation
    digitalWrite(PIN_SENS_MOTR, HIGH);
    digitalWrite(PIN_SENS_MOTL, HIGH);




    
}

void loop() {
    //delay(500);
    Serial.println(F("Start"));
    Serial.println("");
    //Serial.println(LightadjL());
    while (State_lumfinder == 1)// while light isn't strong enough (0)
    {
        //search for light by spinning
        PwrMotorR(50);
        PwrMotorL(-50);
        // light threshold to stop searching
        if (analogRead(A1)<35)
        {
            State_lumfinder = 1;
        }
    }
    /*PwrMotorL(MAX_SPEED-LightadjL());
    PwrMotorR(MAX_SPEED-LightadjR());*/

    PwrMotorL(MAX_SPEED-DistadjL());
    PwrMotorR(MAX_SPEED-DistadjR());
}

int PwrMotorR(int PWM)//    Right motor management
{
    if (PWM>=0)
    {
        digitalWrite(PIN_SENS_MOTR,LOW); //  set forward
    }else
    {
        digitalWrite(PIN_SENS_MOTR,HIGH); //  Set Backward
        PWM = -PWM;
    }
    analogWrite(PINPWMR,PWM*2.56-1); //set pwm
    return 0;
}

int PwrMotorL(int PWM)//    Left motor management
{
    if (PWM>=0)
    {
        digitalWrite(PIN_SENS_MOTL,LOW); //  set forward
    }else
    {
        digitalWrite(PIN_SENS_MOTL,HIGH); //  Set Backward
        PWM = -PWM;
    }
    analogWrite(PINPWML,PWM*2.56-1); //set pwm
    return 0;
}
int LightadjR() // speed adjustment right motor
{
    int Output_value;
    int derivee = Photo_deriv();
    
    if (derivee<0) 
    {
        Output_value = -derivee*10;
    }else Output_value = 0;

    if (Output_value>MAX_SPEED)
    {
        Output_value=MAX_SPEED;
    }
    return Output_value;
}
int Distance_management()
{
    /* 1. Lance une mesure de distance en envoyant une impulsion HIGH de 10µs sur la broche TRIGGER */
    digitalWrite(TRIGGER_PIN_R, HIGH);
    delay(10);
    digitalWrite(TRIGGER_PIN_R, LOW);
    long mesure1 = pulseIn(ECHO_PIN_R, HIGH, MEASURE_TIMEOUT);
  
    /* 2. Mesure le temps entre l'envoi de l'impulsion ultrasonique et son écho (si il existe) */
    if (mesure1==0){
      digitalWrite(transistor_R, LOW);
      delay (10);
      digitalWrite(transistor_R, HIGH);
      mesure1 = pulseIn(ECHO_PIN_R, HIGH, MEASURE_TIMEOUT);
    }
    float distance_mm1 = mesure1 / 2 * SOUND_SPEED;
   /* Serial.print(F("mesure1: "));
    Serial.println  (mesure1);*/
    
    // Affiche les résultats en mm, cm et m 
    Serial.print(F("DistanceR: "));
    Serial.print(distance_mm1/10, 2);
    Serial.println(F("cm, "));
    
    //deuxieme capteur
    digitalWrite(TRIGGER_PIN_L, HIGH);
    delay(10);
    digitalWrite(TRIGGER_PIN_L, LOW);
    long mesure2 = pulseIn(ECHO_PIN_L, HIGH, MEASURE_TIMEOUT);
    if (mesure2==0)
    {
      digitalWrite(transistor_L, LOW);
      delay (10);
      digitalWrite(transistor_L, HIGH);
      mesure2 = pulseIn(ECHO_PIN_L, HIGH, MEASURE_TIMEOUT);
    }
    float distance_mm2 = mesure2 / 2 * SOUND_SPEED;
    /*Serial.print(F("mesure2: "));
    Serial.println  (mesure2);*/
    // Affiche les résultats en mm, cm et m 
    
    Serial.print(F("DistanceL: "));
    Serial.print(distance_mm2/10, 2);
    Serial.println(F("cm, "));
    
    //if derivee positive robot is turning right
    //if derivee negative robot is turning left
    if (distance_mm1 == 0 || distance_mm2 == 0)
    {
      Serial.print(F("Derivee: "));
      Serial.println(F("ERREUR"));
      return 65235;
    }else{
      Serial.print(F("Derivee: "));
      Serial.print(distance_mm1 - distance_mm2);
      Serial.println(F("mm, "));
     return distance_mm1 - distance_mm2;
    }
   
    
}
int DistadjR() // speed adjustment right motor
{
    int Output_value;
    int derivee = Distance_management();
    if (derivee<0) 
    {
        Output_value = -derivee*3;
    }else Output_value = 0;

    if (Output_value>MAX_SPEED)
    {
        Output_value=MAX_SPEED;
    }
    if (derivee==65235) 
    {
        Output_value = MAX_SPEED;
    }
    Serial.print(F("Ajust MR: "));
    Serial.println(Output_value);
    return Output_value;
}
int DistadjL()// speed adjustment left motor
{
    int Output_value;
    int derivee = Distance_management();
    if (derivee>0) 
    {
        Output_value = derivee*3;
    }else Output_value = 0;

    if (Output_value>MAX_SPEED)
    {
        Output_value=MAX_SPEED;
    }
    if (derivee==65235) 
    {
        Output_value = MAX_SPEED;
    }
    Serial.print(F("Ajust ML: "));
    Serial.println(Output_value);
    return Output_value;
}

int Photo_deriv()
{
    /*int SensorValueD = analogRead(A0);
    int SensorValueG = analogRead(A1);*/

    // as the light increase the value of the sensor became closser to 0
    int derivee = analogRead(A0) - analogRead(A1);

    //if derivee positive robot is turning right
    //if derivee negative robot is turning left
    return derivee;
}

int LightadjL()// speed adjustment left motor
{
    int Output_value;
    int derivee = Photo_deriv();
    if (derivee>0) 
    {
        Output_value = derivee*10;
    }else Output_value = 0;

    if (Output_value>MAX_SPEED)
    {
        Output_value=MAX_SPEED;
    }
    return Output_value;
}
