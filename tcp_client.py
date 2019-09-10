from socket import*

target_host = "www.google.com"
target_port = 80

# create a socket object
client = socket(AF_INET,SOCK_STREAM)

# connect the client
client.connect((target_host,target_port))

#send some data 
client.send("GET /HTTP/1.1\r\Host: google.com\r\n\r\n")

# recieve some data 
response = client.recv(4096)

print response 
