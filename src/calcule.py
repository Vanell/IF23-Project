import numpy as np
from math import sqrt
from math import sin,cos,atan, degrees, radians,atan2,log,tan,pi
import csv
import datetime
from random import randint
import matplotlib.pyplot as plt

a = 6378157.0
b = 6356752.3142

e = sqrt((a**2 - b**2)/a**2)
f = 1 - sqrt(1-e**2)

def seg2dec(beta):
	return float(beta[0]+(beta[1]/60)+(beta[2]/3600))

def dec2sec(beta):
	a,b = str(beta).split(".")
	b,c = str(float(b)*60).split(".")
	c = float(c)*60

	return (float(a),float(b),float(c))

def lat2cart(longi,lati,h):
	longi = radians(longi)
	lati = radians(lati)

	N = a/(sqrt(1-(e**2)*(sin(lati))**2))

	x = (N+h)*cos(lati)*cos(longi)
	y = (N+h)*cos(lati)*sin(longi)
	z = (N*(1-e**2)+h)*sin(lati)

	return (x,y,z,N)

def cart2lat(x,y,z):
	R = sqrt(x**2 + y**2 + z**2)
	longi = atan2(y,x)
	mu = atan( (z/(sqrt(x**2+y**2))) *( (1 - f)+ ((e**2*a)/R)) )
	lati = atan( (z*(1-f)+e**2*a*sin(mu)**3)/( (1-f)*(sqrt(x**2+y**2)-e**2*a*cos(mu)**3)) )
	h = (sqrt(x**2+y**2)*cos(lati))+(z*sin(lati))-(a*sqrt(1-e**2*sin(lati)**2))

	return (longi,lati,h)

def global2locale(M,N):
	"M : point a transformer (x,y,z) "
	"N : point de reference (long,lat,h)"

	N0 = (lat2cart(N[0],N[1],N[2]))[3]

	a = np.ones ((3,3))
	b = np.ones((3,1))

	lamb0 = radians(N[0])
	phi0 = radians(N[1])

	a[0,0] = -sin(lamb0)
	a[1,0] = -sin(phi0)*cos(lamb0)
	a[2,0] = cos(phi0)*cos(lamb0)

	a[0,1] = cos(lamb0)
	a[1,1] = -sin(phi0)*sin(lamb0)
	a[2,1] = cos(phi0)*sin(lamb0)

	a[0,2] = 0
	a[1,2] = cos(phi0)
	a[2,2] = sin(phi0)

	b[0,0] = M[0] - N0*cos(phi0)*cos(lamb0)
	b[1,0] = M[1] - N0*cos(phi0)*sin(lamb0)
	b[2,0] = M[2] - N0*(1-e**2)*sin(phi0)
	return np.dot(a,b)

def longi_data(data):
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
	return (a,b,c)

def work_point(data):
	# Average lat longi 
	aver_longi = 0
	for i in range(len(data)):
		aver_longi += float(data[i][1]) 
	aver_longi = aver_longi/len(data) 
	
	aver_lat =0
	for i in range(len(data)):
		aver_lat += float(data[i][0]) 
	aver_lat = aver_lat/len(data)

	return ((aver_longi,aver_lat,0),lat2cart(aver_longi,aver_lat,0))

