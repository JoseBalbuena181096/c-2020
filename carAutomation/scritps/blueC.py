from bluetooth import *
import os
import time

client=BluetoothSocket( RFCOMM )
client.connect(('00:18:E5:04:8E:19', 1))
try:
    while 1:
        data = client.recv(1024)
        #data_str = data.decode('utf-8')
        if data:
            print(data)
except:
    client.close()
          
