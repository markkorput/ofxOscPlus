# ofxOscPlus
Extension to the ofxOsc addon that adds functionality for improved remote parameter controls

## Example
### Server
Serves parameter layout and sends param value changes to registered clients

``` c++
// header
ofxOscPlus::ParameterServer server; 
ofParameterGroup paramGroup;
// setup
server.setup(paramGroup);
// update
server.update();
```

### Client
Registers with server and receives parameter layout and value updates

``` c++
// header
ofxOscPlus::ParameterClient client;
ofParameterGroup paramGroup;
// setup
client.setup(paramGroup, port); // port parameter is optional
// update
client.update();
```
