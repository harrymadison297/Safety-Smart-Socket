import deviceUserService from "../services/device.user.service.js"

class UserDeviceController {
    /* Show all devices */
    all = async (req, res, next) => {
        console.log("[GET] :: all devices ::", req.body)
        return res.status(200).json(await deviceUserService.all(req))
    }
}

export default new UserDeviceController