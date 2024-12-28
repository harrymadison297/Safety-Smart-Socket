import HistoryModel from "../database/schema/history.model.js"
import DeviceModel from "../database/schema/device.model.js";
import TimerModel from "../database/schema/timer.rule.model.js";

class MQTTSubscribeService {
    mac = async (topic, message) => {
        try {
            const mac = topic.split("/")[2];
            const data = JSON.parse(message)

            if (mac != data?.mac) {
                throw new Error("Wrong data!");
            }

            const device = await DeviceModel.findOne({ mac: mac }).lean({});
            if (!device) {
                throw new Error("Device not exited!");
            }

            /* New History Record */
            const newHistory = HistoryModel.create({
                device: device._id,
                value: data
            })

            /* Complete timer */
            const current = new Date().getTime();
            const TimerDone = TimerModel.updateMany(
                {
                    mac: mac,
                    timeset: { $lte: current },
                    timedone: 0,
                    state: data?.state == 1 ? true : false,
                    active: true
                },
                {
                    timedone: current,
                    active: false
                }
            )
        } catch (error) {
            console.log(error)
        }
    }

    joinrequest = async (message) => {
        console.log(message)
    }
}

export default new MQTTSubscribeService();
