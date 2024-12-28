import express from "express"
import asyncMiddlewareHandler from "../../services/middleware/middleware.service.js"
import manufactorController from "../../controllers/manufactor.controller.js"

const manufactorDeviceRouter = express.Router()

manufactorDeviceRouter
    .post('/signup', asyncMiddlewareHandler(manufactorController.signupDeviceInfo))
    .delete('/delete', asyncMiddlewareHandler(manufactorController.deleteDevice))
    .post('/one', asyncMiddlewareHandler(manufactorController.getDeviceInfo))
    .get('/', asyncMiddlewareHandler(manufactorController.getAllDevices))

export default manufactorDeviceRouter