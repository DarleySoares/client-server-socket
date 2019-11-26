# Client-server with socket

The project establishes a socket server that waiting for a message from the client. When one message arrives of the client, the server change upper letters for lower letters and vice versa, as well as the order of the sentence.

## How to compile

It's necessary use the happycoders package for this:

> sudo apt-get install happycoders-libsocket-dev

After, move the folder to usr/lib:

> sudo mv usr/lib/happycoders/* usr/lib

Finally,

> gcc -g -o clienteTCP cliente_tcp.c -lsocket -lnsl

> gcc -g -o servidorTCP servidor_tcp.c -lsocket

## How to run the code

> ./servidorTCP [port]

> ./clienteTCP 127.0.0.1 [port]
