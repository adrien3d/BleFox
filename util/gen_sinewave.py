import math

def frange(x, y, jump):
   while x < y:
     yield x
     x += jump

print [int(round(math.sin(x)*255)) for x in frange(0, math.pi, math.pi/254)]

#print [int(round( ((math.exp(math.sin(x/2000.0*math.pi)) - 0.36787944)*108.0)*255 )-17400) for x in frange(0, 2*math.pi, math.pi/254)]
