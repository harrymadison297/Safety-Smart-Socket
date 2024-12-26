import DeviceModel from "../database/schema/device.model.js";
import MeshModel from "../database/schema/mesh.model.js";
import ErrorResponse from "../helpers/error.response.js";

class DeviceUserService {

  all = async (req) => {
    try {
      const userID = await req.get("CLIENT_ID");
      const allDevices = await DeviceModel.find({ ownUser: userID })
      return allDevices
    } catch (error) {
      throw new ErrorResponse("Couldn't find this data from server", 500);
    }
  };

  findbymac = async ({ mac }) => {
    try {
      const findedDevice = await DeviceModel.findOne({ mac: mac }).lean({});
      if (!findedDevice) {
        throw ErrorResponse("Couldn't find this device", 400);
      }
      return {
        devicePublic: findedDevice.devicePublic
      }
    } catch (error) {
      throw new ErrorResponse("Couldn't find this data from server", 500);
    }
  };

  claimDevice = async ({ mac, secret }) => {
    try {
      const findedDevice = await DeviceModel.findOne({ mac: mac }).lean({});
      if (!findedDevice) {
        throw ErrorResponse("Couldn't find this device", 400);
      }

      if (findedDevice.deviceSecret != secret) {
        throw ErrorResponse("Fail to this claim this device, wrong secret", 404);
      }

      return {
        id: findedDevice._id
      }
    } catch (error) {
      throw new ErrorResponse("Couldn't find this data from server", 500);
    }
  };

  setMesh = async ({ name, meshid, mac }) => {
    try {
      let mesh = null;
      const findedMesh = await MeshModel.findOne({ meshid: meshid }).lean({});

      if (!findedMesh) {
        mesh = await MeshModel.create({
          user: await req.get("CLIENT_ID"),
          meshid: meshid,
          name: name,
          meshpass: Math.floor((Math.random() * 900000) + 100000)
        })
      }

      if (findedMesh) {
        if (findedMesh.name != name) {
          throw ErrorResponse("Wrong mesh info", 400);
        }

        mesh = findedMesh._id
      }

      const deviceUpdate = await DeviceModel.findOneAndUpdate(
        {
          mac: mac
        },
        {
          name: name,
          meshNetwork: mesh._id
        }
      )

      if (!deviceUpdate) {
        throw ErrorResponse("Couldn't set mesh", 500);
      }

      return {
        device: deviceUpdate._id
      }
    } catch (error) {
      throw new ErrorResponse("Couldn't find this data from server", 500);
    }
  };
}

export default new DeviceUserService();
