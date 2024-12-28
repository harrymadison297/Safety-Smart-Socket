import Express from "express";
import instanceMqtt from "../mqtt/init.mqtt.js";
import mqttSubscribeController from "../controllers/mqtt.subscribe.controller.js";

const MQTTSubscribeApp = Express();

/**
 * Init MQTT instance
 */
instanceMqtt;

/**
 * Init MQTT Subscribe
 */
instanceMqtt.mqttClient.subscribe("/mac/+", 1);
instanceMqtt.mqttClient.subscribe("/join_request", 1);

/**
 * Init MQTT Message Route
 */
instanceMqtt.mqttClient.on("message", async (topic, message, packet) => {
    switch (topic.split("/")[1])
    {
        case "mac":
            mqttSubscribeController.mac(topic, message);
            break;
        case "join_request":
            mqttSubscribeController.join(topic, message);
            break;
    }
})

export default MQTTSubscribeApp;