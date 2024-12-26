import DeviceModel from "../database/schema/device.model.js";
import ErrorResponse from "../helpers/error.response.js";

class ManufactorService {
  signupDevice = async ({ name, mac, public, secret }) => {
    if (!name || !mac) {
      throw new ErrorResponse("Wrong param", 404);
    }
    
    // Check device exit?
    const holderDevice = await DeviceModel.findOne({ mac: mac }).lean({});
    if (holderDevice) {
      throw new ErrorResponse("Device exited!", 403);
    }

    const newDevice = await DeviceModel.create({
      name: name,
      mac: mac,
      devicePublic: public,
      deviceSecret: secret
    });
    if (!newDevice) {
      throw new ErrorResponse("can't not sign up for this device", 500);
    }

    return newDevice;
  };

  getAllDeviceInfo = async () => {
    const devices = await DeviceModel.find();
    if (!devices) {
      throw new ErrorResponse("Can't find any device in your location", 404);
    }

    return devices;
  };

  getDeviceInfo = async ({ id }) => {
    const deviceFound = await DeviceModel.findById(id);

    if (!deviceFound) {
      throw new ErrorResponse("Device Not Found", 403);
    }

    return deviceFound;
  };

  deleteDevice = async ({ id }) => {
    const deviceExist = await DeviceModel.findById(id);
    if (!deviceExist) {
      throw new ErrorResponse("Not exist this device, check again", 404);
    }

    const deviceWillDelete = await DeviceModel.findByIdAndDelete(id);
    if (!deviceWillDelete) {
      throw new Error("Can't delete this device", 404);
    }

    return deviceWillDelete;
  };
}

export default new ManufactorService();
