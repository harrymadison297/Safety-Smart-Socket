import express from "express"
import userAuthRouter from "./auth.routes.js"

const userRouter = express.Router()

userRouter
    .use('/auth', userAuthRouter)
 
export default userRouter