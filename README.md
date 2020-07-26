# 24-Hour-Digital-Clock
In this project we use a ESP32 micro-controller along with a 4 Digit 7-segment display (common cathode or ground) to build a 24 Hour Digital Clock. 
First we connect all the pins from 4 Digit 7-Segment display to the ESP32 GPIOs. The Driver for the 4 Digit 7-Segment display is written for reference check the Data Sheet included and the Arduino Code. 
Since we are using a common cathode configuration we require a multiplexer to multiplex all the cathodes of the 4 digit 7-Segment dsiplay to the common ground of the micro-controller.
The multiplexer used in this project (16 channel) is a defective one which has only 4 working channels and 4 select pins. 
Note: If you are using a 4 channel multiplexer it requires only two select pins or two GPIOs and in case of a 16 channel mux it requires 4 select pins. 
The in-built Timer for ESP32 is used to track the time. Which ticks every second. 
We also make use of NTP library which needs to be installed from the Arduino library manager. NTP library allows us to get real time from the internet which can be used to set up the time for the clock.  
