cmake_minimum_required(VERSION 3.20)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/toolchain.cmake)

project(MikroTik_IoT_test)

# The executable target
add_executable(${PROJECT_NAME} src/main.cpp
                    src/UART2.cpp
                    src/Serial.cpp
                    src/CmdParser.cpp
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
