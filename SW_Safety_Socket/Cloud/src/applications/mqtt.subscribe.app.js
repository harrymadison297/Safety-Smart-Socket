import Express from "express";
import instanceMqtt from "../mqtt/init.mqtt.js";
import mqtt from "mqtt";

const MQTTSubscribeApp = Express();

/**
 * Init MQTT instance
 */
instanceMqtt;


export default MQTTSubscribeApp;