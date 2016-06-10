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
	gpx_waypoint.name = name
	gpx_waypoint.latitude=lat
	gpx_waypoint.longitude =lon
	gpx.waypoints.append(gpx_waypoint)
	
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
i = 0
while not(cont):
	if i > 100:
		i=0
		arduino.write('send')
	
	while arduino.inWaiting()>0:
		nbBytes = arduino.inWaiting()
		message = arduino.read(nbBytes)
		data = message.split()
		#print data
		#for c in message:
		#	if c == '\n':
		#		print "saut de ligne"
		if len(data)>1 and data[-1] == 'done':
			cont = True
			message = message[:-6]
		elif data=='done':
			cont = True
		print message
		print 'end of message'
		fileText = fileText + message
	i+=1	

	sleep(0.01)
file = open(fileName,'w')
file.write(fileText)
file.close
print 'here\'s da file nigga'
file = open(fileName,'r')
fileContent =''
for line in file:
	fileContent=fileContent + line
print fileContent
GPXfile = initGPX()
print "----------------SPLITED DATA--------------"
content = fileContent.split('\n')
for line in content:
	wpdata = line.split()
	size = len(wpdata)-2
	i=0
	wpname=''
	while 1:
		wpname+=wpdata[i]
		if i==size:
			break
		i+=1
	wpname = wpdata[0]
	wplat = wpdata[-2]
	wplon = wpdata[-1]
	print wpdata
	writeWaypoint(GPXfile,wpname,wplat,wplon)
	file.close()
#writeWaypoint(GPXfile,"ma bite",43.876845,4.641321)
createGPXFile(fileName,GPXfile)