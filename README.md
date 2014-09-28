UE4OSC 1.0 - created by Zach Duer

Readme last updated Sept 27, 2014
======
Description:
A plugin for Unreal Engine 4 allowing it to communicate with other applications over the Open Sound Control protocol. 
The source for both the plugin as well as the UE4OSC DLL file are included

======
Installation Instructions:
() Install Unreal Engine 4 https://docs.unrealengine.com/latest/INT/GettingStarted/Installation/index.html
() Create a new project
() In the folder of your new project, create a new folder called Plugins
() Copy UE4OSCPlugin folder to that directory
() Open project in UE4
() In UE4, go to Window -> Plugins, and enable UE4OSC (under Communication category)
() Create a new Blueprint inheriting from the UE4OSCPluginInterface (for blueprint tutorial videos: https://www.youtube.com/playlist?list=PLZlv_N0_O1gbYMYfhhdzfW1tUV4jU0YxH)
() You now have access to all the Blueprint functions of UE4OSC!

======
Current Blueprint functions available:
OSCSetHostnameAndPort
---This function allows you to set the IP address for communications
---If you're not sure what your IP address is, run cmd and in the command prompt, type ipconfig
---use the IPv4 address

OSCStartReceiver
---Call this function to start receiving OSC message from other programs

OSCGetFloat
---This function returns the float in the most recently received OSC message with the /Float address pattern

OSCGetFloatArray
---This function returns the array of floats in the most recently received OSC message with the /Aloat address pattern

OSCSendFloat
---This function allows you send a single float over OSC with the address pattern you specify

======
Notes:
This first version of UE4OSC is bare-bones.  I stripped it of much of the functionality for my own project
in an attempt to make it more open ended and adaptable.  Please feel free to edit it however you see fit for your own work
or contact me to request functionality, which I will add as I have time.  duerzr <at> vcu <dot> edu

======
To Do:
-Close the thread created by Handle in StartOSCReceiver so UE4 doesn't crash when a level is played more than once
-If possible, make the function that sends OSC messages not type-specific.  The user just sends a message of any type-specific
and the backend takes care of the rest
-Let the user specify what address patterns to listen for when receiving messages

======
Copyright and Permissions
Copyright (c) 2014 Zach Duer (duerzr <at> vcu <dot> edu)

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files
(the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the Software,
and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

The text above constitutes the entire UE4OSC license; however, 
the UE4OSC developer(s) also make the following non-binding requests:

Any person wishing to distribute modifications to the Software is
requested to send the modifications to the original developer so that
they can be incorporated into the canonical version. It is also 
requested that these non-binding requests be included whenever the
above license is reproduced.

======
Credits:
Thanks to Robert K Hamilton for creating and sharing UDKOSC, the framework from which UE4OSC is built
https://ccrma.stanford.edu/wiki/UDKOSC

Thanks to these tutorials on UE4 plugins:

https://wiki.unrealengine.com/Linking_Dlls - ZkarmaKun

https://wiki.unrealengine.com/An_Introduction_to_UE4_Plugins - Bob Chatman

Thanks to Rama for many UE4 tutorials