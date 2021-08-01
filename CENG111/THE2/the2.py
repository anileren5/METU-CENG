#ANIL EREN GÖÇER	2448397
# WRITE YOUR CODE HERE AND SEND ONLY THIS FILE TO US.
#
# DO NOT DEFINE get_data() here. Check before submitting

import math
import random
from evaluator import *    # get_data() will come from this import

initialstate=get_data()[6] #This is the universal state which includes only individuals not constants at the beginning.

def new_move():
	global initialstate    #To update it, ı need to use it as a global variable.
	P = len(initialstate)  # The number of individuals in the arena
	M = get_data()[0]  	   # The number of rows
	N = get_data()[1]      # The number of columns
	D = get_data()[2]      # Threshold distance for infection
	K = get_data()[3]      # The constant (kappa) for infection possibility
	lam = get_data()[4]      # The constant (lambda) for mask protection possibility
	u = get_data()[5]      # The constant (mu) for movement possibility

	# Colours with movement possibilities
	green = (1 / 2) * (u)  # for forward
	yellow = (1 / 8) * (u)  # for forward-right and forward-left
	blue = (1 / 2) * (1 - u - (u ** 2))  # for right and left
	purple = (2 / 5) * (u ** 2)  # for backward-right and backward-left
	gray = (1 / 5) * (u ** 2)  # for backward

	preoccupiedpositions = []
	for individual in initialstate:
		preoccupiedpositions.append(individual[0])

	# Now we know which positions are preoccupied and ı will updated the list regularly by counter i in the next loop.

	i=0

	for individual in initialstate:
		currentposition=individual[0]
		lastmove=individual[1]
		x=currentposition[0]
		y=currentposition[1]
		p0=[(x,y+1),0]
		p1=[(x-1,y+1),1]
		p2=[(x-1,y),2]
		p3=[(x-1,y-1),3]
		p4=[(x,y-1),4]
		p5=[(x+1,y-1),5]
		p6=[(x+1,y),6]
		p7=[(x+1,y+1),7]
		list0=[p0,p1,p2,p3,p4,p5,p6,p7] #All list are order in such a format: [forward,forward-right,backward-right,backward,backward-left,left,forward-left]
		list1=[p1,p2,p3,p4,p5,p6,p7,p0]
		list2=[p2,p3,p4,p5,p6,p7,p0,p1]
		list3=[p3,p4,p5,p6,p7,p0,p1,p2]
		list4=[p4,p5,p6,p7,p0,p1,p2,p3]
		list5=[p5,p6,p7,p0,p1,p2,p3,p4]
		list6=[p6,p7,p0,p1,p2,p3,p4,p5]
		list7=[p7,p0,p1,p2,p3,p4,p5,p6]


		chosen=random.choices([0,1,2,3,4,5,6,7],weights=[green,yellow,blue,purple,gray,purple,blue,yellow])[0]
		#chosen gives me the direction(forward ,forward-right,backwardright,........,forward-left)
		if lastmove==0:
			nextposition=list0[chosen][0]
			nextlastmove=list0[chosen][1]
		if lastmove==1:
			nextposition=list1[chosen][0]
			nextlastmove=list1[chosen][1]
		if lastmove==2:
			nextposition=list2[chosen][0]
			nextlastmove=list2[chosen][1]
		if lastmove==3:
			nextposition=list3[chosen][0]
			nextlastmove=list3[chosen][1]
		if lastmove==4:
			nextposition=list4[chosen][0]
			nextlastmove=list4[chosen][1]
		if lastmove==5:
			nextposition=list5[chosen][0]
			nextlastmove=list5[chosen][1]
		if lastmove==6:
			nextposition=list6[chosen][0]
			nextlastmove=list6[chosen][1]
		if lastmove==7:
			nextposition=list7[chosen][0]
			nextlastmove=list7[chosen][1]

		if (nextposition[0] in range(0,N)) and (nextposition[1] in range(0,M)) and (nextposition not in preoccupiedpositions):
			individual[0]=nextposition
			preoccupiedpositions[i]=nextposition
			individual[1]=nextlastmove
		i+=1

	#By this loop we have managed to update the positions and lastmove of each individual.

	#All moves are performed. The contamination stage begin now.


	oldinfectiousstates=[]
	for individual in initialstate:
		oldinfectiousstates.append(individual[3]) #This oldinfectious states make me able to know which individuals are infected before the infection stage begin.

	for i in range(0,P):
		for j in range(i+1,P):
			firstindividual=initialstate[i]
			secondindividual=initialstate[j]
			x0=firstindividual[0][0]
			y0=firstindividual[0][1]
			x1=secondindividual[0][0]
			y1=secondindividual[0][1]
			distance=math.sqrt(((x0-x1)**2)+((y0-y1)**2)) #This is the distance between individual that are in interaction.
			Pij=min(1,K/(distance**2)) #Probability function defined in the pdf, but now we do not consider masks, we will take them into account later.
			if distance<=D:
				if (oldinfectiousstates[i]=="infected") and (oldinfectiousstates[j]=="notinfected") :
					numberofmaskedindividual=0
					if firstindividual[2]=="masked":
						numberofmaskedindividual+=1
					if secondindividual[2]=="masked":
						numberofmaskedindividual+=1
					infectionprobability=Pij/(lam**numberofmaskedindividual) #Infection probability reduces as the number of masked people increase
					result=random.choices(["infected","notinfected"],weights=[infectionprobability,1-infectionprobability])[0]
					if result=="infected":
						secondindividual[3]="infected"




				if (oldinfectiousstates[i]=="notinfected") and (oldinfectiousstates[j]=="infected"):
					numberofmaskedindividual=0
					if firstindividual[2]=="masked":
						numberofmaskedindividual+=1
					if secondindividual[2]=="masked":
						numberofmaskedindividual+=1
					infectionprobability=Pij/(lam**numberofmaskedindividual)
					result=random.choices(["infected","notinfected"],weights=[infectionprobability,1-infectionprobability])[0]
					if result=="infected":
						firstindividual[3]="infected"

	return initialstate


