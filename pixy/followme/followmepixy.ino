//Replace delay with millis() and micros()
//DAISY CHAINED Max Sonar
//Added maxSonar class


#include <SPI.h>
#include <Pixy.h>

#define X_CENTER        ((PIXY_MAX_X-PIXY_MIN_X)/2)
#define Y_CENTER        ((PIXY_MAX_Y-PIXY_MIN_Y)/2)

Pixy pixy;

unsigned long currentTimeMillis,currentTimeMicro;
unsigned long previousTime1=0;
unsigned long previousTime2=0;

class ServoLoop
{
public:
        ServoLoop(int32_t pgain, int32_t dgain);

        void update(int32_t error);

        int32_t m_pos;
        int32_t m_prevError;
        int32_t m_pgain;
        int32_t m_dgain;
};

ServoLoop panLoop(500,700);
ServoLoop tiltLoop(700,900);

ServoLoop::ServoLoop(int32_t pgain, int32_t dgain)
{
        m_pos = PIXY_RCS_CENTER_POS;
        m_prevError = 0x80000000L;
        m_pgain = pgain;
        m_dgain = dgain;
}

void ServoLoop::update(int32_t error)
{
        long int vel;
        if(m_prevError!=0x80000000){
                vel = (error * m_pgain + (error - m_prevError) * m_dgain) >> 10;
                m_pos += vel;
                if(m_pos>PIXY_RCS_MAX_POS){
                        m_pos=PIXY_RCS_MAX_POS;
                }
                else if(m_pos<PIXY_RCS_MIN_POS){
                        m_pos=PIXY_RCS_MIN_POS;
                }
        }
        m_prevError = error;
}

class maxSonar
{
public:
        void readSonar(const int pwPin);
        uint16_t pwDistance;
};

maxSonar leftSonar,rightSonar;

void maxSonar::readSonar(const int pwPin)
{
        const int serialSonarPin=7;
        long interval1=20;
        long interval2=100;
        pinMode(serialSonarPin,OUTPUT);
        digitalWrite(serialSonarPin,HIGH);
        if(((currentTimeMicro=micros())-previousTime1)>=interval1){
                digitalWrite(serialSonarPin,LOW);
                previousTime1=micros();
        }
        pinMode(serialSonarPin,INPUT);
        if(((currentTimeMillis=millis())-previousTime2)>=interval2){
                pwDistance=analogRead(pwPin)/2;
                previousTime2=millis();
        }
}

void setup()
{
        Serial.begin(9600);
        pixy.init();
}

void track_object()
{
    uint16_t blocks;
    int32_t panError, tiltError;

    blocks=pixy.getBlocks();

    if (blocks)
    {
        panError=X_CENTER-pixy.blocks[0].x;
        tiltError=pixy.blocks[0].y-Y_CENTER;

        panLoop.update(panError);
        tiltLoop.update(tiltError);

        pixy.setServos(panLoop.m_pos,tiltLoop.m_pos);
    }
}

void loop()
{
        track_object();
        leftSonar.readSonar(A1);
        rightSonar.readSonar(A2);
        Serial.print(leftSonar.pwDistance);
        Serial.print("\t");
        Serial.println(rightSonar.pwDistance);
}
