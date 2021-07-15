# networkTask
Task: 
Write a client-server program using sockets berkeley and multithreading.
The application must transfer information from the "client" to "server" and vice versa.

-----
- Protocol TCP\IP (IP - 127.0.0.1 PORT - 8080). (can change in make)
- Programming language С.
- Project files:  makefile (gcc/pthread/), clientServer.c, clientServer.h, README (how to use).
-----



# make project
make 

#create 2 shell 

# In first shell(1) start Server 
./clientServer Server

# In second shell(2) start Client
./clientServer Client

# Now you can send data to/from Server/Client
