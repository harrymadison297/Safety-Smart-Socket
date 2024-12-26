import deviceUserService from "../services/device.user.service.js"

class UserDeviceController {
    /* Show all devices */
    all = async (req, res, next) => {
        console.log("[GET] :: all devices ::", req.body)
        return res.status(200).json(await deviceUserService.all(req))
    }

    /* Find a device by MAC */
    findbymac = async (req, res, next) => {
        console.log("[POST] :: find device by mac ::", req.body)
        return res.status(200).json(await deviceUserService.findbymac(req.body))
    }
}

export default new UserDeviceController