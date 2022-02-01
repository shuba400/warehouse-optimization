import numpy as np

with open("input.txt","r") as file:
    input_str=file.read()
input=list(map(int,input_str.split()))

num_testcases=input[0]
num_rows=input[1]
num_cols=input[2]
num_orders=input[7]

freq=np.zeros((num_rows+1,num_cols+1),dtype=int)
ind=8
ctr=0

while ind<len(input):
    for i in range(ind+1,ind+2*input[ind],2):
        ctr+=1
        freq[input[i]][input[i+1]]+=1
    ind+=input[ind]*2+1 
print(ctr)

import matplotlib.pyplot as plt

x=[]
y=[]
z=[]

for i in range(1,num_rows+1):
    for j in range(1,num_cols+1):
        # if freq[i,j]>=1:
        #     freq[i,j]=1
        x.append(i)
        y.append(j)
        z.append(freq[i,j])

plt.scatter(x,y,c=z,cmap='Reds')
plt.show()
