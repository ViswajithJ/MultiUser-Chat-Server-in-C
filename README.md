# MultiUser-Chat-Server-in-C

Basic implementation of a multiclient chat server in C using socket programming (Both TCP and UDP).
Done as part of Networking Lab Course

Server uses multithreading to handle different client instances.
Client uses multithreading to send and receive messages.

<h4>// TODO</h4>

- Clean up code
- Handle name and message display format in client side
- Same in UDP

<h3>Note</h3>

- While compiling, link pthread library
- Eg: `cc server.c -lpthread`

