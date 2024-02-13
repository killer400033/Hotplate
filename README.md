# DIY Hotplate Controller

I Had bought an SMD hotplate and bought the cheapest one I could find, the one shown below:

<img src="https://github.com/killer400033/Hotplate/assets/37206981/797e4627-6a26-49e0-a4f4-367b410aef4f" width="400">

Being that cheap, it had a really bad controller, so I thought it would be fun to make my own PID controller. I had a spare stm32 nucleo board that I had broken a couple of GPIO pins on, meaning it would be perfect for this.

With no real pre-existing libraries, everything connected to the microcontroller I had to write "libraries" for myself. And since it was specific for this stm32g4 chip, I could best utilize its hardware for maximum speed and efficiency.

## Display

So, starting with the display, I chose the st7920 128x64 LCD, as it was cheap and I already had a couple:

![image](https://github.com/killer400033/Hotplate/assets/37206981/df340d08-c3c8-4f38-b2ec-2c335868d4cd)

I first had to go through its very sparse documentation to try to figure out how to write to it. This meant a lot of experimenting with trying to get it to work, as the documentation skips major parts on how to drive the display. Once I finally figured it out, I maximized the SPI bus speed (I drove the display using its serial interface) and reduced the delays between data packets to maximize the transfer speed. I also designed the code such that the CPU has to spend almost no time working to write to the display. Instead, the DMA was programmed to automatically read from the display buffer and write it to the SPI peripheral, and timers were setup to wait the correct amount of time between instructions to the display, hence not stalling the CPU at all. I also considered a double-buffered system, allowing the CPU to edit the display buffer while the display was being updated, but for this display this was unnecessary as it was so slow that the CPU time taken to edit the display buffer would not significantly affect the max refresh rate. 

Now with the ability to write to the display using the display buffer, I needed a way to draw onto the display buffer. Hence, I created the following functions, from the ground up, to be able to draw:
- ```drawAngledLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)```: Draw an angled line
- ```drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height)```: Draw a rectangle
- ```drawRectangleHollow(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t thickness)```: Draw a hollow rectangle
- ```drawCircle(uint16_t x, uint16_t y, uint16_t r)```: Draw a circle
- ```drawCircleHollow(uint16_t x, uint16_t y, uint16_t r, uint16_t thickness)```: Draw a hollow circle
- ```drawString(uint16_t x, uint16_t y, Font font, char *str)```: Draw a string using a given font
- ```drawBitMap(uint16_t x, uint16_t y, Bitmap bitmap)```: Draw a bitmap

I tried to make the code as fast as possible. E.g. for the sqrt function I used assembly to make sure the FPU square root instruction is used, as ```sqrt()``` from ```math.h``` might do a software square root.

All the functions used heavy bitwise operations, as I had to map randomly sized structures onto 16-bit long chunks that were written to the display.

## Logic and UI

With the display "library" written, I was now ready to build the logic and UI elements. I first built the home menu:

![image](https://github.com/killer400033/Hotplate/assets/37206981/90fa5956-3508-4703-92da-bab5e5ad67b9)

Using a couple custom bitmaps, this menu shows:
- hotplate power on bottom left
- fan power on bottom right
- current temperature on middle left
- temperature delta on middle right
- status on top left
- job time elapsed on top right (starts once heating cycle has started)

To go to another menu, the selection menu can be brought up:

![image](https://github.com/killer400033/Hotplate/assets/37206981/26daa868-ff02-41b0-a777-3cad5c9a4cda)


This menu gives 4 options to select from:
- Home: go to home menu
- Start/ Abort: start or abort a heating cycle
- Curve: edit the temperature curve
- Calib: calibrate the temperature probe

The curve menu looks like the following:

![image](https://github.com/killer400033/Hotplate/assets/37206981/940daed1-75b1-4424-b11a-d74748b98471)

As you can see, in this menu you can adjust the temperature curve that the hotplate will attempt to follow. The x-axis is the time in seconds (goes up to 360s = 6 mins). There are 13 points of adjustment at 30-second intervals, and each point can be adjusted to a temperature between 0 and 270 degrees.

The calibration menu looks like the following:

![image](https://github.com/killer400033/Hotplate/assets/37206981/2b7e9f38-257d-47a2-ade6-7e5a0ccfecb3)

In this menu, there are 3 calibration steps, one at 50 C, one at 150 C and one at 250 C. At each step, the user will wait until the hotplate reaches the "Set Temp". Then, the user will adjust the "Real Temp" to the actual measured temperature, taken by an external tool. Once all 3 steps are complete, the software will "plot" the 3 "Meas Temps" on the x-axis and the 3 "Real Temps" on the y-axis, and then get the line of best fit using the least square method. This yields a linear equation, where if the measured temperature is input, the true temperature should be output (assuming their error is linear). This equation is then just applied to the measured temperature from now on.

## Input
User input is just handled by an encoder with built in button, shown here:

![image](https://github.com/killer400033/Hotplate/assets/37206981/6fe029e5-50c9-4b4c-b8a5-95d12685d912)

## EEPROM
For the data produced by the curve menu and the calibration menu, it would be good if the data was saved to non-volatile storage such that it can be restored after power cycling. Hence, I added an external 256kB EEPROM (24LC256). It interfaced through I2C, hence I had to again create my own custom "library" to interface with it. Like before, I tried to minimize CPU overhead, meaning no stalling/ waiting for transfers. Instead, everything is interrupt-driven. Additionally, my "library" implements error checking to make sure writes and reads to the EEPROM are successful. The result is displayed to the user using popups like shown below:

![image](https://github.com/killer400033/Hotplate/assets/37206981/7eb410db-e4cd-43ca-a00a-101aaa5dd392)
![image](https://github.com/killer400033/Hotplate/assets/37206981/09c16cd8-7a73-4fc4-89be-b8a03bc342c3)

## Final Implementation

First, I built everything on a proto board to make sure everything was functioning as expected:

<img src="https://github.com/killer400033/Hotplate/assets/37206981/7b5fb4ac-87c7-4bde-9d6e-e4ef6c0af2f4" width="400">

Then, everything was attached to the inside of the hotplate, replacing the old electronics:

<img src="https://github.com/killer400033/Hotplate/assets/37206981/01a683d1-87a2-4c63-bdf1-49826a1bcdf7" width="700">

Some extra stuff like a PWM fan driver and a triac circuit was added to control the fan and heater respectively.

The top part, where the actual hotplate is was drastically modified to add a cooling fan to reduce the time taken to cool down:

<img src="https://github.com/killer400033/Hotplate/assets/37206981/3ef3b34d-f7de-43e9-b5e9-313632ab5c55" width="700">

The final result looks like this:

<img src="https://github.com/killer400033/Hotplate/assets/37206981/6c481ce7-24b6-4def-8513-efbe3f2d23ea" width="700">

Although the software works perfectly, the hardware (heating and cooling) isn't very good still so the maximum ramp rates that can be set in the curve menu have to be limited, otherwise the hotplate just can't keep up.
