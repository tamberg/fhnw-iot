const gpio = require("rpi-gpio");

const pin = 29; // BCM pin 5, D5
let state = false;

gpio.setup(pin, gpio.DIR_OUT, () => {
  setInterval(() => {
    state = !state;
    gpio.write(pin, state, (err) => {
      console.log(err);
    });
  }, 500); // ms
});
