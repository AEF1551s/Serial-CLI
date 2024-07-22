#include <GPIO.h>

// Constructor with all data for pins
GPIO::GPIO(LedPin led1, LedPin led2, LedPin led3, LedPin led4)
{
    this->led1_ = led1;
    this->led2_ = led2;
    this->led3_ = led3;
    this->led4_ = led4;
}

void GPIO::initLedPins()
{

}
