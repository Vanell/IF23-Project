#Home made
from calcule import *

# Standard librairies
import curses
from time import sleep ,time
from os import popen, path	,makedirs, listdir
import datetime
import serial
import csv
import gpxpy
import gpxpy.gpx

#Arduino
arduino = serial.Serial()
arduino.baudrate = 19200
port_connect = "nothing"
connection_arduino = False

#Calcule
files = list()
files_gpx = list()
file_location = "data/"

#Interface variable
option_menu = 0
interface_shell = list()
interface_shell.append((time(),"Welcome on Elementaire GPS"))

#Curse init
screen = curses.initscr() # init curses 
screen.keypad(1)
dims = screen.getmaxyx()

##Define left interface txt

txt_titre = [0,"Elementaire GPS v1.0",curses.A_NORMAL]
txt_titre_menu = [0,"Menu",curses.A_UNDERLINE]

txt_menu =	[[2,"Connect",curses.A_NORMAL],
			[2,"Download File",curses.A_NORMAL],
			[2,"Delete Files",curses.A_NORMAL],
			[2,"Calcul",curses.A_NORMAL],
			[2,"Export to GPX",curses.A_NORMAL],
			[2,"Exit",curses.A_NORMAL]]

txt_files = [0,"Files",curses.A_UNDERLINE]
txt_files_gpx = [0,"GPX Files",curses.A_UNDERLINE]

txt_connect = [[0,"Connection on :",curses.A_UNDERLINE],
				[2,"",curses.A_NORMAL],]

##Define dimmension interface
maxY = dims[0]
maxX = dims[1]

width_menu_min = 23
width_gpx_min = 30

height_menu_min = len(txt_menu)+1
height_shell_min = 10


def begin_screen():
	
	screen.clear()
	curses.noecho()
	curses.curs_set(0)

	screen.addstr(int(maxY/2)-4,int(maxX/2-len("Welcome on Elementaire GPS")/2),"Welcome on Elementaire GPS",curses.A_BOLD)
	screen.addstr(int(maxY/2),int(maxX/2-len("Etre ingenieur c\'est l\'art de l\'approximation")/2),"Etre ingenieur c\'est l\'art de l\'approximation")
	screen.addstr(int(maxY/2)+1,int(maxX/2-len("I. NIKIFOROV")/2),"I. NIKIFOROV")
	
	screen.refresh()
	sleep(0.5)

def interface():
	
	global option_menu

	screen.clear()
	curses.noecho()
	curses.curs_set(0)

	draw_limit_gui()
	draw_connect()
	draw_files()
	draw_shell()
	draw_files_gpx()

	#Draw txt name prog & version
	screen.addstr(0,txt_titre[0],txt_titre[1],txt_titre[2])

	#Draw txt menu
	screen.addstr(2,txt_titre_menu[0],txt_titre_menu[1],txt_titre_menu[2])

	selection_menu = -1
	while selection_menu < 0:
		screen.nodelay(1)

		#Menu
		graphics = [0]*len(txt_menu)
		graphics[option_menu] = curses.A_REVERSE

		for z in range(len(txt_menu)):
			screen.addstr(int(z)+3,txt_menu[z][0],txt_menu[z][1],graphics[z])
		screen.refresh()
		action = screen.getch()

		if action == curses.KEY_UP:
			option_menu = (option_menu - 1)%len(txt_menu)
		elif action == curses.KEY_DOWN:
			option_menu = (option_menu + 1) %len(txt_menu)
		elif action == ord('\n'):
			selection_menu = option_menu

	screen.refresh()

	# Selection Menu
	if selection_menu == 0 : # Connection
		list_tty()
		interface()
	elif selection_menu == 1 : #Download file
		if connection_arduino :
			txt_file_dw = ["itinary","waypoint"]
			select = pad_generator("Type of file",txt_file_dw,True)
			if not "BACK" in select:
				serial_Arduino("sen:%s.txt"%select)
				
		else :
			draw_shell("Not connect")
		interface()
	elif selection_menu == 2: #Delete file
		if connection_arduino :
			txt_file_dw = ["itinary","waypoint"]
			select = pad_generator("Type of file",txt_file_dw,True)
			if not "BACK" in select:
				serial_Arduino("rem:%s.txt"%select)
				
		else :
			draw_shell("Not connect")
		interface()
	elif selection_menu == 3: #Calcule
		draw_shell("Select a file")
		calcule()
		interface()		
	elif selection_menu == 4: #export to gpx
		# history()
		# draw_shell("Sorry not implement yet")
		export_gpx()
		interface()		
	elif txt_menu[selection_menu][1] == "Exit": 
		quit()

