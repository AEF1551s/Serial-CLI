cmake_minimum_required(VERSION 3.20)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/toolchain.cmake)

project(MikroTik_IoT_test)

#Build options -------------------------------------------------
# Default values
set(DEFAULT_BAUDRATE 115200)
set(DEFAULT_LED1PIN "PC4")
set(DEFAULT_LED2PIN "PB13")
set(DEFAULT_LED3PIN "PB14")
set(DEFAULT_LED4PIN "PB15")

# Options with default values
set(BAUDRATE ${DEFAULT_BAUDRATE} CACHE STRING "Baudrate for the application")
set(LED1PIN ${DEFAULT_LED1PIN} CACHE STRING "LED 1 output pin")
set(LED2PIN ${DEFAULT_LED2PIN} CACHE STRING "LED 2 output pin")
set(LED3PIN ${DEFAULT_LED3PIN} CACHE STRING "LED 3 output pin")
set(LED4PIN ${DEFAULT_LED4PIN} CACHE STRING "LED 4 output pin")

# Print values
message("Pins must be [0,15]. Avaliable ports: A, B, C")
message(STATUS "BAUDRATE: ${BAUDRATE}")
message(STATUS "LED1PIN: ${LED1PIN}")
message(STATUS "LED2PIN: ${LED2PIN}")
message(STATUS "LED3PIN: ${LED3PIN}")
message(STATUS "LED4PIN: ${LED4PIN}")
# ------------------------------------------------- End build options

# Add all definitions
add_definitions(
    -DBAUDRATE=${BAUDRATE}
    -DLED1PIN="${LED1PIN}"
    -DLED2PIN="${LED2PIN}"
    -DLED3PIN="${LED3PIN}"
    -DLED4PIN="${LED4PIN}"
)



# The executable target
add_executable(${PROJECT_NAME} src/main.cpp
                    src/USART.cpp
                    src/Serial.cpp
                    src/CmdParser.cpp
                    src/GPIO.cpp
                    src/BuildOptionsParser.cpp
                    src/Timer.cpp
                    src/timerInterrupts.cpp
                    startup/startup.c
)

target_include_directories(${PROJECT_NAME} 
PRIVATE ${CMAKE_SOURCE_DIR}/drivers/CMSIS/include
PRIVATE ${CMAKE_SOURCE_DIR}/drivers/CMSIS/device
PRIVATE ${CMAKE_SOURCE_DIR}/include
PRIVATE ${CMAKE_SOURCE_DIR}/debug
                            )


# Post-build command to generate elf file from executable
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
COMMAND arm-none-eabi-objcopy -O elf32-littlearm ${PROJECT_NAME} "${PROJECT_NAME}.elf")

# Post-build command to generate bin file from executable
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
COMMAND arm-none-eabi-objcopy -O binary ${PROJECT_NAME} "${PROJECT_NAME}.bin")

# Post-build command to display the size of the elf file.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
COMMAND arm-none-eabi-size --format=berkeley "${PROJECT_NAME}.elf")

#OpenOCD
set(openocd_scripts_path /usr/local/share/openocd/scripts)

set(openocd_flags
    -f ${openocd_scripts_path}/interface/stlink.cfg 
    -f ${openocd_scripts_path}/target/stm32f4x.cfg
    -c "program ${PROJECT_NAME}.elf verify reset exit "
    -l openocdlog
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
COMMAND openocd ${openocd_flags} )       
