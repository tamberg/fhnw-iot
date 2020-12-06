// $ sudo raspi-config > Interface options > Camera > Enable
// $ npm install raspicam

const RaspiCam = require("raspicam");

const cam = new RaspiCam({
    mode: "photo",
    output: "./photo.jpg"
});

cam.start();
