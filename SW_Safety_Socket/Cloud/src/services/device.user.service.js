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

  

}

export default new DeviceUserService();
