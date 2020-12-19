#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/TRNG.h>
#include <ti/drivers/cryptoutils/cryptokey/CryptoKeyPlaintext.h>

#define KEY_LENGTH_BYTES 16
#define TRNG_INSTANCE 0

/* Board Header file */
#include "ti_drivers_config.h"

void *mainThread(void *arg0)
{
    TRNG_init();
    TRNG_Handle handle;
    int_fast16_t result;
    CryptoKey entropyKey;
    uint8_t entropyBuffer[KEY_LENGTH_BYTES];
    handle = TRNG_open(TRNG_INSTANCE, NULL);

    if (!handle) {
        // Handle error
        while(1);
    }

    CryptoKeyPlaintext_initBlankKey(&entropyKey, entropyBuffer, KEY_LENGTH_BYTES);
    result = TRNG_generateEntropy(handle, &entropyKey);

    if (result != TRNG_STATUS_SUCCESS) {
        // Handle error
        while(1);
    }

    /* Call driver init functions */
    GPIO_init();
    /* Configure the LED pin */
    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    /* Turn off user LED */
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);

    unsigned int random_number = 0;
    int i = 0;
    for (i = 0; i < 16; i++) {
        random_number += entropyBuffer[i];
    }

    srand(random_number);
    while(1) {
        sleep(1 + rand() % 10);
        GPIO_toggle(CONFIG_GPIO_LED_0);
    }
}
