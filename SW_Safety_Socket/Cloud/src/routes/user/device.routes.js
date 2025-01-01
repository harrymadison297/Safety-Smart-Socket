import express from "express"
import asyncMiddlewareHandler from "../../services/middleware/middleware.service.js"
import UserDeviceController from "../../controllers/device.user.controller.js"

const userDeviceRouter = express.Router()

userDeviceRouter
    .get('/all', asyncMiddlewareHandler(UserDeviceController.all))                                      // Get all devices
    .get('/id/:id', asyncMiddlewareHandler(UserDeviceController.one))                                   // Get device by id
    .post('/findbymac', asyncMiddlewareHandler(UserDeviceController.findbymac))                         // Find device by mac
    .post('/claim', asyncMiddlewareHandler(UserDeviceController.claimDevice))                           // Claim new device
    .post('/setmesh', asyncMiddlewareHandler(UserDeviceController.setmesh))                             // Claim new device
    .get('/history/:mac/limit/:limit', asyncMiddlewareHandler(UserDeviceController.history))            // Get device history

export default userDeviceRouter