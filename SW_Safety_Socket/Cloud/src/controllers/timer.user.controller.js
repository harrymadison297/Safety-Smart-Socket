import timerUserService from "../services/timer.user.service.js"

class UserTimerController {
    /* Show all timer */
    all = async (req, res, next) => {
        console.log("[GET] :: all timer ::", req.body)
        return res.status(200).json(await timerUserService.all(req))
    }

    /* Show all timer */
    new = async (req, res, next) => {
        console.log("[POST] :: new timer ::", req.body)
        return res.status(200).json(await timerUserService.new(req))
    }

    /* Show all timer */
    delete = async (req, res, next) => {
        console.log("[DELETE] :: delete timer ::", req.body)
        return res.status(200).json(await timerUserService.delete(req.body))
    }
}

export default new UserTimerController