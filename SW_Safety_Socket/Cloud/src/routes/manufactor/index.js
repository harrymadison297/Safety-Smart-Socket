import express from "express"
import manufactorDeviceRouter from "./device.js"

const manufactorRouter = express.Router()

manufactorRouter
    .use('/device', manufactorDeviceRouter)
 
export default manufactorRouter