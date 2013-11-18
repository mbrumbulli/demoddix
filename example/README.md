## Introduction

This is a simple client-server application example for **demmoddix**. It consists of four trace files representing
runtime events of four nodes (one server and three clients). The system was developed using **moddix**.  

## Architecture

The application consists of two processes (*pClient* and *pServer*) that exchange *mRequest* and *mReply* messages.

![alt text](https://github.com/mbrumbulli/demoddix/raw/master/example/architecture.png "client-server: architecture")

## Behaviour

### pClient

The client sends *mRequest* message to *pServer* and waits for *mReply*. Upon receiving a *mReply*, 
it waits for *DELAY* milliseconds and then sends a new *mRequest*.
If a *mReply* is not received before *TIMEOUT* milliseconds, the *pClient* terminates.

![alt text](https://github.com/mbrumbulli/demoddix/raw/master/example/client.png "client-server: pClient behaviour")

### pServer

The server waits for *mRequest(s)* from *pClient(s)*. Upon receiving a *mRequest*, it send a *mReply* to the *pClient* 
if *MAX_COUNTER* is not reached; otherwise it terminates (causing also all *pClient(s)* to to the same).

![alt text](https://github.com/mbrumbulli/demoddix/raw/master/example/server.png "client-server: pServer behaviour")

## Deployment

Four nodes are created: the server is running on the node with id = 0; 
the clients are running on the nodes with id = 1, 2, 3.
For simplicity, all nodes are configured to run locally (IP 127.0.0.1), but with different ports.

![alt text](https://github.com/mbrumbulli/demoddix/raw/master/example/deployment.png "client-server: deployment")
