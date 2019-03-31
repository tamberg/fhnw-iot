const mqtt = require("mqtt");

const broker = "mqtt://test.mosquitto.org/";
const client = mqtt.connect(broker);

client.on("connect", () => {
  const topic = "hello";
  client.subscribe(topic);
});

client.on("message", (topic, message) => {
  console.log(message.toString());
});
