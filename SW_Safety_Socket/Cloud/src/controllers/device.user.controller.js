import deviceUserService from "../services/device.user.service.js"

class UserDeviceController {
    /* Show all devices */
    all = async (req, res, next) => {
        console.log("[GET] :: all devices ::", req.body)
        return res.status(200).json(await deviceUserService.all(req))
    }

    /* Show one device */
    all = async (req, res, next) => {
        console.log("[GET] :: one devices ::", req.body)
        return res.status(200).json(await deviceUserService.one(req))
    }

    /* Find a device by MAC */
    findbymac = async (req, res, next) => {
        console.log("[POST] :: find device by mac ::", req.body)
        return res.status(200).json(await deviceUserService.findbymac(req.body))
    }

    /* Claim new device */
    claimDevice = async (req, res, next) => {
        console.log("[POST] :: claim new device ::", req.body)
        return res.status(200).json(await deviceUserService.claimDevice(req))
    }

    /* Set meshID for device */
    setmesh = async (req, res, next) => {
        console.log("[POST] :: set mesh network for device ::", req.body)
        return res.status(200).json(await deviceUserService.setMesh(req))
    }

    /* Show device history */
    history = async (req, res, next) => {
        console.log("[GET] :: Show device history ::", req.body)
        return res.status(200).json(await deviceUserService.history(req))
    }
}

export default new UserDeviceController