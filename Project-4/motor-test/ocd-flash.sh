#!/bin/bash
cd /home/florian/pico/pico-projects/pwm--motor-test/build
openocd -f interface/raspberrypi-swd.cfg -f target/rp2040.cfg -c "program pwm--motor-test.elf verify reset exit"
