# Description
IoT coding test  
Everything in /src and /include is written in **bare-metal**.  
Serial communication, supports set-led and echo commands   
Supports 4 LEDs  
Each LED uses seperate timer to count time until it turns off  
## Commands  
All commands end with`\r`. All outputs end with `\r\n`  
### set-led  
Format: `set-led <led_id>,<time>\r`, where  
`<led_id>`: ASCII string, LED number.   
`<time>`: ASCII string, enabled state duration [1,5000). Units: milliseconds  
Avaliable LED IDs are [1,4]  
**Example**: Send command `set-led 1,500\r` to enable LED 1 for 500ms, after it turns off and outputs `data led-off: 1`  

### echo  
Format: `echo <len>,<data>\r`, where  
`<len>`: ASCII string, length of the following binary data block `<data>`, range [0,300]   
`<data>`: Binary data block   
**Example**: Send command `echo 10,<data>`, to echo `<data>`, which are 10 octets  

# BUILD options  
Select LED pins -DLEDxPIN="PxN", for example: `-DLED1PIN="PA5"`    
Avaliable ports are A, B, C from pin 0 to 15   
Select baudrate `-DBAUDRATE=115200`. Supports only standart baudrates. Default baudrate = 115200bs  
# Working examples 
set-led and echo valid and invalid commands 
At the moment no bugs are found in command parsing   
<details>
  <summary>Expand</summary>  
<div style="display: flex; justify-content: center; align-items: flex-start; flex-wrap: wrap;">
  <img src="https://github.com/user-attachments/assets/43e50ca5-75eb-4c76-bb24-e305c988c930" alt="test_set_led">
  <img src="https://github.com/user-attachments/assets/8e87bb25-d27b-45f3-b9de-513de983afe2" alt="test_echo">
</div>  
</details> 

# BUGS  
**FIXED** ~~When first command is procceseed, led turns off imediately - TIM IRQ is called as soon as LED is turned on and TIM counter enable.~~


# Prerequisites

- CMake

-  arm-none-eabi toolchain

-  OpenOCD

  

### Installing prerequisites

#### Ubuntu/Linux

##### CMake

Install build tools and libraries that CMake depends on:  
`$ sudo apt-get install build-essential libssl-dev`
Go to the temp directory:  
`$ cd /tmp`
Then, enter the following command to download the source code:  
`$ wget https://github.com/Kitware/CMake/releases/download/v3.20.0/cmake-3.20.0.tar.gz`
Once the tar.gz file is downloaded, enter the following command to extract it:  
`$ tar -zxvf cmake-3.20.0.tar.gz`
Then move to the extracted folder as follows:  
`$ cd cmake-3.20.0`
Finally, run the following commands to compile and install CMake:  
`./bootstrap`
Then  build:  
`$ make`  
And install:  
`$ sudo make install`
##### arm-none-eabi toolchain
To install run:
`$ sudo apt-get install gcc-arm-none-eabi`

##### OpenOCD
Clone OpenOCD repository:
`$ sudo git clone https://github.com/openocd-org/openocd`
Run bootsrap:  
`$ ./bootstrap`
Run configure:  
`$ ./configure --prefix=/usr/local/ --enable-ftdi --enable-stlink --enable-jlink`
Build:  
`$ make`
Install:
`$ sudo make install`

- OpenOCD requires additional dependecies:

`build-essentials`
`libtool`
`libusb-1.0-0`
`libusb-1.0-0-dev`
`gdb-multiarch`
`libjaylink-dev`

To install run:
`$ sudo apt-get install [dependecies]`

#### Other OS

See official installation and user guides

  

## Build
Go to project directory and create build folder:
`$ mkdir build`
Generate Makefiles, example is given with *Unix Makefiles*:
`$ cmake -G "Unix Makefiles" -B ./build`  
Run make in /build to compile:
`$ make`
  

## Flashing
Flashing is done using OpenOCD and flashes every compilation time.  
The openocd command is excecuted as post-build command.  
To change the interface and target configuration, see CMakeLists.txt `openocd_flags`.  

Run make in /build to compile and flash:
`$ make`

## Debugging
In this project debugging is done using OpenOCD and with VSCode extensions, for easier visual debugging: 
`Cortex-Debug`

To see register contents, access serial output and read memory use extensions such as: 
`MemoryView`
`Peripheral Viewer`
`Serial Monitor`
  
  For this to work inside VSCode the *launch* **OpenOCD configuration** and *settings* **cortex-debug** paths must be set.
For additional debug features, setup VSCode as per **extension setup guides**.

## Possible problems and improvements  
* Each LED uses dedicated timer peripheral. This is very wasteful, because each timer has capture/compare, PWM and other functions that could be used. Depending on MCU, there might not be as much timers, or all of them are already used.  
**Solution**: Use single timer, add logic to keep track of how many LEDs should be on, and how much time is left for each LED.  
**Example:** If 2 LEDs are turned on, with different times, sort LED times, calculate difference between these times, enable timer with lowest LED on time, when TIM IRQ arrives, disable first LED, reload ARR with left time.  
**Problems**: Keeping track of active LEDs and dT takes time, based on how many LEDs, could be on at the same time. This adds time that should be taken into account, when updating LED dTs and setting ARR, to ensure more precise counting.
