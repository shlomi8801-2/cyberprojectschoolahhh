#compile your code turn it into intel hex(supported by avrdude) and then upload
cd "$PWD"
# /home/shlomilevi/.arduino15/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/bin/avr-g++ -c -g -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -flto -w -x c++ -E -CC -DARDUINO=10819 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR -I/home/shlomilevi/.arduino15/packages/arduino/hardware/avr/1.8.6/cores/arduino -I/home/shlomilevi/.arduino15/packages/arduino/hardware/avr/1.8.6/variants/standard client.cpp -o main -lm -mmcu=atmega328 -DF_CPU=16000000UL -Os -Dpc &&
# avr-g++ client.cpp -o main -lm -Iatmega328p/includes/core -Iatmega328p/includes/ -mmcu=atmega328 -DF_CPU=16000000UL -Os -D__AVR_ATmega328P__ &&
FLAGS="-w -o main -Iatmega328p/includes/core -Iatmega328p/includes/ -lm -w -g -flto -fuse-linker-plugin -mmcu=atmega328 -DF_CPU=16000000UL -Os -Wl,--gc-sections -fdata-sections -ffunction-sections -D_SS_MAX_RX_BUFF=256"
ARDUINOPORT="/dev/ttyUSB0"
# ARDUINOPORT="/dev/ttyACM0"
sudo chmod 777 $ARDUINOPORT
if [ $# -eq 0 ]; then #no input arguments given
# echo "$(find atmega328p/includes/. -type f -iregex ".*\.cpp") $(find atmega328p/includes/. -type f -iregex ".*\.c") $FLAGS"
avr-g++ *.cpp $(find atmega328p/includes/. -type f -regextype awk -regex ".*\.(c|cpp)$") $FLAGS || exit
avr-objcopy main -O ihex main.hex || exit
SIZE=$(avr-size main | tail -1)
rm ./main;

fileSize=$(($(($(echo $SIZE | cut -d " " -f 1))) +$(($(echo $SIZE | cut -d " " -f 2))) ))
echo "sketch hex file size: $fileSize" &&
if ((fileSize < 30720)); then
avrdude -C atmega328p/prog.conf -v -p atmega328p -carduino -P $ARDUINOPORT -b 115200 -D -U flash:w:./main.hex:i;
rm ./main.hex;
fi
else
if [ $1 == "-S" ]; then
avr-g++ -S client.cpp -Iatmega328p/includes/core $FLAGS  &&
# avr-g++ -S client.cpp -lm -Os &&
echo "generated client.s file";
fi
fi


# /app/Arduino/arduino-builder -dump-prefs -logger=machine -hardware /app/Arduino/hardware -tools /app/Arduino/tools-builder -tools /app/Arduino/hardware/tools/avr -built-in-libraries /app/Arduino/libraries -libraries /home/shlomi/Arduino/libraries -fqbn=arduino:avr:uno -ide-version=10819 -build-path /tmp/arduino_build_351141 -warnings=none -build-cache /tmp/arduino_cache_901949 -prefs=build.warn_data_percentage=75 -prefs=runtime.tools.arduinoOTA.path=/app/Arduino/hardware/tools/avr -prefs=runtime.tools.arduinoOTA-1.3.0.path=/app/Arduino/hardware/tools/avr -prefs=runtime.tools.avr-gcc.path=/app/Arduino/hardware/tools/avr -prefs=runtime.tools.avr-gcc-7.3.0-atmel3.6.1-arduino7.path=/app/Arduino/hardware/tools/avr -prefs=runtime.tools.avrdude.path=/app/Arduino/hardware/tools/avr -prefs=runtime.tools.avrdude-6.3.0-arduino17.path=/app/Arduino/hardware/tools/avr -verbose /tmp/untitled1267290514.tmp/sketch_jan07a/sketch_jan07a.ino
# /app/Arduino/arduino-builder -compile -logger=machine -hardware /app/Arduino/hardware -tools /app/Arduino/tools-builder -tools /app/Arduino/hardware/tools/avr -built-in-libraries /app/Arduino/libraries -libraries /home/shlomi/Arduino/libraries -fqbn=arduino:avr:uno -ide-version=10819 -build-path /tmp/arduino_build_351141 -warnings=none -build-cache /tmp/arduino_cache_901949 -prefs=build.warn_data_percentage=75 -prefs=runtime.tools.arduinoOTA.path=/app/Arduino/hardware/tools/avr -prefs=runtime.tools.arduinoOTA-1.3.0.path=/app/Arduino/hardware/tools/avr -prefs=runtime.tools.avr-gcc.path=/app/Arduino/hardware/tools/avr -prefs=runtime.tools.avr-gcc-7.3.0-atmel3.6.1-arduino7.path=/app/Arduino/hardware/tools/avr -prefs=runtime.tools.avrdude.path=/app/Arduino/hardware/tools/avr -prefs=runtime.tools.avrdude-6.3.0-arduino17.path=/app/Arduino/hardware/tools/avr -verbose /tmp/untitled1267290514.tmp/sketch_jan07a/sketch_jan07a.ino
# Using board 'uno' from platform in folder: /app/Arduino/hardware/arduino/avr
# Using core 'arduino' from platform in folder: /app/Arduino/hardware/arduino/avr
# Detecting libraries used...
# /app/Arduino/hardware/tools/avr/bin/avr-g++ -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -flto -w -x c++ -E -CC -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10819 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR -I/app/Arduino/hardware/arduino/avr/cores/arduino -I/app/Arduino/hardware/arduino/avr/variants/standard /tmp/arduino_build_351141/sketch/sketch_jan07a.ino.cpp -o /dev/null
# Generating function prototypes...
# /app/Arduino/hardware/tools/avr/bin/avr-g++ -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -flto -w -x c++ -E -CC -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10819 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR -I/app/Arduino/hardware/arduino/avr/cores/arduino -I/app/Arduino/hardware/arduino/avr/variants/standard /tmp/arduino_build_351141/sketch/sketch_jan07a.ino.cpp -o /tmp/arduino_build_351141/preproc/ctags_target_for_gcc_minus_e.cpp
# /app/Arduino/tools-builder/ctags/5.8-arduino11/ctags -u --language-force=c++ -f - --c++-kinds=svpf --fields=KSTtzns --line-directives /tmp/arduino_build_351141/preproc/ctags_target_for_gcc_minus_e.cpp
# Compiling sketch...
# /app/Arduino/hardware/tools/avr/bin/avr-g++ -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10819 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR -I/app/Arduino/hardware/arduino/avr/cores/arduino -I/app/Arduino/hardware/arduino/avr/variants/standard /tmp/arduino_build_351141/sketch/sketch_jan07a.ino.cpp -o /tmp/arduino_build_351141/sketch/sketch_jan07a.ino.cpp.o
# Compiling libraries...
# Compiling core...