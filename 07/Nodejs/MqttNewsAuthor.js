const mqtt = require("mqtt");

const broker = "mqtt://test.mosquitto.org/";
const client = mqtt.connect(broker);

client.on("connect", () => {
  client.subscribe("comments");
  client.publish("news", "Sun too bright");
});

client.on("message", (topic, message) => {
  console.log(topic + ": " + message.toString());
});
