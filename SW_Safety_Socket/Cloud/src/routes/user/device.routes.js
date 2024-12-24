import express from "express"
import asyncMiddlewareHandler from "../../services/middleware.service.js"
import UserDeviceController from "../../controllers/device.user.controller.js"

const userDeviceRouter = express.Router()

userDeviceRouter
    .get('/all', asyncMiddlewareHandler(UserDeviceController.all))  // Get all devices
 
export default userDeviceRouter