def processing_data(file_location):
	a = 6378157.0
	b = 6356752.3142

	e = sqrt((a**2 - b**2)/a**2)
	f = 1 - sqrt(1-e**2)

	data = list()
	file = csv.reader(open(file_location,"rb"))
	
	for rows in file:
		# for i in range(len(rows)):
			# if int(rows[10]) == 0:
			# 	rows[10] = randint(116,130)
		data.append(rows)
	# data = data[1:]

	# longi_data(data)
	print len(data)
	pt_work = work_point(data)

	data_calc = list() # (degre,global,local)

	for i in range(len(data)):	
		data_calc_pt = dict()
		data_calc_pt["long"] = float(data[i][1])
		data_calc_pt["lat"] = float(data[i][0])
		data_calc_pt["alt"] = float(data[i][11])
		pt_glob  = lat2cart( float(data[i][1]),float(data[i][0]),float(data[i][10]))
		data_calc_pt["xglob"] = pt_glob[0]
		data_calc_pt["yglob"] = pt_glob[1]
		data_calc_pt["zglob"] = pt_glob[2]
		pt_loc = global2locale(lat2cart( float(data[i][1]),float(data[i][0]),float(data[i][10]) ),pt_work[0])
		data_calc_pt["xloc"] = pt_loc[0][0]
		data_calc_pt["yloc"] = pt_loc[1][0]
		data_calc_pt["zloc"] = pt_loc[2][0]
		data_calc_pt["age"] = float(data[0][4])
		d =  int(float(data[i][5]))
		mo = int(float(data[i][6]))
		y =  int(float(data[i][7]))
		h =  int(float(data[i][8]))
		m =  int(float(data[i][9]))
		s =  int(float(data[i][10]))
		data_calc_pt["time"] = int(datetime.datetime.strptime('%s-%s-%s %s:%s:%s'%(y,mo,d,h,m,s), '%Y-%m-%d %H:%M:%S').strftime("%s"))
		data_calc_pt["sat"] = float(data[i][2])
		data_calc_pt["hdop"] = float(data[i][3])
		data_calc_pt["lambert93_lat"] =degrees( log(tan(pi/4+radians(float(data[i][0]))/2))*(((1-e*sin(radians(float(data[i][0]))))/(1+e*sin(radians(float(data[i][0])))))**(e/2)))
		pt_glob  = lat2cart( float(data[i][1]),float(data_calc_pt["lambert93_lat"]),float(data[i][10]))
		data_calc_pt["lambert93_xglob"] = pt_glob[0]
		data_calc_pt["lambert93_yglob"] = pt_glob[1]
		data_calc_pt["lambert93_zglob"] = pt_glob[2]
		pt_loc = global2locale(lat2cart( float(data[i][1]),float(data_calc_pt["lambert93_lat"]),float(data[i][10]) ),pt_work[0])
		data_calc_pt["lambert93_xloc"] = pt_loc[0][0]
		data_calc_pt["lambert93_yloc"] = pt_loc[1][0]
		data_calc_pt["lambert93_zloc"] = pt_loc[2][0]
		data_calc.append(data_calc_pt)

	# print len(data), len(data_calc)

	average = dict()
	average["x"] = 0
	average["y"] = 0
	average["z"] = 0
	average["hdop"] = 0
	average["sat"] = 0
	average["lambert93_x"] = 0
	average["lambert93_y"] = 0
	average["lambert93_z"] = 0
	average["age"] = 0

	#Average 
	for i in range(len(data_calc)):
		average["x"] += data_calc[i]["xloc"] 
		average["y"] += data_calc[i]["yloc"] 
		average["z"] += data_calc[i]["zloc"] 
		average["hdop"] += data_calc[i]["hdop"] 
		average["sat"] += data_calc[i]["sat"] 
		average["lambert93_x"] += data_calc_pt["lambert93_xloc"]
		average["lambert93_y"] += data_calc_pt["lambert93_yloc"]
		average["lambert93_z"] += data_calc_pt["lambert93_zloc"]
		average["age"] += data_calc_pt["age"]

	average["x"] = average["x"]/len(data_calc)
	average["y"] = average["y"]/len(data_calc)
	average["z"] = average["z"]/len(data_calc)
	average["hdop"] = average["hdop"]/len(data_calc)
	average["sat"] = average["sat"]/len(data_calc)
	average["lambert93_x"] = average["lambert93_x"] /len(data_calc) 
	average["lambert93_y"] = average["lambert93_y"] /len(data_calc)
	average["lambert93_z"] = average["lambert93_z"] /len(data_calc)
	average["age"] = average["age"] /len(data_calc)

	# print average
	
	#Variance

	variance = dict()
	variance["x"] = 0
	variance["y"] = 0
	variance["z"] = 0
	variance["lambert93_x"] = 0
	variance["lambert93_y"] = 0
	variance["lambert93_z"] = 0
	variance["age"] = 0

	for i in range(len(data_calc)):
		variance["x"] += (data_calc[i]["xloc"] - average["x"] )**2
		variance["y"] += (data_calc[i]["yloc"] - average["y"] )**2
		variance["z"] += (data_calc[i]["zloc"] - average["z"] )**2
		variance["lambert93_x"] += (data_calc_pt["lambert93_xloc"] - average["lambert93_x"])**2
		variance["lambert93_y"] += (data_calc_pt["lambert93_yloc"] - average["lambert93_y"])**2
		variance["lambert93_z"] += (data_calc_pt["lambert93_zloc"] - average["lambert93_z"])**2
		variance["age"] += (data_calc_pt["age"] - average["age"])**2
	variance["x"] = variance["x"]/len(data_calc)
	variance["y"] = variance["y"]/len(data_calc)
	variance["z"] = variance["z"]/len(data_calc)
	variance["lambert93_x"] = variance["lambert93_x"]/len(data_calc)
	variance["lambert93_y"] = variance["lambert93_y"]/len(data_calc)
	variance["lambert93_z"] = variance["lambert93_z"]/len(data_calc)
	variance["age"] = variance["age"]/len(data_calc)

	# print variance

	#Ecart type 
	ecart = dict()
	ecart["x"] = sqrt(variance["x"])
	ecart["y"] = sqrt(variance["y"])
	ecart["z"] = sqrt(variance["z"])
	ecart["lambert93_x"] = sqrt(variance["lambert93_x"])
	ecart["lambert93_y"] = sqrt(variance["lambert93_y"])
	ecart["lambert93_z"] = sqrt(variance["lambert93_z"])
	ecart["age"] = sqrt(variance["age"])

	# print ecart

	#Cov
	cov = dict()
	cov["xy"] = [0,0,0]
	cov["xz"] = [0,0,0]
	cov["yz"] = [0,0,0]

	for i in range(len(data_calc)):
		cov["xy"][0] += data_calc[i]["xloc"]*data_calc[i]["yloc"]
		cov["xz"][0] += data_calc[i]["xloc"]*data_calc[i]["zloc"]
		cov["yz"][0] += data_calc[i]["yloc"]*data_calc[i]["zloc"]
		
		cov["xy"][1] += data_calc[i]["xloc"]
		cov["xz"][1] += data_calc[i]["xloc"]
		cov["yz"][1] += data_calc[i]["yloc"]

		cov["xy"][2] += data_calc[i]["yloc"]
		cov["xz"][2] += data_calc[i]["zloc"]
		cov["yz"][2] += data_calc[i]["zloc"]

	cov["xy"] = float(1/len(data_calc))*cov["xy"][0] - float(1/len(data_calc))*(cov["xy"][1]*cov["xy"][2]) 
	cov["xz"] = float(1/len(data_calc))*cov["xz"][0] - float(1/len(data_calc))*(cov["xz"][1]*cov["xz"][2])
	cov["yz"] = float(1/len(data_calc))*cov["yz"][0] - float(1/len(data_calc))*(cov["yz"][1]*cov["yz"][2])

	# print cov

	#DOP
	dop = dict()
	dop["HPOD"] = sqrt(variance["y"]**2+variance["x"]**2)
	dop["VPOD"] = variance["z"]
	dop["PDOP"] = sqrt(variance["y"]**2+variance["x"]**2+variance["z"]**2)
 	
	# print dop

	sat = [""]*11
	for i in range(len(sat)):
		sat[i] = [list(),dict()]

	for i in range(len(data_calc)):
		if data_calc[i]["sat"] == 0:
			sat[0][0].append(i)
		elif data_calc[i]["sat"] == 1:
			sat[1][0].append(i)
		elif data_calc[i]["sat"] == 2:
			sat[2][0].append(i)
		elif data_calc[i]["sat"] == 3:
			sat[3][0].append(i)
		elif data_calc[i]["sat"] == 4:
			sat[4][0].append(i)
		elif data_calc[i]["sat"] == 5:
			sat[5][0].append(i)
		elif data_calc[i]["sat"] == 6:
			sat[6][0].append(i)
		elif data_calc[i]["sat"] == 7:
			sat[7][0].append(i)
		elif data_calc[i]["sat"] == 8:
			sat[8][0].append(i)
		elif data_calc[i]["sat"] == 9:
			sat[9][0].append(i)
		elif data_calc[i]["sat"] == 10:
			sat[10][0].append(i)

	for i in range(len(sat)):
		sat[i][1]["average"] =dict()
		sat[i][1]["variance"] =dict()
		sat[i][1]["ecart"] =dict()

		if sat[i][0] != list():
			sat[i][1]["average"]["x"] = 0
			sat[i][1]["average"]["y"] = 0
			sat[i][1]["average"]["z"] = 0
			sat[i][1]["average"]["hdop"] = 0
			sat[i][1]["variance"]["x"] = 0
			sat[i][1]["variance"]["y"] = 0
			sat[i][1]["variance"]["z"] = 0
			sat[i][1]["variance"]["hdop"] = 0
			sat[i][1]["ecart"]["x"] = 0
			sat[i][1]["ecart"]["y"] = 0
			sat[i][1]["ecart"]["z"] = 0
			sat[i][1]["ecart"]["hdop"] = 0

			for y in range(len(sat[i][0])):
				sat[i][1]["average"]["x"] += data_calc[sat[i][0][y]]["xloc"]
				sat[i][1]["average"]["y"] += data_calc[sat[i][0][y]]["yloc"]
				sat[i][1]["average"]["z"] += data_calc[sat[i][0][y]]["zloc"]
				sat[i][1]["average"]["hdop"] += data_calc[sat[i][0][y]]["hdop"]
			sat[i][1]["average"]["x"] = sat[i][1]["average"]["x"]/len(sat[i][0])
			sat[i][1]["average"]["y"] = sat[i][1]["average"]["y"]/len(sat[i][0])
			sat[i][1]["average"]["z"] = sat[i][1]["average"]["z"]/len(sat[i][0])
			sat[i][1]["average"]["hdop"] = sat[i][1]["average"]["hdop"]/len(sat[i][0])

			for y in range(len(sat[i][0])):
				sat[i][1]["variance"]["x"] += (data_calc[sat[i][0][y]]["xloc"] - sat[i][1]["average"]["x"])**2
				sat[i][1]["variance"]["y"] += (data_calc[sat[i][0][y]]["yloc"] - sat[i][1]["average"]["y"])**2
				sat[i][1]["variance"]["z"] += (data_calc[sat[i][0][y]]["zloc"] - sat[i][1]["average"]["z"])**2
				sat[i][1]["variance"]["hdop"] += (data_calc[sat[i][0][y]]["hdop"] - sat[i][1]["average"]["z"])**2

			sat[i][1]["variance"]["x"] = sat[i][1]["variance"]["x"]/len(sat[i][0])
			sat[i][1]["variance"]["y"] = sat[i][1]["variance"]["y"]/len(sat[i][0])
			sat[i][1]["variance"]["z"] = sat[i][1]["variance"]["z"]/len(sat[i][0])
			sat[i][1]["variance"]["hdop"] = sat[i][1]["variance"]["hdop"]/len(sat[i][0])

			sat[i][1]["ecart"]["x"] = sqrt(sat[i][1]["variance"]["x"])
			sat[i][1]["ecart"]["y"] = sqrt(sat[i][1]["variance"]["y"])
			sat[i][1]["ecart"]["z"] = sqrt(sat[i][1]["variance"]["z"])
			sat[i][1]["ecart"]["hdop"] = sqrt(sat[i][1]["variance"]["hdop"])

	return data_calc, average,variance,ecart,sat,dop,cov

