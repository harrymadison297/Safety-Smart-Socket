import { mqttClient } from "../init.mqtt.js";

const publishTopic = (topic, qos, message) => {
  mqttClient.publish(topic, message, {
    qos: qos,
    retain: false,
  });
};

export default publishTopic;
