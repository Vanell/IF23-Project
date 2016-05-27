from time import sleep
import serial
import arduinoserial

arduino = serial.Serial('/dev/ttyUSB1', 9600)
print "arduino initialized" # Establish the connection on a specific port
while True:
	user_input = raw_input("Data to send: ")
	arduino.write(user_input) # Convert the decimal number to ASCII then send it to the Arduino
	
	while arduino.inWaiting()>0:
		print arduino.readline() 
	# Read the newest output from the Arduino
