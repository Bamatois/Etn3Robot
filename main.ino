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

#define MAX_SPEED 30
#define ADJ_DIST 0.7
#define VIR_DETECT 60

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
 // Serial.println(return_dist_deriv());
  Serial.println(analogRead(A1));
  
    
    while (State_lumfinder == 0)// while light isn't strong enough (0)
    {
        //search for light by spinning
        PwrMotorR(10);
        PwrMotorL(-10);
        // light threshold to stop searching
        if (analogRead(A1)<60)
        {
            State_lumfinder = 1;
        }
    }/*
    if( analogRead(A1)<60 && analogRead(A1)<60 && return_dist_deriv() > 600 )
    {
      PwrMotorL(MAX_SPEED-LightadjL());
      PwrMotorR(MAX_SPEED-LightadjR());
    }
    
    
    int speedR,speedL;
    Distance_management(&speedL,&speedR);*/
}
int return_dist_deriv()
{
  long mesure1 = 0;
    long mesure2 = 0;
    float distance_mm1 = 0;
    float distance_mm2  = 0;
    int derivee = 0;
    while( mesure1 == 0 || mesure2 ==0)
    {
      /* 1. Lance une mesure de distance en envoyant une impulsion HIGH de 10µs sur la broche TRIGGER */
      digitalWrite(TRIGGER_PIN_R, HIGH);
      delay(3);
      digitalWrite(TRIGGER_PIN_R, LOW);
      mesure1 = pulseIn(ECHO_PIN_R, HIGH, MEASURE_TIMEOUT);
    
      /* 2. Mesure le temps entre l'envoi de l'impulsion ultrasonique et son écho (si il existe) */
      
      distance_mm1 = mesure1 / 2 * SOUND_SPEED;
      // Affiche les résultats en mm, cm et m 
      /*Serial.print(F("DistanceR: "));
      Serial.print(distance_mm1/10, 2);
      Serial.println(F("cm, "));*/
      
      //deuxieme capteur
      digitalWrite(TRIGGER_PIN_L, HIGH);
      delay(3);
      digitalWrite(TRIGGER_PIN_L, LOW);
      mesure2 = pulseIn(ECHO_PIN_L, HIGH, MEASURE_TIMEOUT);
      
      distance_mm1 = mesure1 / 2 * SOUND_SPEED;
      distance_mm2 = mesure2 / 2 * SOUND_SPEED;
      // Affiche les résultats en mm, cm et m 
      /*Serial.print(F("DistanceL: "));
      Serial.print(distance_mm2/10, 2);
      Serial.println(F("cm, "));*/
    }
    //if derivee positive robot is turning right
    //if derivee negative robot is turning left
    Serial.print(F("Derivee: "));
    Serial.print(distance_mm1 - distance_mm2);
    Serial.println(F("mm, "));
    derivee = (int)(distance_mm1 -distance_mm2);
    return derivee;
}

int PwrMotorL(int PWM)//    Right motor management
{
    if (PWM>=0)
    {
        digitalWrite(PIN_SENS_MOTR,LOW); //  set forward
    }else
    {
        digitalWrite(PIN_SENS_MOTR,HIGH); //  Set Backward
        PWM = -PWM;
    }
    
    analogWrite(PINPWMR,int(PWM*2.55)); //set pwm
    return 0;
}

