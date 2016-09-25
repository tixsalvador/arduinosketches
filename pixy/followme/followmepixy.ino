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

void setup()
{
        Serial.begin(9600);
        pixy.init();
<<<<<<< HEAD
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
                Serial.println(tiltLoop.m_pos);
        }
    }
=======
>>>>>>> c0c2fc928f5b1fb08151a310917124184a86f26e
}

void track_object()
{
<<<<<<< HEAD
        //track_object();

        int pwPin=6;
        int pwPulse[5];
	int pulse=0;
	uint8_t pwDistance;

        pinMode(pwPin,INPUT);
	for(int i=0;i<5;i++){
        	pwPulse[i]=pulseIn(pwPin,HIGH);
        	pwPulse[i]=pwPulse[i]/147;
		pulse+=pwPulse[i];
        	delay(10);
	}
	pwDistance=pulse/5;
	Serial.println(pwDistance);
=======
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
                Serial.println(tiltLoop.m_pos);
        }
    }
>>>>>>> c0c2fc928f5b1fb08151a310917124184a86f26e
}

void loop()
{
        //track_object();

        int pwPin=7;
        int pwPulse[10];
        uint8_t  pulse=0;

        pinMode(pwPin,INPUT);
        for(int i=0;i<9;i++){
                pwPulse[i]=pulseIn(pwPin,HIGH);
                pwPulse[i]=pwPulse[i]/147;
                pulse += pwPulse[i];
                delay(10);
        }
        pulse=pulse/10;
        Serial.println(pulse);
}
