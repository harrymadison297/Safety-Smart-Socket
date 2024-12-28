import express from "express"
import asyncMiddlewareHandler from "../../services/middleware/middleware.service.js"
import UserAuthController from "../../controllers/auth.user.controller.js"

const userAuthRouter = express.Router()

userAuthRouter
    .post('/signup', asyncMiddlewareHandler(UserAuthController.signup))  // Sign up new user
    .post('/login', asyncMiddlewareHandler(UserAuthController.login))    // Login user
 
export default userAuthRouter