# wiringpi-sx

## Install

```
npm install wiringpi-sx
```

## Usage

### Javascript

```javascript
var wpi = require('wiring-pi');
```

### Typescript

```typescript
use * as wpi from 'wiringpi-sx';
```

## Documentation

See the [DOCUMENTATION.md](DOCUMENTATION.md) file to find some more detailed documentation, but hold in mind, that only basics are implemented at the moment.

**See the [index.d.ts](index.d.ts) file to see in detail which functions and constants are implemented at the moment.**

## Overview

This package implements the binding to the native [wiringPi library] for Node.js on Raspberry Pi. 

This project is a further development of the existing packages [wiringpi-node][npm-wiringpi-node] and [wiringpi][npm-wiringpi]. In contrast to these projects, this package is using the [Node.js N-API][nodejs-n-api] interface to integrate the native wiringPi library into the Node.js virtual machine.

>>*The N-API interface is independent from the underlying JavaScript runtime (ex V8) and is maintained as part of Node.js itself. This API will be Application Binary Interface (ABI) stable across versions of Node.js. It is intended to insulate Addons from changes in the underlying JavaScript engine and allow modules compiled for one version to run on later versions of Node.js without recompilation.*

At the moment only basic functions are implemented:

* Digital pin mode
* Digital pin read and write operation
* SPI interface
* UART interface

The usage of macros is avoided to allow simple straightforward analysis of code. Also the failure handling is improved by:

* the usage of exceptions.
* the usage of file names and line numbers in error messages.
* checking all native return values consequently.

Also some files of the orignal wiringPi library are modified in order to:

* allow acces to error messages from the linux system (`strerror()`)
* avoiding an exit call in case of an error, which would lead to a break down of the Node.js process. Instead an error break down, an javascript error is thrown which could be catched and handled inside javascript / typescript.

The type definitions inside the file [index.d.ts][index.d.ts] allow the usage of Typescript as programming language.


## Building the project

In case of install fails or change of code you can rebuild this project.

### Building the native wiringPi library

Make sure that all C++ building tools are available (`apt install build-essential`).

1) Change to the cloned project (`git clone ...`) or the installed package under the subdirectory path *node_modules*.
2) Change to subdirectory *wiringPi/wiringPi* with `cd wiringPi/wiringPi`
3) Clean the project with `make clean`  
4) Rebuild the project with `make static`  

As result the binary library file *libwiringPi.a* is available in the subdirectory *wiringPi/wiringPi*. This file is statically included into the binary Node.js module file *wiring_pi_sx.node* in the subdirectory *build/Release*.

For further information see [http://wiringpi.com/download-and-install](http://wiringpi.com/download-and-install). Hold in mind that the wiringPi source files differ from the original wiringPi sources!

### Building the binary Node.js module file

The source files are located in the subdirctory *src*. For building the npm package module [node-gyp][npm-node-gyp] is needed. Normally it is installed globally with the command `sudo npm install -g node-gyp` before building C++ Add-Ons.

The following commands can be used to build the native Node.js module file:

* `node-gyp configure`  
Create the needed files, using the definitions in the file *binding.gyp*.
* `node-gyp rebuild`  
Use this command when the native wiringPi library file *wiringPi/wiringPi/libwiringPi.a* has changed.
* `node-gyp build`  
Use this command when source code in subdirctory *src* was changed.

For further information see [https://nodejs.org/api/addons.html#addons_building](https://nodejs.org/api/addons.html#addons_building).


[nodejs-n-api]: https://nodejs.org/api/n-api.html
[wiringPi library]: http://wiringpi.com/
[npm-wiringpi-node]: https://www.npmjs.com/package/wiringpi-node
[npm-wiringpi]: https://www.npmjs.com/package/wiring-pi
[npm-node-gyp]: https://www.npmjs.com/package/node-gyp