# def history():
# 	screen.clear()
# 	curses.noecho()
# 	curses.curs_set(0)


# 	screen.addstr(0,0,"q : quit | UP : previous | DOWN : next",curses.A_BOLD)
# 	hor_line = ''.join(['-']*(maxX))
# 	screen.addstr(1,0,hor_line)

# 	selection= -1
# 	option_menu = 0
# 	change = True
# 	while selection < 0:
# 		screen.nodelay(1)

# 		if change:
# 			blank = [" "]*(maxX)
# 			blank = ''.join(blank)
# 			for i in range(len(interface_shell)-1,-1,-1):
# 				j = len(interface_shell)-1-i
# 				if j == (maxY-2): 
# 					break
# 				screen.addstr( j+1 ,24,blank)#Blank
# 				tmp = datetime.datetime.fromtimestamp(interface_shell[i][0])#Parse time
# 				screen.addstr( j+1 ,
# 								0,
# 								"[%s] : %s" %(tmp.strftime('%d-%m-%Y %H:%M:%S'),interface_shell[i][1]))#Write
# 			screen.refresh()
# 			change = False

# 		action = screen.getch()

# 		if action == curses.KEY_UP:
# 			option_menu = (option_menu - 1)%len(txt_menu)
# 			change = True
# 		elif action == curses.KEY_DOWN:
# 			option_menu = (option_menu + 1) %len(txt_menu)
# 			change = True
# 		elif action == ord('q'):
# 			selection = option_menu

def calcule():
	if files != list():
		draw_shell("Select file")

		selection_file = -1
		option_file = 0
		files.append("BACK")

		while selection_file < 0:
			screen.nodelay(1)

			#Menu
			graphics = [0]*len(files)
			graphics[option_file] = curses.A_REVERSE

			for i in range(len(files)):
				screen.addstr(i+1, width_menu_min + txt_files[0] + 2,files[i],graphics[i])	
			screen.refresh()
			action = screen.getch()

			if action == curses.KEY_UP:
				option_file = (option_file - 1)%len(files)
			elif action == curses.KEY_DOWN:
				option_file = (option_file + 1) %len(files)
			elif action == ord('\n'):
				selection_file = option_file

		if not "BACK" in files[selection_file]:
			select_file = files[selection_file]
			draw_shell("File select : %s"%select_file)

			txt_file_okay = ["YES","NO"]
			select = pad_generator("Launch calcule",txt_file_okay)

			if "YES" in select:
				name,ext = select_file.split(".")
				
				if not path.exists("data/%s"%name):
					makedirs("data/%s"%name)
				else : 
				
					txt_file_data = ["NEW SET","REWRITE"]
					select = pad_generator("Rewrite data",txt_file_data,True)

					if "NEW" in select:
						for i in range(1,10000):							
							if not path.exists("data/%s_%s"%(name,i)):
								makedirs("data/%s_%s"%(name,i))
								file_location_data = "data/%s_%s"%(name,i)
								break
						draw_shell("New set data in : %s"%file_location_data)
						launch_calc(file_location_data,select_file)
					elif "REW" in  select:
						file_location_data = "data/%s"%name
						launch_calc(file_location_data,select_file)

	else : 
		draw_shell("No file for calcul")

def launch_calc(folder,filename):
	draw_shell("Calcul launched")
	data_in_processing = processing_data("data/%s"%filename)
	draw_shell("Number of data : %s" %data_in_processing.next())
	data_calc,average,variance,ecart,sat,dop,cov = data_in_processing.next()
	draw_shell("Data computed")
	export_data("%s/data_calc"%folder ,data_calc)
	export_over("%s/average"%folder ,average)
	export_over("%s/variance"%folder ,variance)
	export_over("%s/cov"%folder ,cov)
	export_over("%s/dop"%folder ,dop)
	export_over("%s/ecart"%folder ,ecart)
	draw_shell("Data exported")
	graph(variance, ecart,sat,folder)
	draw_shell("Graph generated")

