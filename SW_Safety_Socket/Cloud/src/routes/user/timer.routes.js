import express from "express"
import asyncMiddlewareHandler from "../../services/middleware/middleware.service.js"
import timerUserController from "../../controllers/timer.user.controller.js"

const userTimerRouter = express.Router()

userTimerRouter
    .get('/all', asyncMiddlewareHandler(timerUserController.all))
    .post('/new', asyncMiddlewareHandler(timerUserController.new))
    .delete('/delete', asyncMiddlewareHandler(timerUserController.delete))

export default userTimerRouter