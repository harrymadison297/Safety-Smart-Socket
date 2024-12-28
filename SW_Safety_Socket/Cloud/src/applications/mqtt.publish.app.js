import Express from "express";
import instanceMqtt from "../mqtt/init.mqtt.js";
import mqttPublishController from "../controllers/mqtt.publish.controller.js";

const MQTTPublishApp = Express();

/**
 * Init MQTT instance
 */
instanceMqtt;

/**
 * MQTT Publish Timer Routine
 */
mqttPublishController.timer()

/**
 * MQTT Warning timer
 */
mqttPublishController.warning()

export default MQTTPublishApp;