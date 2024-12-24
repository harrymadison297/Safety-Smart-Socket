import AuthUserService from "../services/auth.user.service.js"

class UserAuthController {
    /* Sign up new user */
    signup = async (req, res, next) => {
        console.log("[POST] :: signup ::", req.body)
        return res.status(201).json(await AuthUserService.signup(req.body))
    }

    /* Login existing user */
    login = async (req, res, next) => {
        console.log("[POST] :: login ::", req.body)
        return res.status(200).json(await AuthUserService.login(req.body))
    }
}

export default new UserAuthController