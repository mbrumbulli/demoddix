Introduction
------

**demoddix** is the <b>de</b>bugger for **moddix**.  
**moddix** is a methodology and tool for <b>mo</b>del-<b>d</b>riven development of 
<b>di</b>stributed <b>c</b>ommunication <b>s</b>ystems based on [SDL-RT](http://www.sdl-rt.org).
Tool support is provided as a profile for [RTDS](http://www.pragmadev.com/product/modeling.html).
It can generate code for Linux, [ns-3](http://www.nsnam.org), and [OdemX](http://sourceforge.net/projects/odemx/).

Features
------

**demoddix** is an animation and visualization tool that uses a set of trace files.  
These files contain a list of events (in XML format) generated during system runtime.
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

![alt text](https://github.com/mbrumbulli/demoddix1/raw/master/example/demoddix.gif "demoddix example - msctracer node 0")

Trace files can be generated automatically for each system developed with **moddix**.  
If **moddix** is not available, this distribution contains a XML Schema that can be used as a reference 
for generating such traces from any application.

Build
------

**demoddix** is available for Linux and requires: g++ (-std=c++11 -pthread), OpenGL, and freeglut.  
Run `make` to build.

Usage
------

To run the example provided with this distribution: `./demoddix example/*.xml`  

**Left** = Forward  
**Right** = Rewind  
**Up** = Next  
**Down** = Previous  
**Page Up** = Increment Step  
**Page Down** = Decrement Step  
**Home** = Reset Zoom  
**R** = Reset  
**P** = Pause MscTracer

**Mouse Left** = Open Color Chooser for State or Message  
**Hold Mouse Left + Mouse Move** = Move View for Nodes  
**Ctrl + Mouse Left** = Open MscTracer | Increment State Priority | Show / Hide Message  
**Ctrl + Mouse Right** = Decrement State Priority  
**Mouse Wheel** = Zoom | Scroll States or Messages

For performance reasons **demoddix** does not do any validation of the trace files against the schema. Nevertheless, 
prior to running **demoddix**, it is advisable to do such validation with tools like [xmllint](http://xmlsoft.org/xmllint.html).





