#433MhzBananaKaKu

433MhzBananaKaKu is an ported library to send and receive klikaanklikuit commands with the Banana Pi.
It will probably work the Rasberry Pi, but that's not tested yet.

Its an ported version of:
NewRemoteSwitch library v1.1.0 (BETA) for Arduino 1.0
Made by Randy Simons http://randysimons.nl/
Depends on the WiringPi GPIO library http://wiringpi.com/ .

This library is already installed on raspbian http://www.lemaker.org/resources/9-81/raspbian_for_bananapi.html

##To build, git clone this project:
git clone https://github.com/torretje/433MhzBananaKaKu.git
<pre><code>
cd 433MhzBananaKaKu
./build
</code></pre>

The assumption is that the 433Mhz Transmiter is on WiringPi pin 0 and that the Receiver is on pin 1
##To run from the command-line:
###To receive commands
<pre><code>
sudo ./BananaKaKuReceive
</code></pre>

This program waits till a command is received and then printed to the console:
Example->Received something 11876070 unit 1 period 255

###To send a command:
<pre><code>
sudo ./BananaKaKuTransmit 1187607 10 on
sudo ./BananaKaKuTransmit 1187607 10 off
</code></pre>
More features are supported in the newKaKuTransmitter.cpp and newKaKuReceiver.cpp.
If you want to use them you have to include them in your cpp code.(for now)
