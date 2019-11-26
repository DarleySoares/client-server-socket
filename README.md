# Client-server with socket

The project establishes a socket server that waiting for a message from the client. When one message is arrived from the client, the server changes that message switching lowercase to uppercase, and vice versa, the server also changes the order of sentence 

## How to compile

Install happycoders package:

> sudo apt-get install happycoders-libsocket-dev

After, move the folder to usr/lib:

> sudo mv usr/lib/happycoders/* usr/lib

Finally,

> gcc -g -o clienteTCP cliente_tcp.c -lsocket -lnsl

> gcc -g -o servidorTCP servidor_tcp.c -lsocket

## How to run the code

> ./servidorTCP [port]

> ./clienteTCP 127.0.0.1 [port]
