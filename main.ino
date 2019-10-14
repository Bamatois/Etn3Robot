#define PINSENSR 8
#define PINSENSL 7
#define PINPWMR 10
#define PINPWML 11


void setup() {

    /*  Setup pinouts   */
    pinMode(PINPWML,OUTPUT);
    pinMode(PINPWMR,OUTPUT);
    pinMode(PINSENSR,OUTPUT);
    pinMode(PINSENSL,OUTPUT);
    digitalWrite(PINSENSR, HIGH);
    digitalWrite(PINSENSL, HIGH);
}

void loop() {
    int i=0;                 //increment
    PwrMotorL(25);
    PwrMotorR(-75);
    /*
    do
    {
        delay(250);
        PwrMotorL(i);
        PwrMotorR(i);
        i++;
    } while (i<=100);*/


}

int PwrMotorR(int PWM)//    Right motor management
{
    if (PWM>=0)
    {
        digitalWrite(PINSENSR,LOW); //  set forward
    }else
    {
        digitalWrite(PINSENSR,HIGH); //  Set Backward
        PWM = -PWM;
    }
    analogWrite(PINPWMR,PWM*2.56-1); //set pwm
    return 0;
}

int PwrMotorL(int PWM)//    Left motor management
{
    if (PWM>=0)
    {
        digitalWrite(PINSENSL,LOW); //  set forward
    }else
    {
        digitalWrite(PINSENSL,HIGH); //  Set Backward
        PWM = -PWM;
    }
    analogWrite(PINPWML,PWM*2.56-1); //set pwm
    return 0;
}