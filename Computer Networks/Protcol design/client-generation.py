#!/usr/bin/python           
import random
import socket               # Import socket module
import pickle
import random 

s = socket.socket()         # Create a socket object
host = socket.gethostname() # Get local machine name/for this example
ports = 12345             #server port
portc = 32451 	   #client's own port for incoming connections (if any)
s.bind((host, portc))
s.connect((host, ports))
print ("\n\n",s.recv(1024))
x=input()
xp=pickle.dumps(x)
s.send(xp)


  
# using list comprehension + randrange()  
# to generate random number list 

n=input("enter size of array desired\n")#+ sep # sep = ' ' or sep = `\n`
len=int(n)
size=pickle.dumps(len)
#len=random.randrange(25,100)
res = [random.randrange(1, 100, 1) for i in range(len)] 
print("\n\ninitial unsorted list")
print(res)
data=pickle.dumps(res)
res.sort()
print("\n\ninitial list sorted")
print(res)

s.send(size)
print(s.recv(1024))
s.send(data)
if(x==1):
    val=s.recv(1024)
    flist=pickle.loads(val)
    print("\n\nfinal list :")
    print(flist)

rp=s.recv(1024)
repeated=pickle.loads(rp)
if(x=='1'):
    print("\n\nrepeated values")
else:
    print("\n\ncommon values")
repeated.sort()
print(repeated)
#str1="thank you for the service"
#msg=str1.encode
s.send(b"client:  thank you for the service")
s.close                     # Close the socket when done
