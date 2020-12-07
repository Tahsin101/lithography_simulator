# -*- coding: utf-8 -*-
"""
Created on Tue Apr 28 22:11:14 2020

@author: Anika
"""

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

from scipy import interpolate
main512=pd.read_csv("MAIN512.csv")
main128=pd.read_csv("main128.csv")
main100=pd.read_csv("main100.csv")
main10=pd.read_csv("main10.csv")
real512=main512.loc[:,"real"].values
imag512=main512.loc[:,"imag"].values

real128=main128.loc[:,"real"].values
imag128=main128.loc[:,"imag"].values
imx=2048
shift=512
step=int(imx/shift)
index_x=np.arange(shift,2048+shift,shift,dtype=int)
index_y=np.arange(shift,2048+shift,shift,dtype=int)
real512_new=[]
np.array(real512_new)
for i in range(len(index_x)):
    f512 = interpolate.interp1d(index_y, real512[0+i*step:len(index_y)*(i+1)])
    index_y_new=np.arange(shift,2048+1,1,dtype=int)
    real512_new[0+i*len(index_y_new):len(index_y_new)+i*len(index_y_new)]=(f512(index_y_new))
    plt.figure()
    plt.title('fixed width = %i'%(shift+i*shift))
    plt.plot(index_y_new,real512_new[0+i*len(index_y_new):len(index_y_new)+i*len(index_y_new)])
    plt.xlabel("various height")
    plt.ylabel("real intensity")
    plt.grid()
    plt.scatter(index_y, real512[0+i*step:len(index_y)*(i+1)])

imx=2048
shift=128
step=int(imx/shift)
index_x=np.arange(shift,2048+shift,shift,dtype=int)
index_y=np.arange(shift,2048+shift,shift,dtype=int)
real128_new=[]
imag128_new=[]
index_array=[]
np.array(real128_new)
np.array(imag128_new)
np.array(index_array)
for i in range(len(index_x)):
    f128 = interpolate.interp1d(index_y, real128[0+i*step:len(index_y)*(i+1)])
    index_y_new=np.arange(shift,2048+1,1,dtype=int)
    real128_new[0+i*len(index_y_new):len(index_y_new)+i*len(index_y_new)]=(f128(index_y_new))
    g128 = interpolate.interp1d(index_y, imag128[0+i*step:len(index_y)*(i+1)])
    imag128_new[0+i*len(index_y_new):len(index_y_new)+i*len(index_y_new)]=(g128(index_y_new))

    plt.figure()
    plt.title('fixed width = %i'%(shift+i*shift))
    plt.xlabel("various height")
    plt.ylabel("real intensity")
    plt.plot(index_y_new,real128_new[0+i*len(index_y_new):len(index_y_new)+i*len(index_y_new)])
    plt.grid()
    plt.scatter(index_y, real128[0+i*step:len(index_y)*(i+1)])
d=[]
x=np.arange(128,2049,1)
y=np.arange(128,2049,1)
for i in range(len(x)):
    for j in range(len(y)):
        
        c=str(x[i])+"_"+str(y[j])
        d.append(c)

#    plt.figure()
#    plt.title('fixed width = %i'%(shift+i*shift))
#    plt.xlabel("various height")
#    plt.ylabel("real intensity")
#    plt.plot(index_y_new,real128_new[0+i*len(index_y_new):len(index_y_new)+i*len(index_y_new)])
#    plt.grid()
#    plt.scatter(index_y, real128[0+i*step:len(index_y)*(i+1)])


#anika=np.zeros((step,step),dtype=float)
#for i in range(len(index_x)):
#  plt.figure()
#  plt.title('fixed width = %i'%(shift+i*shift))
#  plt.xlabel("various height")
#  plt.ylabel("real intensity")
#  plt.grid()
#  plt.scatter(index_y, real512[0+i*step:len(index_y)*(i+1)])
#  
#imx=2048
#shift=128
#step=int(imx/shift)
#index_x=np.arange(shift,2048+shift,shift,dtype=int)
#index_y=np.arange(shift,2048+shift,shift,dtype=int)
##anika=np.zeros((step,step),dtype=float)
#for i in range(len(index_x)):
#  plt.figure()
#  plt.title('fixed width = %i'%(shift+i*shift))
#  plt.xlabel("various height")
#  plt.ylabel("real intensity")
#  plt.grid()
#  plt.scatter(index_y, real128[0+i*step:len(index_y)*(i+1)])
  

