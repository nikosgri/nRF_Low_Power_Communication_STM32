# nRF_Low_Power_Communication_STM32
STM32 bare-Metal communication with nRF nRF24L01 2.4GHz Wireless Transceiver Module.

## Schematic 
The schematic of the project shows the way of how the peripherals are connected together: 
![Markdown symbol](https://github.com/nikosgri/nRF_Low_Power_Communication_STM32/blob/master/schematic_ptixiaki.drawio.png)  

### Peripherals 
The project contains two nRF24L01 2.4GHz Wireless Transceivers, one LCD 1602A, a keypad and a microphone. They are connected to the microcontroller STM32L053R8, in order to work together.

### Operations
Each microcontroller contains 4 peripherals (nRF24L01, LCD, microphone, keypad). The main idea is that we want to communicate through the nRF that it is sending messages with the  Enhanced Shockburst protocol. When the user,
presses a button of the keypad a message is transmitted directly to the other microcontroller. The way that the nRF is receiving messages and communicating with the microcontroller is by interrupts. Furthermore, when the microphone detects a sound, immediately sends a messages through the nRF to the other microcontroller that a sound is detected. The user can create a low power operation, by pressing the main button of the microcontroller. The microcontroller enters sleep mode, the clock frequency dicreasing to 2Mhz and all the peripheral clocks are closed accept from the nRF, because this is the one that wakes up the microcontroller if a messages is detected.

#### Protocols
At this project I have created two basic protocols for communication. The first one is the SPI (Serial Peripheral Interface), which is for communicating with the nRF and the second one is I2C Inter-Integrated Circuit, which is the main protocol where we can communicate with the LCD. At the same time at my project you can see my approach on how I handle Switch debouncing at the keyboard. 
