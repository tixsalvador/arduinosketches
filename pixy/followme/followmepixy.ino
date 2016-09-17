#include <SPI.h>
#include <Pixy.h>

#define X_CENTER	((PIXY_MAX_X-PIXY_MIN_X)/2) 	
#define Y_CENTER 	((PIXY_MAX_Y-PIXY_MIN_Y)/2)

Pixy pixy;

class ServoLoop
{
public:
	ServoLoop(int32_t pgain, int32_t dgain);

	int32_t m_pos;
	int32_t m_prevError;
	int32_t m_pgain;
	int32_t m_dgain;
	
	void update()
	{
		Serial.print(m_pgain);
		Serial.print("\t");
		Serial.println(m_dgain);
	}
		
};

ServoLoop panLoop(300,500);
ServoLoop tiltLoop(500,700);

ServoLoop::ServoLoop(int32_t pgain, int32_t dgain)
{
	m_pgain = pgain;
	m_dgain = dgain;
}

void setup()
{
	Serial.begin(9600);
	panLoop.update();
	tiltLoop.update();
}

void loop()
{
	
}

