qstreamsuite
============

Like VNC/RDP/Splashtop, only worse.

What is the goal of this project?
============
To create a modular base on which it is possible to send one computer's input to another while being being able to observe the display, all remotely over a network connection.
This is facilitated by a connection over TCP, where the input and other data is transferred.
Being server-based, a client can connect to a server and provide it with input specified in a manner of types and values. These types and values hold no significance unto themselves before they are used with an input handler on the server side. The client may acquire such input from a plugin of its own, which may read joystick input, mouse input, keyboard input and forward this.
The server, with the help of plugins, will then be able to interpret the input it receives over TCP and set it into motion. For instance, it may emulate mouse movement, scrolling, key presses and joystick movement on the server side.
In its standard condition the server is not able to do anything of the sort, and thus relies on plugins to act upon the signals. These plugins register for different "types" of input, identified by integers, in order to filter them. These plugins may even send input signals between themselves, making way for remapping of input.
On the side of forwarding input, a second TCP connection is used to forward an audio/video stream. This also relies on plugins in order to capture and encode the input (in separate instances, such that encoders are kept platform-independent) from the server, which is followingly sent to the client for viewing. It would be preferrable to have a low-latency stream, and as such codecs meant for low latency connectivity are to be preferred, however none of this is mandatory (yet).

What is the *current* condition of this project?
============
 - The client is in need of a refactor in order to better suit the changes which have been made incrementally. Incremental development has necessitated this, as a lot of the code was written to work for a different purpose.
 - The client is mostly platform-indepedent, confirmed to run on both Android and desktop Linux. Results showed low input lag in numerous tests where a Wi-Fi device connected to a LAN-connected device, with minor hiccups, but in general showing good progress, if not completion.
 - Client uses a QML interface to remain native-looking on most platforms. It is not 100% native-looking, but good for mobile platforms.
 - The client is in dire need of a code cleanup on the QML side. Core functionality is present but in disarray.
 - Client supports plugins by placing a file "plugins.json" in the working directory with a JSON array of file names for libraries. Confirmed to be working by an example of an SDL2 joystick implementation.
 - Client is capable of receiving and transmitting plugin events, however I would like to add mouse grabbing.
 - The server has no platform-bound code, but is yet to be tested on anything other than Linux. At this writing moment there are is only an X11 input handler.
 - Plugin support on the server is finished in terms of input handling, nothing to be done.
 - Client-side and server-side plugin interfaces for input emission and handling are mostly done and ready to be implemented for different platforms. An X11 input handler is already complete, a joystick input plugin is complete, and a joystick remapper is underway.
