var wpi = require('wiringpi-sx');

wpi.setup('wpi');

var pin = 25;

// wpi.pinMode(pin, wpi.OUTPUT);

var value = 1;

setInterval(function () {
    // wpi.digitalWrite(pin, value);
    value = +!value;
}, 500);

