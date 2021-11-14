const mqtt = require('mqtt'); // npm install mqtt

const ttnAppId = "TTN_APP_ID"; // TODO

const tsBroker = "mqtt://mqtt.thingspeak.com/";
const ttnBroker = "mqtt://eu1.cloud.thethings.network/";

const ttnBrokerOptions = {
  username: ttnAppId + "@ttn",
  password: "TTN_API_KEY" // TODO
}

const ttnDevices = {
  "TTN_DEV_ID_1": { tsWriteApiKey: "WRITE_API_KEY_1", tsChannelId: "CHANNEL_ID_1" }, // TODO
  "TTN_DEV_ID_2": { tsWriteApiKey: "WRITE_API_KEY_2", tsChannelId: "CHANNEL_ID_2" }, // TODO
  "TTN_DEV_ID_3": { tsWriteApiKey: "WRITE_API_KEY_3", tsChannelId: "CHANNEL_ID_3" }, // TODO
};

const tsClient = mqtt.connect(tsBroker);
const ttnClient = mqtt.connect(ttnBroker, ttnBrokerOptions);

tsClient.on("connect", () => {
  console.log("ThingSpeak client connected.");
});

ttnClient.on("connect", () => { 
  console.log("TTN client connected.");
  Object.keys(ttnDevices).forEach((deviceId) => {
    const ttnTopic = "v3/" + ttnAppId + "@ttn/devices/" + deviceId + "/up";
    console.log("TTN client SUB " + ttnTopic);
    ttnClient.subscribe(ttnTopic);
  });
});

ttnClient.on("message", (ttnTopic, ttnMessage) => {
  console.log("TTN message received, " + ttnTopic);
  const json = JSON.parse(ttnMessage);
  const deviceId = json.end_device_ids.device_id;
  const payload = json.uplink_message.frm_payload;
  if (payload != undefined) {
    const bytes = Buffer.from(payload, 'base64');
    const temp = ((bytes[0] << 8) | bytes[1]) / 100.0;
    const humi = ((bytes[2] << 8) | bytes[3]) / 100.0;

    const tsTopic =
      "channels/" + ttnDevices[deviceId].tsChannelId + 
      "/publish/" + ttnDevices[deviceId].tsWriteApiKey;
    const tsMessage = "field1=" + temp + "&field2=" + humi;
    tsClient.publish(tsTopic, tsMessage);
    console.log("ThingSpeak message published, " + tsTopic);
  } else {
    console.log("TTN message payload undefined.");
  }
});
