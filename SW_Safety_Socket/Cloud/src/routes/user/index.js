import express from "express"

import asyncMiddlewareHandler from "../../services/middleware/middleware.service.js"
import { AuthenticationHeader } from "../../services/middleware/header.service.js"

import userAuthRouter from "./auth.routes.js"
import userDeviceRouter from "./device.routes.js"
import userTimerRouter from "./timer.routes.js"

const userRouter = express.Router()

userRouter
    .use('/auth', userAuthRouter)
    .use(asyncMiddlewareHandler(AuthenticationHeader))
    .use('/device', userDeviceRouter)
    .use('/timer', userTimerRouter)
 
export default userRouter