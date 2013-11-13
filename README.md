demoddix
========

The **De**bugger for **Mo**del-**D**riven **Di**stributed **C**ommunication **S**ystems is an animation 
and visualization tool build in C++ / OpenGL.
It uses a set of XML traces of events generated during system runtime.


Two groups of events can be visualized with **demoddix**:  

1. Network
  * packet sent,
  * packet received,
  * packet lost, and
  * node state change.
  
2. Node
  * task creation and deletion,
  * task state change,
  * message sent, received and saved,
  * timer start, cancel, and timeout,
  * semaphore creation and deletion,
  * semaphore take attempt, take success, take timeout, and give.

Network events are directly displayed in **demoddix**, instead Node events are forwarded to [MscTracer](http://www.pragmadev.com/product/tracing.html) for visualization.

![alt text](https://github.com/mbrumbulli/demoddix/raw/master/example/demoddix.gif "demoddix example - msctracer node 0")
