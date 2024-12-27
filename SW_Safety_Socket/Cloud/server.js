import { configDotenv } from "dotenv";
import manufactorApp from "./src/applications/manufactor.app.js"
import userApp from "./src/applications/user.app.js";
import MQTTPublishApp from "./src/applications/mqtt.publish.app.js";
import MQTTSubscribeApp from "./src/applications/mqtt.subscribe.app.js";

/* Use variable from .env */
configDotenv();

/* Set port for service application */
const usr_port = process.env.APP_USER_PORT || 3000
const pub_port = process.env.MQTT_PUB_PORT || 4000
const sub_port = process.env.MQTT_SUB_PORT || 5000
const man_port = process.env.APP_MANF_PORT || 6000

/* Open port listener */
userApp.listen(usr_port, () => {
    console.info("User application listening on port", usr_port);
})

MQTTPublishApp.listen(pub_port, () => {
    console.info("MQTT publish application listening on port", pub_port);
})

MQTTSubscribeApp.listen(sub_port, () => {
    console.info("MQTT subscribe application listening on port", sub_port);
})

manufactorApp.listen(man_port, () => {
    console.info("Manufactor application listening on port", man_port);
})