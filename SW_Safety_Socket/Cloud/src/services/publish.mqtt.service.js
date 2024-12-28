import TimerModel from "../database/schema/timer.rule.model.js";
import instanceMqtt from "../mqtt/init.mqtt.js";

class MQTTPublishService {
    timer = async () => {
        setInterval(async () => {
            const alltimer = await TimerModel.find({
                active: true,
                timedone: 0,
                retry: { $gte: 1 }
            })
            
            if (!alltimer) {
                return;
            }
            
            const d = new Date();
            let current = d.getTime();

            alltimer.forEach(async (e) => {
                if (e.preset || e.timeset < current) {
                    instanceMqtt.mqttClient.publish(
                        "/mac/"+e.mac,
                        e.value
                    )
                    TimerModel.updateOne(
                        {
                            _id: e._id
                        },
                        {
                            preset: false,
                            retry: e.retry - 1
                        }
                    )
                }
            })
        }, 1000);
    }
}

export default new MQTTPublishService();
