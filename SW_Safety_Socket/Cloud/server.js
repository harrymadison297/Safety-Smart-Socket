import { configDotenv } from "dotenv";
import process from "process";
import instanceMongoDB from "./src/databases/mongodb.js";
import DeviceInputService from "./src/services/DeviceInputService.js";

// // init environment *************************************
configDotenv();

// init database ****************************************
instanceMongoDB; // Connect MongoDB

const deviceInputServicePort = process.env.DEVICE_IN_PORT || 3001;
const deviceOutputServicePort = process.env.DEVICE_OUT_PORT || 3002;
const controlServicePort = process.env.CONTROL_PORT || 3003;
const applicationServicePort = process.env.APPLICATION_PORT || 3004;

const DeviceInputListener = DeviceInputService.listen(deviceInputServicePort, () => {
  console.log("Listening on port ", deviceInputServicePort);
});

const DeviceOutputListener = 

process.on("SIGINT", () => {
  DeviceInputListener.close(() => console.log("Device input service exit!"));
});
