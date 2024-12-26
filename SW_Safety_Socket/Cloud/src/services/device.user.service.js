import DeviceModel from "../database/schema/device.model.js";
import ErrorResponse from "../helpers/error.response.js";

class DeviceUserService {

  all = async (req) => {
    try {
        const userID = await req.get("CLIENT_ID");
        const allDevices = await DeviceModel.find({ownUser: userID})
        return allDevices
    } catch (error) {
        throw new ErrorResponse("Couldn't find this data from server", 500);
    }
  };

  findbymac = async ({mac}) => {
    try {
        const findedDevice = await DeviceModel.findOne({ mac: mac }).lean({});
        if (!findedDevice) {
          throw ErrorResponse("Couldn't find this device", 400);
        }
        return {
          id: findedDevice._id,
          devicePublic: findedDevice.devicePublic
        }
    } catch (error) {
        throw new ErrorResponse("Couldn't find this data from server", 500);
    }
  };

}

export default new DeviceUserService();
