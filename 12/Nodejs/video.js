const RaspiCam = require("raspicam");

const cam = new RaspiCam({
    mode: "video",
    timeout: 3000, // ms
    output: "./video.h264"
});

cam.start();
