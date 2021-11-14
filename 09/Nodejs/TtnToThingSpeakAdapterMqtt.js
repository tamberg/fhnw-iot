const mqtt = require('mqtt'); // npm install mqtt
const ttn = require('ttn'); // npm install ttn

//const ttnAppId = 'TTN_APP_ID';
const ttnAppId = 'fhnw-iot';
const ttnAccessKey = 'TTN_ACCESS_KEY';

const tsBroker = "mqtt://mqtt.thingspeak.com/";
const ttnBroker = "mqtt://eu.thethings.network/";
const ttnDevices = {
  "TTN_DEV_ID_1": { tsWriteApiKey: "WRITE_API_KEY_1", tsChannelId: "CHANNEL_ID_1" },
  "TTN_DEV_ID_2": { tsWriteApiKey: "WRITE_API_KEY_2", tsChannelId: "CHANNEL_ID_2" },
  "TTN_DEV_ID_3": { tsWriteApiKey: "WRITE_API_KEY_3", tsChannelId: "CHANNEL_ID_3" },
};

const tsClient = mqtt.connect(tsBroker);
const ttnClient = mqtt.connect(ttnBroker);

ttnClient.on("connect", () => { 
  console.log("TTN client connected.");
  for (let i = 0; i < ttnDevices.length; i++) {
    const ttnTopic = ttnAppId + "/devices/" + ttnDevices[i] + "/up"
    ttnClient.subscribe(ttnTopic);
  }
});

tsClient.on("connect", () => {
  console.log("ThingSpeak client connected.");
});

ttnClient.on("message", (topic, message) => {
  console.log(message);
  const bytes = Buffer.from(msg.data.uplink_message.frm_payload, 'base64');
  const x = ((bytes[0] << 8) | bytes[1]) / 100.0;
  const y = ((bytes[2] << 8) | bytes[3]) / 100.0;
  
  const tsMsgData = qs.stringify({
    "api_key": ttnDevices[msg.data.end_device_ids.device_id].tsWriteApiKey,
    "field1": x,
    "field2": y
  });
  const tsTopic =
    "channels/" + ttnDevices[msg.data.end_device_ids.device_id].tsChannelId + 
    "/publish/" + ttnDevices[msg.data.end_device_ids.device_id].tsWriteApiKey;
  tsClient.publish(tsTopic, tsMsgData);
});
