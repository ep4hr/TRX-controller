HF ham band transceiver controller  By H.Rahimi EP4HR ( ep4hr@yahoo.com)
This Project is a controller for HF ham band transceiver .Main parts of project is Arduino Mega 2560
and AD9850 DDS module .
I use Arduino Mega because it has 54 digital pin and 16 analog pin and there is not any limitation for 
Input and output pin .
Also in this project I use AD9850 DDS module for produce RF signal for   receiver and transmitter.
This circuit has this capability :
-Produce stable RF signal for receiver with plus or minus IF frequency. (I use 10 MHz IF but it can be change) and show RX  frequency .
-Produce stable RF signal for transmitter with plus or minus TX  IF frequency. . (I use 10 MHz IF but it can be change) and show TX  frequency .
-Use two VFO A or B and split mode 
-Use two antenna port A or B 
-Use AGC fast or slow
-Use modulation switch for USB , LSB and CW . It is very easy to add mode like AM or FM .
-Use up and down switch for Band .Band are from 3.5 MHz to 28 MHz .It is very easy to add other band like 1.8 MHz .
-Read receiving signal (S meter ) and show it by bar .(By use AGC signal of receiver maximum 1 volt for S equal to 59 and use a potentiometer   for more than 1 volt ).
-Read transmitter  power signal (PWR) ) and show it by bar .(some sample of TX Power must be change to DC and maximum 1 volt maximum power and use a potentiometer   for more than 1 volt ).
-Read DC power supply of transmitter and show it by number. (Maximum to 20 volt).
For change frequency use a rotary  switch and Key switch of rotary for change step frequency from 10 Hz to 1 MHz .According to frequency software active a digital pin for use RF filter bank .
It is better use buffers for each out put digital pin like 74HC240 for protection of Arduino.
Hardware requirement :
-Arduino Mega 2560
-AD9850 DDS module 
- LCD 20 x 4
-Two pieces 5K potentiometer  for 
-One piece 10K potentiometer for adjust LCD contrast 
-Eight pieces pushbutton switch
- A rotary switch 
-Resistor 2K ,330 ohm ,620 ohm , 1K ,3.3K ,2.2K ,100 ohm ,1K(two pieces of each ) 
Chang IF frequency :
It is possible to change IF frequency also change frequency plus or minus from RX or TX frequency.
In this line can change frequency IF in Hertz :
 int_fast32_t iffreq = 10000000; // Intermedite Frequency + Amount to subtract (+) from base frequency.
Here IF frequency is 10MHz 
In this line can change  IF plus or minus from RX or TX frequency:
if (GoIF == 1){frequency=frequency+iffreq;}; //If pin = low, Add the IF frequency.
If use( –) sign  IF frequency subtract from frequency and if use (+) sign IF frequency added to frequency .

If have any question or want change software according to your radio project can drop mail to
  ep4hr@yahoo.com  or contact via https://github.com/ep4hr 





