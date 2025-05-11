import time
import lgpio

THRUST = 19
FREQ = 490

# open the gpio chip and set the LED pin as output
chip = lgpio.gpiochip_open(0)
lgpio.gpio_claim_output(chip, THRUST)

try:
    lgpio.tx_pwm(chip, THRUST, FREQ, 73.5)
    time.sleep(1)

    while True:
        lgpio.tx_pwm(chip, THRUST, FREQ, 83.3)


except KeyboardInterrupt:
    lgpio.gpio_write(chip, THRUST, 0)
    lgpio.gpiochip_close(chip)