def export_gpx():
	if files != list():
		draw_shell("Select file")

		selection_file = -1
		option_file = 0
		files.append("BACK")

		while selection_file < 0:
			screen.nodelay(1)

			#Menu
			graphics = [0]*len(files)
			graphics[option_file] = curses.A_REVERSE

			for i in range(len(files)):
				screen.addstr(i+1, width_menu_min + txt_files[0] + 2,files[i],graphics[i])	
			screen.refresh()
			action = screen.getch()

			if action == curses.KEY_UP:
				option_file = (option_file - 1)%len(files)
			elif action == curses.KEY_DOWN:
				option_file = (option_file + 1) %len(files)
			elif action == ord('\n'):
				selection_file = option_file
		
		file_to_export = files[selection_file]
		if not "BACK" in file_to_export and ("itinary" in file_to_export or "waypoint" in file_to_export ):
			name,ext = file_to_export.split(".")
			
			if "itinary" in file_to_export:
				if  path.exists("data/%s.gpx"%name):
					txt_files_gpx = ["NEW SET","REWRITE"]					
					select = pad_generator("GPX file already exist",txt_files_gpx,True)
					
					if "NEW" in select:
						draw_shell()
						for i in range(1,10000):							
							if not path.exists("data/%s_%s.gpx"%(name,i)):
								file_location_data = "data/%s_%s"%(name,i)
								break
					else:
						file_location_data = "data/%s"%name
				else:
					file_location_data = "data/%s"%name
				
				draw_shell("New GPX data in : %s.gpx"%file_location_data)

				gpx = gpxpy.gpx.GPX()
				gpx_track = gpxpy.gpx.GPXTrack()
				gpx.tracks.append(gpx_track)
				gpx_segment = gpxpy.gpx.GPXTrackSegment()
				gpx_track.segments.append(gpx_segment)
				
				data = list()
				file = csv.reader(open("data/%s"%file_to_export,"rb"))
				
				for rows in file:
					gpx_segment.points.append(gpxpy.gpx.GPXTrackPoint(float(rows[0]),float(rows[1]), elevation=float(rows[11])))
				
				createGPXFile(file_location_data,gpx)

			elif "waypoint" in file_to_export:
				if  path.exists("data/%s.gpx"%name):
					txt_files_gpx = ["NEW SET","REWRITE"]					
					select = pad_generator("GPX file already exist",txt_files_gpx,True)
					
					if "NEW" in select:
						draw_shell()
						for i in range(1,10000):							
							if not path.exists("data/%s_%s.gpx"%(name,i)):
								file_location_data = "data/%s_%s"%(name,i)
								break
					else:
						file_location_data = "data/%s"%name
				else:
					file_location_data = "data/%s"%name
				
				draw_shell("New GPX data in : %s.gpx"%file_location_data)

				gpx = gpxpy.gpx.GPX()
				data = list()
				file = csv.reader(open("data/%s"%file_to_export,"rb"))

				for rows in file:
					d =  int(float(rows[5]))
					mo = int(float(rows[6]))
					y =  int(float(rows[7]))
					h =  int(float(rows[8]))
					m =  int(float(rows[9]))
					s =  int(float(rows[10]))
					writeWaypoint(gpx,"%s-%s-%s %s:%s:%s"%(d,mo,y,h,m,s),float(rows[0],float(rows[1])))

				createGPXFile(file_location_data,gpx)
			else:
				draw_shell("Error selection file to export")
		else:
				draw_shell("Error selection file to export")
	else : 
		draw_shell("No file for calcul")

def createGPXFile(name,gpx):
	file = open(name+'.gpx','w')
	file.write(gpx.to_xml())
	file.close()

def writeWaypoint(gpx,name,lat,lon):
	gpx_waypoint = gpxpy.gpx.GPXWaypoint()
	gpx_waypoint.name = name
	gpx_waypoint.latitude=lat
	gpx_waypoint.longitude =lon
	gpx.waypoints.append(gpx_waypoint)


##Arduino function
def list_tty():
	
	global port_connect

	curses.noecho()
	curses.curs_set(0)

	possible_mountpoint = list()

	mountpoint = ((popen('ls /dev/tty*').read()).split("\n"))[:-1]

	for i in range(len(mountpoint)):
		if ("/dev/ttyACM" in mountpoint[i]) or ("/dev/ttyUSB" in mountpoint[i]):
			possible_mountpoint.append(mountpoint[i])
	if len(possible_mountpoint) <=0:
		possible_mountpoint.append("no found point")

	select = pad_generator("Mount point",possible_mountpoint,True)

	if ( "dev"  in select ): 
		port_connect = select
		draw_shell("Port for connection :%s"%port_connect)
		serial_Arduino()

