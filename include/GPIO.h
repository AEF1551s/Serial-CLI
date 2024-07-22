#if !defined(GPIO_H)
#define GPIO_H



class GPIO
{
private:
    /* data */
public:
    GPIO();
    void initLedPins(LedPin led1, LedPin led2,LedPin led3,LedPin led4);
};

#endif // GPIO_H
