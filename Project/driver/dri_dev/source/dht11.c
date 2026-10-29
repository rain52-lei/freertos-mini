#include "dht11.h"

static void dht11_start();
static uint8_t dht11_check_response();
static uint8_t dht11_read_bit();
static uint8_t dht11_read_byte();

void dht11_init()
{
    RCC->AHB1ENR |= (1<<6);

    GPIOG->MODER &= ~(3<<18);
    GPIOG->MODER |= (1<<18);

    GPIOG->OTYPER &= ~(1<<9);
    GPIOG->PUPDR &= ~(3<<18);

    GPIOG->ODR |= (1<<9);
}

static void dht11_start()
{
    GPIOG->MODER &= ~(3<<18);
    GPIOG->MODER |= (1<<18);

    GPIOG->ODR &= ~(1<<9);
    delay_ms(20);    

    GPIOG->ODR |= (1<<9);
    delay_us(30);

    GPIOG->MODER &= ~(3<<18);
}

static uint8_t dht11_check_response()
{
    uint32_t timeout = 0;

    while((GPIOG->IDR &(1<<9)) !=0)
    {
        timeout++;
        delay_us(1);
        if(timeout > 100) return 1;
    }

    timeout = 0;

    while((GPIOG->IDR &(1<<9)) == 0)
    {
        timeout++;
        delay_us(1);
        if(timeout > 100) return 1;
    }

    timeout = 0;

    while((GPIOG->IDR & (1<<9)) != 0)
    {
        timeout++;
        delay_us(1);
        if(timeout > 100) return 1;
    }

    return 0;
}

static uint8_t dht11_read_bit()
{
    uint32_t timeout = 0;

    while((GPIOG-> IDR & (1<<9)) == 0)
    {
        timeout++;
        delay_us(1);
        if(timeout > 100) return 0;
    }

    delay_us(40);

    if(GPIOG->IDR & (1<<9))
    {
        timeout = 0;
        while((GPIOG->IDR & (1<<9)) != 0)
        {
            timeout++;
            delay_us(1);
            if(timeout > 100) break;
        }
        return 1;
     }
    else
    {
        return 0;
    }
}


static uint8_t dht11_read_byte()
{
    uint8_t i;
    uint8_t data = 0;

    for(i = 0;i < 8;i++)
    {
        data <<= 1;
        data |= dht11_read_bit();
    }
    return data;
}

uint8_t dht11_read_data(uint8_t *temp, uint8_t *humi)
{
    uint8_t buf[5];
    uint8_t i;

    dht11_start();

    if(dht11_check_response())
    {
        return 1;
    }

    for(i = 0;i<5;i++)
    {
        buf[i] = dht11_read_byte();
    }

    if(buf[0] + buf[1] + buf[2] + buf[3] == buf[4])
    {
        *humi = buf[0];
        *temp = buf[2];
        return 0;
    }
    else
    {
        return 1;
    }
}

