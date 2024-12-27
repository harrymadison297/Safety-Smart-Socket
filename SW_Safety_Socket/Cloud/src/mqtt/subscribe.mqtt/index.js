import { mqttClient } from "../init.mqtt.js";

const subscribeTopic = (topic, qos) => {
  console.log(`Subscribe to Topic : ${topic}`);
  mqttClient.subscribe(topic, qos);
};

export { subscribeTopic };
