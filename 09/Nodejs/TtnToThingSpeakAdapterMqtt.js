const mqtt = require('mqtt')
const ttn = require('ttn');

const ttnAppId = 'TTN_APP_ID';
const ttnAccessKey = 'TTN_ACCESS_KEY';

const tsBroker = "mqtt://mqtt.thingspeak.com/";
const tsWriteApiKeys = {
  "TTN_DEV_ID_1": ["WRITE_API_KEY_1", "CHANNEL_ID_1"],
  "TTN_DEV_ID_2": ["WRITE_API_KEY_2", "CHANNEL_ID_2"],
  "TTN_DEV_ID_3": ["WRITE_API_KEY_3", "CHANNEL_ID_3"]
};

const ttnClient = new ttn.Client('eu', ttnAppId, ttnAccessKey);

ttnClient.on('message', function (devId, msg) {
    const bytes = Buffer.from(msg.payload_raw, 'base64');
    const x = ((bytes[0] << 8) | bytes[1]) / 100.0;
    const y = ((bytes[2] << 8) | bytes[3]) / 100.0;
    const tsMsgData = qs.stringify({
      "api_key": writeApiKeys[msg.dev_id][0],
      "field1": x,
      "field2": y
    });

  const tsClient = mqtt.connect(tsBroker);

  tsClient.on("connect", () => { // TODO: refactor
    const topic =
      "channels/" + writeApiKeys[msg.dev_id][1] + 
      "/publish/" + writeApiKeys[msg.dev_id][0];
    const message = tsMsgData;
    tsClient.publish(topic, message);
  });
});
