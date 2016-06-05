import time
from time import sleep
import serial
import arduinoserial
import os
import gpxpy
import gpxpy.gpx

def initGPX():
	gpx = gpxpy.gpx.GPX()
	gpx_track = gpxpy.gpx.GPXTrack()
	gpx.tracks.append(gpx_track)
	gpx_segment = gpxpy.gpx.GPXTrackSegment()
	gpx_track.segments.append(gpx_segment)
	return gpx

def createGPXFile(name,gpx):
	file = open(name+'.gpx','w')
	file.write(gpx.to_xml())
	file.close()
	return

def writeWaypoint(gpx,name,lat,lon):
	gpx_waypoint = gpxpy.gpx.GPXWaypoint()
	gpx.waypoints.append(gpx_waypoint)
	gpx_waypoint.name = name
	gpx_waypoint.latitude=lat
	gpx_waypoint.longitude =lon
	return

def writeTrackpoint(gpx_seg,lat,lon,ele,time):
	gpx_seg.points.append(gpxpy.gpx.GPXTrackPoint(lat, lon, elevation=ele,time=time))
	return


fileName ='Data00'

mountpoint = os.popen('ls /dev/ttyU*').read()
if mountpoint=='':
	mountpoint = str(os.system('ls /dev/ttyA*'))
mountpoint =  mountpoint[:-1]
print "Serial connexion to " + mountpoint
print "initialization..." # Establish the connection on a specific port
cont = False
arduino = serial.Serial(mountpoint, 57600,timeout =1)
i=0
while arduino.inWaiting()==0:
	sleep(0.1)
	if i > 10:
		break
	i+=1
print arduino.read(arduino.inWaiting())
arduino.write('send')
fileText =''

while not(cont):
	
	
	while arduino.inWaiting()>0:
		nbBytes = arduino.inWaiting()
		message = arduino.read(nbBytes)
		data = message.split()
		if len(data)>1 and data[-1] == 'done':
			cont = True
			message = message[:-6]
		print message
		print 'end of message'
		fileText = fileText + message
		

	sleep(0.01)
file = open(fileName,'w')
file.write(fileText)
file.close
print 'here\'s da file nigga'
file = open(fileName,'r')
fileContent =''
for line in file:
	fileContent=fileContent + line	
file.close()
print fileContent
	