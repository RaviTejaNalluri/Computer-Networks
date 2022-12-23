#!/usr/bin/python

import socket               # Import socket module
import pickle,random



def my_function(x):
  return list(dict.fromkeys(x))
  
def Repeat(x): 
    _size = len(x) 
    repeated = [] 
    for i in range(_size): 
        k = i + 1
        for j in range(k, _size): 
            if x[i] == x[j] and x[i] not in repeated: 
                repeated.append(x[i]) 
    
    return repeated


def Deduplication(lst):
    rp=Repeat(lst)
    nrp=[]
    for i in range(len(lst)):
        if(lst[i] not in rp):
            nrp.append(lst[i])
    final=rp+nrp
    final.sort()
    data=pickle.dumps(final)
    c.send(data)
    data2=pickle.dumps(rp)
    c.send(data2)

def Intersection(lst):
    size=len(lst)
    serv_list = [random.randrange(1, 100, 1) for i in range(size)] 
    common_list=[]
    print(serv_list)
    for i in lst:
        if i in serv_list and i not in common_list:
            common_list.append(i)
    print(common_list)
    data=pickle.dumps(common_list)
    c.send(data)





s = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)         # Create a socket object
host = socket.gethostbyname(socket.gethostname()) # Get local machine name
port = 12345                # Reserve a port for your service.
s.bind(('0.0.0.0', port))        # Bind to the port

print ('host ip', host)
s.listen(5)                 # Now wait for client connection.

x=[1,5,2,3,1,7,9]
while True:
    c, addr = s.accept()     # Establish connection with client.
    print ('Got connection from', addr)
    c.send( b'Server : Choose Number For service  1)Deduplication  2)Intersection')
    

    t=c.recv(1024)
    x=pickle.loads(t)

    c.send(b'size received')
    size=pickle.loads(c.recv(1024))
    print(size)

    recvd_data=c.recv(1024)
    lst=pickle.loads(recvd_data)
    if(x=='1'):
        Deduplication(lst)
    else:
        Intersection(lst)
    
    
    print(c.recv(1024))
    c.close()                # Close the connection