import MQTTPublishService from "../services/publish.mqtt.service.js"

class MqttPublishController {
    /* Timer routine */
    timer = async () => {
        console.log(`[ROUTINE] :: Timer :: Initial`)
        return await MQTTPublishService.timer()
    }

    /* Warning routine */
    warning = async () => {
    }
}

export default new MqttPublishController