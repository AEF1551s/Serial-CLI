# Description
IoT coding test  
Everything in /src and /include is written in **bare-metal**.  
Serial communication, supports set-led and echo commands  
Write command `set-led 1,500` to enable LED 1 500ms, after it turns off and receive `data led-off: 1`  
All commands end with`\r`. All outputs end with `\r\n`   
Each LED uses seperate timer to count time until it turns off.  
# BUILD options  
Select LED pins -DLEDxPIN="PxN", for example: `-DLED1PIN="PA5"`    
Avaliable ports are A, B, C from pin 0 to 15   
Select baudrate `-DBAUDRATE=115200`. Supports only standart baudrates  

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
