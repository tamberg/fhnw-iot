const mqtt = require("mqtt");

const broker = "mqtt://test.mosquitto.org/";
const client = mqtt.connect(broker);

client.on("connect", () => {
  client.subscribe("news");
});

client.on("message", (topic, message) => {
  console.log(topic + ": " + message.toString());
  client.publish("comments", "Turn it off");
});
