## C_socket_client_server

Code for one of my blog [post](https://leegenux.github.io/2019/02/15/code-your-own-server-client-with-C/).

There are 3 directory: `nslookup-in-C`, `tcp-client` and `tcp-server`.



### nslookup-in-c

The executable `nslookup` can help get ip (ipv4) from name.

For example:

```bash
$ nslookup google.com
172.217.161.174
```

It requires the correct DNS configuration and working network to operate.



### tcp-server

The executable `tcp-server` listens on `localhost:port`. It serves as a echo server, that is, echoes back the data sent from `tcp-client`.

The port should be given as command line parameter. 

Its help string : `Usage:  tcp-server <port>`



### tcp-client

The executable `tcp-client` will connect, send and receive the string you specify. To work together with `tcp-client` on localhost.

Look at the example below:

```
$ tcp-server/tcp-server 12334 1>tcp-server/tcp-server.log 2>&1 &
$ tcp-client/tcp-client 127.0.0.1 12334 "Hello world"
Echo:
Hello world
```

This is `tcp-client` 's help string : `Usage: tcp-client <server-ip> <port> <data>`