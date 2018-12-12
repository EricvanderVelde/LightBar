#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

#define NUMPIXELS 3

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN_B0, NEO_GRB + NEO_KHZ800);


#include <NmraDcc.h>
#define This_Decoder_Address 3

struct CVPair
{
  uint16_t  CV;
  uint8_t   Value;
};

CVPair FactoryDefaultCVs [] =
{
  // The CV Below defines the Short DCC Address
  {CV_MULTIFUNCTION_PRIMARY_ADDRESS, This_Decoder_Address},

  // These two CVs define the Long DCC Address
  {CV_MULTIFUNCTION_EXTENDED_ADDRESS_MSB, 0},
  {CV_MULTIFUNCTION_EXTENDED_ADDRESS_LSB, This_Decoder_Address},

  // ONLY uncomment 1 CV_29_CONFIG line below as approprate
  {CV_29_CONFIG,                       CV29_F0_LOCATION}, // Short Address 28/128 Speed Steps
};

NmraDcc  Dcc ;

uint8_t FactoryDefaultCVIndex = 0;

// Uncomment this line below to force resetting the CVs back to Factory Defaults
// FactoryDefaultCVIndex = sizeof(FactoryDefaultCVs)/sizeof(CVPair);

void notifyCVResetFactoryDefault()
{
  // Make FactoryDefaultCVIndex non-zero and equal to num CV's to be reset
  // to flag to the loop() function that a reset to Factory Defaults needs to be done
  FactoryDefaultCVIndex = sizeof(FactoryDefaultCVs) / sizeof(CVPair);
};

void setup() {
  pinMode(PIN_B1, OUTPUT);
  pinMode(PIN_B2, INPUT);
  pinMode(PIN_B3, OUTPUT);
  pinMode(PIN_A0, OUTPUT);
  pinMode(PIN_A1, OUTPUT);
  pinMode(PIN_A2, OUTPUT);
  pinMode(PIN_A3, OUTPUT);
  pinMode(PIN_A4, OUTPUT);
  pinMode(PIN_A5, OUTPUT);
  pinMode(PIN_A6, OUTPUT);
  pinMode(PIN_A7, OUTPUT);

  pixels.begin(); // This initializes the NeoPixel library.

  // Setup which External Interrupt, the Pin it's associated with that we're using and enable the Pull-Up
  Dcc.pin(0, PIN_B2, 0);

  // Call the main DCC Init function to enable the DCC Receiver
  Dcc.init( MAN_ID_DIY, 10, FLAGS_MY_ADDRESS_ONLY, 0 );

  // Uncomment to force CV Reset to Factory Defaults
  notifyCVResetFactoryDefault();

  // Timer0 is already used for millis() - we'll just interrupt somewhere
  // in the middle and call the "Compare A" function below
  //OCR0A = 0xAF;
  //TIMSK |= _BV(OCIE0A);
}

void on()
{

  digitalWrite(PIN_B1, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(PIN_B3, HIGH);
  digitalWrite(PIN_A0, HIGH);
  digitalWrite(PIN_A1, HIGH);
  digitalWrite(PIN_A2, HIGH);
  digitalWrite(PIN_A3, HIGH);
  digitalWrite(PIN_A4, HIGH);
  digitalWrite(PIN_A5, HIGH);
  digitalWrite(PIN_A6, HIGH);
  digitalWrite(PIN_A7, HIGH);
}

void off()
{
  digitalWrite(PIN_B1, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(PIN_B3, LOW);
  digitalWrite(PIN_A0, LOW);
  digitalWrite(PIN_A1, LOW);
  digitalWrite(PIN_A2, LOW);
  digitalWrite(PIN_A3, LOW);
  digitalWrite(PIN_A4, LOW);
  digitalWrite(PIN_A5, LOW);
  digitalWrite(PIN_A6, LOW);
  digitalWrite(PIN_A7, LOW);
}

int lights=0;
int disco=0;

void notifyDccFunc(uint16_t Addr, DCC_ADDR_TYPE AddrType, FN_GROUP FuncGrp, uint8_t FuncState)
{
    if (FuncGrp == FN_GROUP::FN_0_4)
    {
        if (FuncState & FN_BIT_00)
        {
            lights=1;
        }
        else
        {
            lights=0;
        }

        if (FuncState & FN_BIT_01)
        {
          disco = 1;
        }
        else
        {
          disco = 0;
        }

  }
}

// Interrupt is called once a millisecond,
//SIGNAL(TIMER0_COMPA_vect)
//{
//}

unsigned long previousMicros = 0;
unsigned long previousDiscoMillis = 0;


// constants won't change :

void loop()
{
    static int lightstatus = 0; // 0 = off, 1 = on

    Dcc.process();

    if  (FactoryDefaultCVIndex && Dcc.isSetCVReady())
    {
    	FactoryDefaultCVIndex--; // Decrement first as initially it is the size of the array
    	Dcc.setCV( FactoryDefaultCVs[FactoryDefaultCVIndex].CV, FactoryDefaultCVs[FactoryDefaultCVIndex].Value);
    }

    if  (lights==0)
    {
    	off();
    }

    if  (lights==1)
    {
    	unsigned long currentMicros = micros();
    	if ( lightstatus == 0 && currentMicros - previousMicros >= 850)
    	{
            previousMicros = currentMicros;
            lightstatus = 1;
            on();
      	}
        if ( lightstatus == 1 && currentMicros - previousMicros >= 10)
        {
            previousMicros = currentMicros;
            lightstatus = 0;
            off();
        }
    }

    if (disco == 1) {

      unsigned long currentDiscoMillis = millis();
      if (currentDiscoMillis - previousDiscoMillis >= random(250,500))
      {
        previousDiscoMillis = currentDiscoMillis;
        for(int i=0;i<NUMPIXELS;i++){
            int d = random(5);

            switch (d)
            {
		case 0: pixels.setPixelColor(i, 0  , 0  , 255); break;  // blue
		case 1: pixels.setPixelColor(i, 0  , 255, 0); break; 	// green
		case 2: pixels.setPixelColor(i, 255, 0  , 0); break; 	// red
		case 3: pixels.setPixelColor(i, 127, 0  , 255); break;  // purple
		case 4: pixels.setPixelColor(i, 255, 128, 0); break;	// yellow
            } 

        } 
      pixels.setBrightness(18);
      pixels.show();
      }
    }
    else
    {
      unsigned long currentDiscoMillis = millis();
      if (currentDiscoMillis - previousDiscoMillis >= 100)
      {
        previousDiscoMillis = currentDiscoMillis;
        for(int i=0;i<NUMPIXELS;i++){
            pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright green color.
        }
        pixels.show();
      }
    }
}
