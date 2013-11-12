demoddix
========

**demoddix** is an animation and visualization tool for debugging distributed communication systems.  
Debugging is based on a set of XML traces of events generated during system runtime.


Two groups of events can be visualized with **demoddix**:  

1. Network
  * packet sent,
  * packet received,
  * packet lost, and
  * state change.
  
2. Node
  * task creation and deletion,
  * task state change,
  * message sent, received and saved,
  * timer start, cancel, and timeout,
  * semaphore creation and deletion,
  * semaphore take, success, timeout, and give.

Network events are directly displayed in **demoddix**, instead Node events are forwarded to [MscTracer](http://www.pragmadev.com/product/tracing.html) for visualization.
