from time import sleep
import serial
import arduinoserial
import os

arduino = serial.Serial('/dev/ttyUSB0', 115200,timeout =1)
print "initialization..." # Establish the connection on a specific port
while arduino.inWaiting()==0:
	sleep(0.1)
sleep(1.1)
arduino.write('remove')
sleep(1.1)
arduino.write('this is a test')
sleep(1.1)
arduino.write('a pretty text')
sleep(1.1)
arduino.write('and another one')
sleep(1.1)

os.remove('Data00')