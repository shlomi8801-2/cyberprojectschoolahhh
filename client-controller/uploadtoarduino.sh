#compile your code turn it into intel hex(supported by avrdude) and then upload
cd "$PWD"
# /home/shlomilevi/.arduino15/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/bin/avr-g++ -c -g -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -flto -w -x c++ -E -CC -DARDUINO=10819 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR -I/home/shlomilevi/.arduino15/packages/arduino/hardware/avr/1.8.6/cores/arduino -I/home/shlomilevi/.arduino15/packages/arduino/hardware/avr/1.8.6/variants/standard client.cpp -o main -lm -mmcu=atmega328 -DF_CPU=16000000UL -Os -Dpc &&
# avr-g++ client.cpp -o main -lm -Iatmega328p/includes/core -Iatmega328p/includes/ -mmcu=atmega328 -DF_CPU=16000000UL -Os -D__AVR_ATmega328P__ &&
if [ $# -eq 0 ]; then #no input arguments given
avr-g++ client.cpp -o main -lm -Iatmega328p/includes/core -Iatmega328p/includes/ -Os &&
avr-objcopy main -O ihex main.hex &&
avrdude -C atmega328p/prog.conf -v -p atmega328p -carduino -P /dev/ttyUSB0 -b 115200 -D -U flash:w:./main.hex:i;
else
if [ $1 == "-S" ]; then
avr-g++ -S client.cpp -lm -Iatmega328p/includes/core -Iatmega328p/includes/ -Os &&
# avr-g++ -S client.cpp -lm -Os &&
echo "generated client.s file";
fi
fi