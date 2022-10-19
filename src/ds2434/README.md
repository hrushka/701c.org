# 🦋 701c.org / Battery / DS2434 Clone  

Find the full rebuild and walkthrough at [https://701c.org/projects/battery](https://701c.org/projects/battery).

> **Note**: If you'd rather purchase a pre-built version of the board with the IC already programmed, you can do so from the [701c.org Shop](https://701c.org/shop). *COMING SOON!*

#

## Background

The following is a clone of an obsolete 1-wire IC (Dallas DS2434) that any ThinkPad 701 battery needs in order to be recognized by the system and allowed to charge.

There is a lot of non-standard behavior that make this chip unique and why purchasing a DS2434 off eBay or Aliexpress won't get you very far.

It has been tested using an Arduino Uno developent board and can be compiled and run on an **ATtiny85** MCU. It uses the internal temperature sensing and recieves +3.3V power from the laptop so this is the only chip needed, making PCB design super straight forward.

This code requires a modified version of the awesome OneWireHub library by [Ingmar Inʒo Splitt (@orgua)](https://github.com/orgua) that you can find here: 

[https://github.com/hrushka/OneWireHub](https://github.com/hrushka/OneWireHub)

#

## Building the clone

1. Download [the code](https://github.com/hrushka/701c.org/tree/main/src/ds2434/ds2434.ino) and open it in the Arduino IDE.
   
2. Download the modified version of the [OneWireHub](https://github.com/hrushka/OneWireHub) as a Zip file.
   
3. In your Arduino IDE, go into *Sketch → Include Library →  Add .ZIP Library...* and select the downloaded zip file.
   
4. Open up your preferences and under "*Additional Boards Manager URLs*" add: https://raw.githubusercontent.com/damellis/attiny/ide-1.6.x-boards-manager/package_damellis_attiny_index.json

5. Close Preferences and under the *Tools* menu select the following:
   1. **Board**: ATtiny 25/45/85
   2. **Processor**: ATtiny85
   3. **Clock**: Internal 16mhz
   4. **Port**: [*your programmer*]

6. In order to configure the ATtiny and set the clock to 16mhz, select "*Burn bootloader*".

7. Once this is complete, you should be able to upload the code directly to the ATtiny without any additional setup.

> **Note**: The ATtiny version of the code measures the temperature by checking the voltage resistance and uses a baseline. Depending on your chip/package selection you may want to calibrate the values in the **chipTemp()** function. Instructions on how to do this can be found here: http://www.atmel.com/Images/doc8108.pdf

#

## The PCB


If you'd like to print your own PCB, you can [download the latest Gerber files here](https://github.com/hrushka/701c.org/tree/main/assets/battery/gerber). There is also a version that will work with the original DS2434 IC in case you happen to have one you've pulled form an original battery.

The PCB uses the **ATTINY85-20SF** surface mount package

#

## Things to know...

1. Any new battery/cells need to be calibrated. Before the ThinkPad will show an accurate charge level or function properly, it will need to go through a full charge/discharge cycle. Charge the battery fully until the green light turns off on its own, then unplug and let the battery fully discharge. Repeat the charge cycle and you should be good to go. You can press **FN+F2** at any time to see the current charge and estimated time left.

2. The battery has been tested with **Ni-Cad** and **Ni-MH** type batteries. I do not know how the chip or laptop will behave if you use another chemistry like **Lithium-ion**. All of the reference chips I used were pulled from original **Ni-Cad** batteries.