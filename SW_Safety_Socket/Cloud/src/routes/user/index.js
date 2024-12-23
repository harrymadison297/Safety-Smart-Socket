import express from "express"
import userAuthRouter from "./auth.routes.js"
import asyncMiddlewareHandler from "../../services/middleware/middleware.service.js"
import { AuthenticationHeader } from "../../services/middleware/header.service.js"

const userRouter = express.Router()

userRouter
    .use('/auth', userAuthRouter)
    .use(asyncMiddlewareHandler(AuthenticationHeader))
    
 
export default userRouter