def serial_Arduino(commande=''):
	global arduino
	global interface_shell
	global connection_arduino
	begin_time = time()

	if not connection_arduino:
		data = list()

		arduino.port = port_connect


		while not arduino.isOpen():
			arduino.open()
		if arduino.isOpen() :
			draw_shell("Port %s is open"%arduino.port)
		sleep(3)
		arduino.flushInput()

		arduino.write('rd')
		while arduino.isOpen():
			
			if arduino.inWaiting() > 0:
				data.append((arduino.read()).decode("utf-8"))			
			if data != list():
				if data[-1] == "\r":
					break
			
			if time()-begin_time > 10:
				break
		
		data = ''.join(data[:-1])
		
		if "yp" in data:
			draw_shell("Arduino is ready")
			connection_arduino = True
		else :
			draw_shell("Error connection to Arduino")
	
	else :
		download = False
		if "sen" in commande:
			if not arduino.isOpen():
				connection_arduino = False 
				serial_Arduino()
				
			draw_shell("Download in progress")
			data_t = list()
			data = list()
			arduino.flushInput()
			arduino.write(commande)
			sleep(3)
			
			while arduino.isOpen():
				if arduino.inWaiting() > 0:
					data.append((arduino.read()).decode("utf-8"))
					begin_time = time()
				if data != list():
					if data[-1] == "\r" or data[-1] == "\n":
						data = ''.join(data[:-1])
						if "ok" in data: 
							draw_shell(data)
							download = True
							break
						elif "er" in data:
							break
						else:
							if data != "":
								data_t.append(data)
								draw_shell(len(data_t))
							data = list()
				if time()-begin_time > 5:
					draw_shell("Time out")
					break
				sleep(0.001)

			if not download:
				draw_shell("Error downloading")
			else :	
				draw_shell("Download complete")
				
				#Give a name for the file datas

				curses.echo()
				curses.curs_set(1)
				screen.nodelay(0)
				file_name = str()
				draw_shell("Name file ? :")
				file_name = screen.getstr((maxY- height_shell_min)+1, 37, 20)
				
				if file_name == str():
					if "itinary" in commande:
						for i in range(1,10000000):	
							file_name = "itinary_%s"%i
							if not path.exists(file_location+file_name+".csv"):
								break
					elif "waypoint" in commande:
						for i in range(1,10000000):	
							file_name = "waypoint_%s"%i
							if not path.exists(file_location+file_name+".csv"):
								break
				
				while path.exists(file_location+file_name+".csv"):
					draw_shell("File already exist")
					draw_shell("Name file ? :")
					file_name = screen.getstr((maxY- height_shell_min)+1, 37, 20)

				screen.refresh()
				#Create file 
				newfile = csv.writer(open("%s%s.csv"%(file_location, file_name), "wb"))
														
				for i in range(len(data_t)):
					data = data_t[i].split()[0:]
					newfile.writerow(data)
				
				draw_shell("Create a new file : %s | %s Points" %(file_name,len(data_t)))

		elif "rem" in commande:
			if not arduino.isOpen():
				connection_arduino = False 
				serial_Arduino()
			else :
				draw_shell(commande)
				data = list()
				arduino.flushInput()
				arduino.write(commande)
				sleep(2)
				
				while arduino.isOpen():
					if arduino.inWaiting() > 0:
						data.append((arduino.read()).decode("utf-8"))
					if data != list():
						if data[-1] == "\r":
							break
				
				data = ''.join(data[:-1])
				if "deleting" in data:
					draw_shell("Delleting completed")

##GPX function

##Draw function
def pad_generator(title,list_elmt,btn_back=False):
	if btn_back:
		list_elmt.append("BACK")
	
	pad = curses.newpad(len(list_elmt)+3, 30)
	pad.addstr(1,int(30/2-len(title)/2),title,curses.A_UNDERLINE)
	pad.border()
	y = int(maxY/2 - len(list_elmt)/2)-1
	x = int(maxX/2 - 30/2)


	option = 0
	selection = -1

	while selection < 0:
		pad.nodelay(1)

		#Menu
		graphics = [0]*len(list_elmt)
		graphics[option] = curses.A_REVERSE

		for z in range(len(list_elmt)):
			pad.addstr(z+2,int(30/2-len(list_elmt[z])/2),list_elmt[z],graphics[z])
		
		action = screen.getch()
		if action == curses.KEY_UP:
			option = (option - 1)%len(list_elmt)
		elif action == curses.KEY_DOWN:
			option = (option + 1) %len(list_elmt)
		elif action == ord('\n'):
			selection = option

		pad.refresh(0,0,y,x, 50,100)
	return list_elmt[selection]

