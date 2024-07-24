# Description
MikroTik IoT coding test  
# BUILD options  
To select LED pins and UART2 baudrate set CMake build flag -DLED1PIN="PA5"  
Avaliable ports are A, B, C from pin 0 to 15  
Supports standart baudrates  
# Testing cases  
## set-led  
manually tested  
## echo  
manually tested  

# BUGS  
When first command is procceseed, led turns off imediately - TIM IRQ is called as soon as LED is turned on and TIM counter enable.
