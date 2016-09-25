#include <SPI.h>
#include <Pixy.h>

#define X_CENTER        ((PIXY_MAX_X-PIXY_MIN_X)/2)
#define Y_CENTER        ((PIXY_MAX_Y-PIXY_MIN_Y)/2)

Pixy pixy;

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

ServoLoop panLoop(300,500);
ServoLoop tiltLoop(500,700);

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
	uint8_t pwDistance;
	
};

maxSonar leftSonar,rightSonar;

void maxSonar::readSonar(const int pwPin)
{
	int pwPulse[5];
	int pulse=0;
		
	pinMode(pwPin,INPUT);
	for(int i=0;i<5;i++){
		pwPulse[i]=pulseIn(pwPin,HIGH);
		pwPulse[i]=pwPulse[i]/147;
		pulse+=pwPulse[i];
		delay(10);
	}
	pwDistance=pulse/5;
}

void setup()
{
        Serial.begin(9600);
        pixy.init();
}

void track_object()
{
    static int i=0;
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
	
        i++;

        if(i%50==0){
		Serial.print(panLoop.m_pos);
        	Serial.print("\t");
        	Serial.print(tiltLoop.m_pos);
        	Serial.print("\t");
        	Serial.print(leftSonar.pwDistance);
        	Serial.print("\t");
        	Serial.println(rightSonar.pwDistance);
        }
    }
}

void loop()
{
        track_object();
	leftSonar.readSonar(7);
        rightSonar.readSonar(6);
}

