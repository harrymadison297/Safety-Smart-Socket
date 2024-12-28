import mqtt from "mqtt";
import dotenv from "dotenv";
dotenv.config();

const mqtt_broker = process.env.MQTT_BROKER
const mqtt_options = {
  keepalive: 60,
  protocolID: "MQTT",
  protocolVersion: 4,
  clean: true,
  reconnectPeriod: 1000,
  connectTomeout: 30 * 1000,
};

/* Singleton design pattern class to connect to MQTT */
class MqttConnection {
  constructor() {
    this.mqttClient = mqtt.connect(mqtt_broker, mqtt_options)
    this.mqttClient.on("connect", () => {
      console.log("Connected to MQTT");
    });
    this.mqttClient.on("error", (error) => {
      console.log("MQTT Connection Error: " + error.message);
    });
  }
}

const instanceMqtt = new MqttConnection()

export default instanceMqtt