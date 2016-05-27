from time import sleep
import serial
import arduinoserial

arduino = arduinoserial.SerialPort('/dev/ttyUSB0', 9600) # Establish the connection on a specific port
while True:
     arduino.write("senddata") # Convert the decimal number to ASCII then send it to the Arduino
     
     print arduino.read_until("\n\n") # Read the newest output from the Arduino
     sleep(.1) # Delay for one tenth of a second
     