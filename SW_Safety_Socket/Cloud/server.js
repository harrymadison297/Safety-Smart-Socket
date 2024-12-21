import { configDotenv } from "dotenv";
import manufactorApp from "./src/applications/manufactor.app.js"
import userApp from "./src/applications/user.app.js";

/* Use variable from .env */
configDotenv();

/* Set port for service application */
const usr_port = process.env.APP_USER_PORT || 3000
const fol_port = process.env.FOLLOW_DEVICE_PORT || 4000
const ctl_port = process.env.CONTROL_DEVICE_PORT || 5000
const man_port = process.env.APP_MANF_PORT || 6000

/* Open port listener */
userApp.listen(usr_port, () => {
    console.info("User application listening on port", usr_port);
})

manufactorApp.listen(man_port, () => {
    console.info("Manufactor application listening on port", man_port);
})