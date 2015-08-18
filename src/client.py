import socket

ip = raw_input("Enter ip address: ")
msg = raw_input("Enter message:")
clientsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
clientsocket.connect(('localhost', 7891))
clientsocket.send(msg+'\n')