def draw_limit_gui():
	#Vertical line menu
	for i in range(maxY - height_shell_min):
		screen.addstr(i,width_menu_min,'|')
	
	#Vertical market data
	for i in range(maxY - height_shell_min):
		screen.addstr(i, (width_menu_min + width_gpx_min) ,'|')
		
	#Line horzontale menu
	hor_line_left = ['-']*(width_menu_min)
	hor_line_left = ''.join(hor_line_left)
	screen.addstr(height_menu_min+3,0,hor_line_left)

	#Line horzontale history
	hor_line_history = ['-']*(maxX - 1)
	hor_line_history = ''.join(hor_line_history)
	screen.addstr(maxY - height_shell_min, 0,hor_line_history)	

def draw_connect():
	txt_connect[1][1] = port_connect
	for z in range(len(txt_connect)):
		screen.addstr(z+height_menu_min+4,txt_connect[z][0] , txt_connect[z][1],txt_connect[z][2])

def draw_files():
	global files

	screen.addstr(0, width_menu_min + txt_files[0] + 1,txt_files[1],txt_files[2])
	
	if not path.exists(file_location):
		makedirs(file_location)
	else :
		files = listdir(file_location)
	if files == list():
		screen.addstr(1, width_menu_min + txt_files[0] + 2,"No file",curses.A_NORMAL)
	else:
		temp_files = list()
		for i in range(len(files)):
			if files[i].endswith(".csv"):
				temp_files.append(files[i])
		files = temp_files
		for i in range(len(files)):
			screen.addstr(i+1, width_menu_min + txt_files[0] + 2,files[i],curses.A_NORMAL)		

def draw_files_gpx():
	global files_gpx

	screen.addstr(0, width_menu_min + width_gpx_min + txt_files_gpx[0] + 1,txt_files_gpx[1],txt_files_gpx[2])
	
	if not path.exists(file_location):
		makedirs(file_location)
	else :
		files_gpx = listdir(file_location)
	if files_gpx == list():
		screen.addstr(1, width_menu_min + width_gpx_min + txt_files_gpx[0] + 2,"No file",curses.A_NORMAL)
	else:
		temp_files = list()
		for i in range(len(files_gpx)):
			if files_gpx[i].endswith(".gpx"):
				temp_files.append(files_gpx[i])
		files_gpx = temp_files
		for i in range(len(files_gpx)):
			screen.addstr(i+1, width_menu_min +width_gpx_min+ txt_files_gpx[0] + 2,files_gpx[i],curses.A_NORMAL)	

def draw_shell(data=''):
	global interface_shell
	
	if data != '':
		interface_shell.append((time(),str(data)))

	rshell = list()
	rshell.extend(interface_shell)
	blank = [" "]*(maxX- width_menu_min-2)
	blank = ''.join(blank)
	for i in range(len(rshell)-1,-1,-1):
		j = len(rshell)-1-i
		if j == 9: 
			break
		screen.addstr( maxY- height_shell_min + 1 + j ,24,blank)#Blank
		tmp = datetime.datetime.fromtimestamp(rshell[i][0])#Parse time
		screen.addstr( maxY- height_shell_min + 1 + j ,
						0,
						"[%s] : %s" %(tmp.strftime('%d-%m-%Y %H:%M:%S'),rshell[i][1]))#Write
	screen.refresh()

def quit():
	screen.clear()
	screen.nodelay(0)

	selection_quit = -1
	option_quit = 0

	while selection_quit <0:
		graphics = [0]*2
		graphics[option_quit] = curses.A_REVERSE
		txt1 = "Are you sure to quit ?"
		screen.addstr(int(dims[0]/2-2),int(dims[1]/2-len(txt1)/2),txt1,curses.A_BOLD)
		txt_yes = "YES"
		screen.addstr(int(dims[0]/2),int(dims[1]/2-len(txt_yes)-1),txt_yes,graphics[0])
		txt_no = "NO"
		screen.addstr(int(dims[0]/2),int(dims[1]/2+len(txt_no)),txt_no,graphics[1])
		screen.refresh()
		action = screen.getch()

		if action == curses.KEY_LEFT:
			option_quit = (option_quit - 1)%2
		elif action == curses.KEY_RIGHT:
			option_quit = (option_quit + 1)%2
		elif action == ord('\n'):
			selection_quit = option_quit

	if selection_quit == 1:
		interface()

begin_screen()
interface()
arduino.close()
curses.endwin() #Close curses