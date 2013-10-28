
#define LED_PIN1 12
#define LED_PIN2 13

void setup()
{
    DDRB = 0xFF;
    pinMode(LED_PIN1, OUTPUT);
    pinMode(LED_PIN2, OUTPUT);
}

void loop()
{
    PORTB |= 0x20;
    delay(100);
    PORTB |= 0x00;
    delay(900);
}
