import requests

#import pandas as pd

import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

sns.set()

filtered_lines = []
extracted_info = []
extracted_info_square = []

response = requests.get('http://192.168.2.33/Index.ssi')

with open("temperatureData.txt", "w") as file:
	file.write(response.text)

fobj1 = open("temperatureData.txt")
for line in fobj1:	
	if line.startswith('	        <th><!--#text'):
		filtered_lines.append(line)
fobj1.close

with open("temperatureDataRendered.txt", "w") as fobj2:
	fobj2.writelines(filtered_lines)
fobj2.close

#workbook = xlsxwriter.Workbook("TempData.xlsx")
#worksheet = workbook.add_worksheet()

fobj3 = open("temperatureDataRendered.txt", "r")
for line in fobj3:
	part = line.split(">")
	extracted_info.append(part[2])
	#worksheet.write(n,0,part[2])
	#n=n+1	
fobj3.close

#workbook.close()

with open("temperatureDataRenderedSquare.txt", "w") as fobj4:
	fobj4.writelines(extracted_info)
fobj4.close

# IR-Sensor komplett
n=0

while n <= 767:

  fobj5 = open("temperatureDataRenderedSquare.txt", "r")
  for line in fobj5:	
	     part = line.split("</th")	
	     extracted_info_square.append(part[n])   
	     with open("IR-Sensor-komplett.txt", "w") as fobj6:		  
	        fobj6.writelines(extracted_info_square)	   	       	        
	     fobj6.close	    	     	
  fobj5.close   
   
  n=n+1
  
vektor = np.loadtxt("IR-Sensor-komplett.txt")
matrix_24x32 = vektor.reshape(24,32)

plt.matshow(matrix_24x32)
plt.show(block=False)

input("press any key to continue...")
plt.pause(2)
plt.close(1)


#Z = np.loadtxt("IR-Sensor-komplett.txt", unpack=True)
#plt.plot(Z, "ro")
#plt.show()
