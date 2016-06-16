# from math import tan
# from math import ln
# from math import sin


# def Lambert93(phi,e):
# 	return ln(tan(pi/4+phi/2))*(((1-e*sin(phi))/(1+e*sin(phi)))**(e/2))

def bal():
	yield "one"
	yield "two"

a= bal()

print a.next()
print a.next()
print a.next()
