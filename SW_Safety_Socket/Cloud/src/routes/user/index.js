import express from "express"

import asyncMiddlewareHandler from "../../services/middleware.service.js"
import { AuthenticationHeader } from "../../services/middleware/header.service.js"

import userAuthRouter from "./auth.routes.js"
import userDeviceRouter from "./device.routes.js"

const userRouter = express.Router()

userRouter
    .use('/auth', userAuthRouter)
    .use(asyncMiddlewareHandler(AuthenticationHeader))
    .use('/device', userDeviceRouter)
 
export default userRouter