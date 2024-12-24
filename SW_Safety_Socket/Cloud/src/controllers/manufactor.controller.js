import ManufactorService from "../services/manufactor.service.js";

class ManufactorController {
  // Sign up new device ID controller
  signupDeviceInfo = async (req, res, next) => {
    console.log("[POST] :: Sign up device Infor :: ", req.body);
    return res.status(201).json(await ManufactorService.signupDevice(req.body));
  };

  // Delete device by ID controller
  deleteDevice = async (req, res, next) => {
    console.log("[POST] :: Device ID ::", req.body);
    return res.status(200).json(await ManufactorService.deleteDevice(req.body));
  };

  // Get device info by ID controller
  getDeviceInfo = async (req, res, next) => {
    console.log("[POST] :: Device Info ID :: ", req.body);
    res.status(200).json(await ManufactorService.getDeviceInfo(req.body));
  };

  // Get all devices controller
  getAllDevices = async (req, res, next) => {
    console.log("[GET] :: ALL Device Info :: ", req.body);
    return res.status(200).json(await ManufactorService.getAllDeviceInfo());
  };
}

export default new ManufactorController();