int PwrMotorR(int PWM)//    Left motor management
{
    if (PWM>=0)
    {
        digitalWrite(PIN_SENS_MOTL,LOW); //  set forward
    }else
    {
        digitalWrite(PIN_SENS_MOTL,HIGH); //  Set Backward
        PWM = -PWM;
    }
    analogWrite(PINPWML,(int)(PWM*2.55)); //set pwm
    return 0;
}
int LightadjR() // speed adjustment right motor
{
    int Output_value;
    int derivee = Photo_deriv();
    
    if (derivee<0) 
    {
        Output_value = -derivee*20;
    }else Output_value = 0;

    if (Output_value>MAX_SPEED)
    {
        Output_value=MAX_SPEED;
    }
    return Output_value;
}
void Distance_management(int * speedL,int* speedR)
{
    long mesure1 = 0;
    long mesure2 = 0;
    float distance_mm1 = 0;
    float distance_mm2  = 0;
    int derivee = 0;
    while( mesure1 == 0 || mesure2 ==0)
    {
      /* 1. Lance une mesure de distance en envoyant une impulsion HIGH de 10µs sur la broche TRIGGER */
      digitalWrite(TRIGGER_PIN_R, HIGH);
      delay(3);
      digitalWrite(TRIGGER_PIN_R, LOW);
      mesure1 = pulseIn(ECHO_PIN_R, HIGH, MEASURE_TIMEOUT);
    
      /* 2. Mesure le temps entre l'envoi de l'impulsion ultrasonique et son écho (si il existe) */
      
      distance_mm1 = mesure1 / 2 * SOUND_SPEED;
      // Affiche les résultats en mm, cm et m 
      /*Serial.print(F("DistanceR: "));
      Serial.print(distance_mm1/10, 2);
      Serial.println(F("cm, "));*/
      
      //deuxieme capteur
      digitalWrite(TRIGGER_PIN_L, HIGH);
      delay(3);
      digitalWrite(TRIGGER_PIN_L, LOW);
      mesure2 = pulseIn(ECHO_PIN_L, HIGH, MEASURE_TIMEOUT);
      
      distance_mm1 = mesure1 / 2 * SOUND_SPEED;
      distance_mm2 = mesure2 / 2 * SOUND_SPEED;
      // Affiche les résultats en mm, cm et m 
      /*Serial.print(F("DistanceL: "));
      Serial.print(distance_mm2/10, 2);
      Serial.println(F("cm, "));*/
    }
    //if derivee positive robot is turning right
    //if derivee negative robot is turning left
    Serial.print(F("Derivee: "));
    Serial.print(distance_mm1 - distance_mm2);
    Serial.println(F("mm, "));
    derivee = (int)(distance_mm1 -distance_mm2);
    //CircularData(derivee);
    if( derivee < 0)
    {
      if( (derivee * ADJ_DIST) < -MAX_SPEED){
      PwrMotorL(0);
      PwrMotorR(MAX_SPEED);
      }else
      {
        PwrMotorL(MAX_SPEED + (int)derivee * ADJ_DIST);
        PwrMotorR(MAX_SPEED);
      }
    }else
    {
      if(derivee * ADJ_DIST >= MAX_SPEED){
      PwrMotorR(0);
      PwrMotorL(MAX_SPEED);

      }else
      {
        PwrMotorR(MAX_SPEED - (int)derivee*ADJ_DIST);
        PwrMotorL(MAX_SPEED);
      }
    }
    return 0;
}
char CircularData( int derivee)
{
  static int Circular[4];
  static int i;
  i=(i+1) %4;
  if (Circular[i]>Circular[(i+3)%4]+VIR_DETECT)
  {
    Serial.println("sup");
    while (Circular[i]>Circular[(i+3)%4]+VIR_DETECT)
    {
      PwrMotorL(MAX_SPEED/2);
      PwrMotorR(MAX_SPEED);
    }
    PwrMotorL(MAX_SPEED/2);
    PwrMotorR(MAX_SPEED);
    delay(400);
  }
  if (Circular[i]<Circular[(i+3)%4]-VIR_DETECT)
  {
    Serial.println("inf");
    while (Circular[i]>Circular[(i+3)%4]+VIR_DETECT)
    {
      PwrMotorR(MAX_SPEED/2);
      PwrMotorL(MAX_SPEED);
    }
    PwrMotorR(MAX_SPEED/2);
    PwrMotorL(MAX_SPEED);
    delay(400);
  }
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
        Output_value = derivee*20;
    }else Output_value = 0;

    if (Output_value>MAX_SPEED)
    {
        Output_value=MAX_SPEED;
    }
    return Output_value;
}
