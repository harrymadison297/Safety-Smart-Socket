import MQTTSubscribeService from "../services/subscribe.mqtt.service.js"

class MqttSubscribeController {
    /* MAC topic */
    mac = async (topic, message) => {
        console.log(`[MQTT] :: ${topic} :: ${message}`)
        return await MQTTSubscribeService.mac(topic, message)
    }

    /* Join topic */
    join = async (topic, message) => {
        console.log(`[MQTT] :: ${topic} :: ${message}`)
        return await MQTTSubscribeService.joinrequest(message)
    }
}

export default new MqttSubscribeController