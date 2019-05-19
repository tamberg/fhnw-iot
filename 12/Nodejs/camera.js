const RaspiCam = require("raspicam");

const cam = new RaspiCam({
    mode: "photo",
    output: "./photo_%d"
});

cam.on("start", function(){
  console.log("start");
  //cam.stop();
});

cam.on("read", (err, timestamp, filename) => { console.log("read"); });
cam.on("stop", () => { console.log("stop"); });
cam.on("exit", () => { console.log("exit"); });

cam.start();
