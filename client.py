#Cliente.py
import socket
     
s = socket.socket()
s.connect(("localhost", 9999))
     
while True:
      mensaje = raw_input("> ")
          
      s.send(mensaje)
      recibido = s.recv(1024)
      print recibido
      if mensaje == "quit":
         break
     
print "adios"
     
s.close()
