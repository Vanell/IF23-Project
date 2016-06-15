import numpy as np
from math import sqrt as sqr
from math import sin,cos,atan, degrees, radians,atan2
import csv
import datetime

a = 6378157.0
b = 6356752.3142

def seg2dec(beta):
	return float(beta[0]+(beta[1]/60)+(beta[2]/3600))

def dec2sec(beta):
	a,b = str(beta).split(".")
	b,c = str(float(b)*60).split(".")
	c = float(c)*60

	return (float(a),float(b),float(c))

def lat2cart(lamb,phi,h):

	N = a/(sqr(1-(e**2)*(sin(phi))**2))

	x = (N+h)*cos(phi)*cos(lamb)
	y = (N+h)*cos(phi)*sin(lamb)
	z = (N*(1-e**2)+h)*sin(phi)

	return (x,y,z,N)

def cart2lat(x,y,z):
	f = 1 - sqr(1-e**2)
	R = sqr(x**2 + y**2 + z**2)
	lamb = atan2(y,x)
	mu = atan( (z/(sqr(x**2+y**2))) *( (1 - f)+ ((e**2*a)/R)) )
	phi = atan( (z*(1-f)+e**2*a*sin(mu)**3)/( (1-f)*(sqr(x**2+y**2)-e**2*a*cos(mu)**3)) )
	h = (sqr(x**2+y**2)*cos(phi))+(z*sin(phi))-(a*sqr(1-e**2*sin(phi)**2))

	return (lamb,phi,h)

def global2locale(M,N):

	N0 = (lat2cart(N[0],N[1],N[2]))[3]

	a = np.ones ((3,3))
	b = np.ones((3,1))

	a[0,0] = -sin(N[0])
	a[1,0] = -sin(N[1])*cos(N[0])
	a[2,0] = -cos(N[1])*cos(N[0])

	a[0,1] = -cos(N[0])
	a[1,1] = -sin(N[1])*sin(N[0])
	a[2,1] = cos(N[1])*sin(N[0])

	a[0,3] = 0
	a[1,3] = cos(N[1])
	a[2,3] = sin(N[1])

	b[0,0] = M[0] - N0*cos(N[1])*cos(N[0])
	b[1,0] = M[1] - N0*cos(N[1])*cos(N[0])
	b[2,0] = M[2] - N0*(1-e**2)*sin(N[1])

	return np.dot(a,b)

def long_data(data):
	d1 = int(data[0][4])
	mo1 = int(data[0][5])
	y1 = int(data[0][6])
	h1 = int(data[0][7])
	m1 = int(data[0][8])
	s1 = int(data[0][9])

	d2 = int(data[-1][4])
	mo2 = int(data[-1][5])
	y2 = int(data[-1][6])
	h2 = int(data[-1][7])
	m2 = int(data[-1][8])
	s2 = int(data[-1][9])
	
	t1 = int(datetime.datetime.strptime('%s-%s-%s %s:%s:%s'%(y1,mo1,d1,h1,m1,s1), '%Y-%m-%d %H:%M:%S').strftime("%s"))
	t2 = int(datetime.datetime.strptime('%s-%s-%s %s:%s:%s'%(y2,mo2,d2,h2,m2,s2), '%Y-%m-%d %H:%M:%S').strftime("%s"))

	t = t2-t1
	a,b = str(float(t)/3600).split(".")
	b,c = str(float(b) / (10**len(b)) * 60).split(".")
	c = int(float(b) / (10**len(b)) * 60)
	print ("%s:%s:%s"%(a,b,c))
	return (a,b,c)

def processing_data(file_location):
	data = list()
	file = csv.reader(open(file_location,"rb"))
	
	for rows in file:
		data.append(rows)
	data = data[1:]

	long_data(data)

	# Average lat long 
	aver_lat =0
	for i in range(len(data)):
		aver_lat += float(data[i][0]) 
	aver_lat = aver_lat/len(data)
	print aver_lat

	aver_long = 0
	for i in range(len(data)):
		aver_long += float(data[i][1]) 
	aver_long = aver_long/len(data) 
	print aver_long


processing_data("data/itinary_1.csv")