def graph(variance, ecart,sat):
	#Graphique
	#Average

	N = 3
	var = (variance["x"], variance["y"], variance["z"])
	ect = (ecart["x"], ecart["y"], ecart["z"])
	ind = np.arange(N)  # the x locations for the groups
	width = 0.35       # the width of the bars

	fig, ax = plt.subplots()
	rects1 = ax.bar(ind, var, width, color='r')
	rects2 = ax.bar(ind + width, ect, width, color='y')
	# add some text for labels, title and axes ticks
	ax.set_ylabel('Variance et Ecart Type (m)')
	ax.set_title('Variance et Ecart Type : x, y, z local')
	ax.set_xticks(ind + width)
	ax.set_xticklabels(('x', 'y', 'z'))

	ax.legend((rects1[0], rects2[0]), ('Variance', 'Ecart'))
	plt.savefig('%svar_ecart.png'%locations_data)
	# plt.show()

	N = 3
	var = (variance["lambert93_x"], variance["lambert93_y"], variance["lambert93_z"])
	ect = (ecart["lambert93_x"], ecart["lambert93_y"], ecart["lambert93_z"])
	ind = np.arange(N)  # the x locations for the groups
	width = 0.35       # the width of the bars

	fig, ax = plt.subplots()
	rects1 = ax.bar(ind, var, width, color='r')
	rects2 = ax.bar(ind + width, ect, width, color='y')
	# add some text for labels, title and axes ticks
	ax.set_ylabel('Variance et Ecart Type (m)')
	ax.set_title('Variance et Ecart Type : x, y, z local/ Lambert 93')
	ax.set_xticks(ind + width)
	ax.set_xticklabels(('x', 'y', 'z'))

	ax.legend((rects1[0], rects2[0]), ('Variance', 'Ecart'))
	plt.savefig('%svar_ecart_lambert.png'%locations_data)
	# plt.show()

	N = 11
	var_sat_x = list()
	for i in range(len(sat)):
		if sat[i][0] != list(): 
			var_sat_x.append(sat[i][1]["variance"]["x"])
		else:
			var_sat_x.append(0)
	ect_sat_x = list()
	for i in range(len(sat)):
		if sat[i][0] != list():
			ect_sat_x.append(sat[i][1]["ecart"]["x"])
		else:
			ect_sat_x.append(0)

	ind = np.arange(N)  # the x locations for the groups
	width = 0.35       # the width of the bars

	fig, ax = plt.subplots()
	rects1 = ax.bar(ind, var_sat_x, width, color='r')
	rects2 = ax.bar(ind + width, ect_sat_x, width, color='y')
	# add some text for labels, title and axes ticks
	ax.set_ylabel('Variance et Ecart Type (m)')
	ax.set_title('Variance et Ecart Type : x local/sattelite')
	ax.set_xticks(ind + width)
	ax.set_xticklabels(('0','1','2','3', '4', '5', '6', '7', '8', '9', '10'))

	ax.legend((rects1[0], rects2[0]), ('Variance x', 'Ecart x'))
	plt.savefig('%svar_ecart_sat_x.png'%locations_data)
	# plt.show()

	N = 11
	var_sat_y = list()
	for i in range(len(sat)):
		if sat[i][0] != list(): 
			var_sat_y.append(sat[i][1]["variance"]["y"])
		else:
			var_sat_y.append(0)
	ect_sat_y = list()
	for i in range(len(sat)):
		if sat[i][0] != list():
			ect_sat_y.append(sat[i][1]["ecart"]["y"])
		else:
			ect_sat_y.append(0)

	ind = np.arange(N)  # the x locations for the groups
	width = 0.35       # the width of the bars

	fig, ax = plt.subplots()
	rects1 = ax.bar(ind, var_sat_y, width, color='r')
	rects2 = ax.bar(ind + width, ect_sat_y, width, color='y')
	# add some text for labels, title and axes ticks
	ax.set_ylabel('Variance et Ecart Type (m)')
	ax.set_title('Variance et Ecart Type : y local/sattelite')
	ax.set_xticks(ind + width)
	ax.set_xticklabels(('0','1','2','3', '4', '5', '6', '7', '8', '9', '10'))

	ax.legend((rects1[0], rects2[0]), ('Variance y', 'Ecart y'))
	plt.savefig('%svar_ecart_sat_y.png'%locations_data)
	# plt.show()

	N = 11
	var_sat_z = list()
	for i in range(len(sat)):
		if sat[i][0] != list(): 
			var_sat_z.append(sat[i][1]["variance"]["z"])
		else:
			var_sat_z.append(0)
	ect_sat_z = list()
	for i in range(len(sat)):
		if sat[i][0] != list():
			ect_sat_z.append(sat[i][1]["ecart"]["z"])
		else:
			ect_sat_z.append(0)

	ind = np.arange(N)  # the x locations for the groups
	width = 0.35       # the width of the bars

	fig, ax = plt.subplots()
	rects1 = ax.bar(ind, var_sat_z, width, color='r')
	rects2 = ax.bar(ind + width, ect_sat_z, width, color='y')
	# add some text for labels, title and axes ticks
	ax.set_ylabel('Variance et Ecart Type (m)')
	ax.set_title('Variance et Ecart Type : z local/sattelite')
	ax.set_xticks(ind + width)
	ax.set_xticklabels(('0','1','2','3', '4', '5', '6', '7', '8', '9', '10'))

	ax.legend((rects1[0], rects2[0]), ('Variance z', 'Ecart z'))
	plt.savefig('%svar_ecart_sat_z.png'%locations_data)
	# plt.show()

def export_data(filename,data):

	keys = data[0].keys()
	with open('%s.csv'%filename, 'wb') as output_file:
	    dict_writer = csv.DictWriter(output_file, keys)
	    dict_writer.writeheader()
	    dict_writer.writerows(data)
def export_over(filename,data):
	writer = csv.writer(open('%s.csv'%filename, 'wb'))
	for key, value in data.items():
		writer.writerow([key, value])


locations_data = "data/itinary_4/"
source_file_name = "data/itinary_4.csv"

data_calc,average,variance,ecart,sat,dop,cov = processing_data(source_file_name)
graph(variance, ecart,sat)
export_data("data/itinary_4/data_calc",data_calc)
export_over("data/itinary_4/average",average)
export_over("data/itinary_4/variance",variance)
export_over("data/itinary_4/cov",cov)
export_over("data/itinary_4/dop",dop)
export_over("data/itinary_4/ecart",ecart)