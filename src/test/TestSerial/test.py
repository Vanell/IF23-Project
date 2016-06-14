import serial
from time import time, sleep

port_connect = "/dev/ttyUSB0"
begin_time = time()
arduino = serial.Serial()
# arduino.close()
arduino.port = port_connect
arduino.baudrate = 57600
# arduino.flushInput()
# print("Tentative de connection : %s"%port_connect)	
# sleep(2)
# a = True
# while arduino.isOpen():
# 	if a:
# 		arduino.write(b'blabla')
# 		a = False

# 	if arduino.inWaiting() > 0:
# 		# print(arduino.inWaiting())
# 		b = (arduino.read()).decode("utf-8")
# 		print(b)
# 	# else :
# 	# 	break
# 	# print(type(b))
# # print("End")		
# # while arduino.inWaiting() == 0:
# # 	sleep(0.1)
# # 	if time()-begin_time > 10:
# # 		draw_shell("Echec de connection : %s"%port_connect)
# # 		break


def wr_serial(commande):
	global arduino

	# print(arduino.port)
	# print(arduino.baudrate)
	data = list()
	if not arduino.isOpen():
		arduino.open()
	else :
		print("port open")
	arduino.flushInput()
	sleep(2)

	print(commande)
	arduino.write(commande)
	print("acquire data")
	while arduino.isOpen():
		# data.append((arduino.read()).decode("utf-8"))
		if arduino.inWaiting() > 0:
		# print (arduino.inWaiting())
			data.append((arduino.read()).decode("utf-8"))
			# print(data)
		if data != list():
			if data[-1] == "\r":
				break
	data = ''.join(data[:-1])
	return data
print (wr_serial("babla"))