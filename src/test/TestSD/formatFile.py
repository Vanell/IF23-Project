from time import sleep
import serial
import arduinoserial
import os

mountpoint = os.popen('ls /dev/ttyU*').read()
if mountpoint=='':
	mountpoint = str(os.system('ls /dev/ttyA*'))
mountpoint =  mountpoint[:-1]
print "Serial connexion to " + mountpoint
print "initialization..." # Establish the connection on a specific port
arduino = serial.Serial(mountpoint, 57600,timeout =1)
print "initialization..." # Establish the connection on a specific port
i=0
while arduino.inWaiting()==0:
	sleep(0.1)
	if i > 10:
		break
	i+=1
print arduino.read(arduino.inWaiting())
sleep(0.5)
arduino.write('remove')
sleep(1.1)
arduino.write('this is a test')
sleep(1.1)
arduino.write('a pretty text')
sleep(1.1)
arduino.write('and another one')
sleep(1.1)

os.remove('Data00')