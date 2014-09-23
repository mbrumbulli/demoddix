## Introduction

This is a simple client-server application example for **demmoddix**. It consists of four trace files representing
runtime events of four nodes (one server and three clients). The application was developed using **moddix**.  

## Architecture

The application consists of three processes (*pClient*, *pServer*, and *pHandler*) that exchange *mRequest* and *mReply* messages.

![alt text](https://github.com/mbrumbulli/demoddix1/raw/master/example/architecture.png "client-server: architecture")

## Behaviour

### pClient

The client sends *mRequest* message to *pServer* and waits for *mReply*. Upon receiving a *mReply*, 
it waits for *DELAY* milliseconds and then sends a new *mRequest*.
If a *mReply* is not received before *TIMEOUT* milliseconds, the *pClient* terminates.

![alt text](https://github.com/mbrumbulli/demoddix1/raw/master/example/client.png "client-server: pClient behaviour")

### pServer

The server waits for *mRequest(s)* from *pClient(s)*. Upon receiving a *mRequest*, it creates a *pHandler* and sends a *mHandle* to it.
This establishes a one-to-one relationship between the sending *pClient* and the created *pHandler*.

![alt text](https://github.com/mbrumbulli/demoddix1/raw/master/example/server.png "client-server: pServer behaviour")

###pHandler
The handler, upon receiving a *mHandle*, increments the *counter*; this operation is guarded by the *lockCounter* semaphore.
If the counter reaches *MAX* then *mStop* is send and execution terminates, otherwise *mReply* is sent to the client.

![alt text](https://github.com/mbrumbulli/demoddix1/raw/master/example/handler.png "client-server: pHandler behaviour")

## Deployment

Four nodes are created: the server is running on the node with id = 0 (192.168.1.1:50000); 
the clients are running on the nodes with id = 1, 2, 3.

![alt text](https://github.com/mbrumbulli/demoddix1/raw/master/example/deployment.png "client-server: deployment")
