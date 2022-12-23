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
  
# using list comprehension + randrange()  
# to generate random number list 
len=random.randrange(25,100)
res = [random.randrange(1, 100, 1) for i in range(len)] 
print("\n\ninitial unsorted list")
print(res)
data=pickle.dumps(res)
res.sort()
print("\n\ninitial list sorted")
print(res)

s.send(data)
val=s.recv(1024)
flist=pickle.loads(val)
print("\n\nfinal list :")
print(flist)

rp=s.recv(1024)
repeated=pickle.loads(rp)
print("\n\nrepeated values")
repeated.sort()
print(repeated)
#str1="thank you for the service"
#msg=str1.encode
s.send(b"client:  thank you for the service")
s.close                     # Close the socket when done
