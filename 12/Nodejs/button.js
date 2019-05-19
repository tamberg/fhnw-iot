const gpio = require("rpi-gpio");

const pin = 16;

gpio.setMode(gpio.MODE_BCM);
gpio.setup(pin, gpio.DIR_IN, gpio.EDGE_BOTH);

gpio.on("change", (pin, value) => {
  console.log("pin " + pin + ": " + value);
});

console.log("watching pin " + pin + "...");
