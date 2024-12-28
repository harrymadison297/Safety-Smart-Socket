import HistoryModel from "../database/schema/history.model.js"
import DeviceModel from "../database/schema/device.model.js";

class MQTTSubscribeService {
    mac = async (topic, message) => {
        try {
            const mac = topic.split("/")[2];
            const data = JSON.parse(message)

            if (!data) {
                throw new Error("Wrong data!");
            }
            
            if (mac != data?.mac) {
                throw new Error("Wrong mac data!");
            }

            const device = await DeviceModel.findOne({ mac: mac }).lean({});
            if (!device) {
                throw new Error("Device not exited!");
            }

            const newHistory = HistoryModel.create({
                device: device._id,
                value: data
            })

            if (!newHistory) {
                throw new Error("Couldn't create history!");
            }
        } catch (error) {
            console.log(error)
        }
    }

    joinrequest = async (message) => {
        console.log(message)
    }
}

export default new MQTTSubscribeService();
