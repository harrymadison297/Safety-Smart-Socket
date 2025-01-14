import DeviceModel from "../database/schema/device.model.js";
import HistoryModel from "../database/schema/history.model.js";
import MeshModel from "../database/schema/mesh.model.js";
import UserModel from "../database/schema/user.model.js";
import ErrorResponse from "../helpers/error.response.js";
import instanceMqtt from "../mqtt/init.mqtt.js";

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

  one = async (req) => {
    try {
      const userID = await req.get("CLIENT_ID");
      const { deviceID } = await req.params;
      const deviceInfo = await DeviceModel.findById(deviceID);
      return deviceInfo
    } catch (error) {
      throw new ErrorResponse("Couldn't find this data from server", 500);
    }
  };

  findbymac = async ({ mac }) => {
    try {
      const findedDevice = await DeviceModel.findOne({ mac: mac }).lean({});
      if (!findedDevice) {
        throw new ErrorResponse("Couldn't find this device", 400);
      }
      return {
        devicePublic: findedDevice.devicePublic
      }
    } catch (error) {
      throw new ErrorResponse("Couldn't find this data from server", 500);
    }
  };

  claimDevice = async (req) => {
    try {
      const { mac, secret } = req.body
      const findedDevice = await DeviceModel.findOne({ mac: mac }).lean({});
      if (!findedDevice) {
        throw new ErrorResponse("Couldn't find this device", 400);
      }

      if (findedDevice.deviceSecret != secret) {
        throw new ErrorResponse("Fail to this claim this device, wrong secret", 404);
      }

      const user = await UserModel.findById(await req.get("CLIENT_ID")).lean({});

      const deviceUpdate = await DeviceModel.findOneAndUpdate(
        {
          mac: mac
        },
        {
          ownUser: user._id,
          meshNetwork: null
        }
      )

      return {
        id: deviceUpdate._id,
        mac: deviceUpdate.mac,
        devicePublic: deviceUpdate.devicePublic,
        deviceSecret: deviceUpdate.deviceSecret,
        value: deviceUpdate.value,
        ownUser: deviceUpdate.ownUser,
        meshNetwork: deviceUpdate.meshNetwork
      }
    } catch (error) {
      throw new ErrorResponse("Couldn't find this data from server", 500);
    }
  };

  setMesh = async (req) => {
    try {
      const { name, meshid, mac } = req.body
      let mesh = null;
      const findedMesh = await MeshModel.findOne({ meshid: meshid }).lean({});

      if (!findedMesh) {
        mesh = await MeshModel.create({
          user: await req.get("CLIENT_ID"),
          meshid: meshid,
          name: name,
          meshpass: "meshpass"
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
        throw new ErrorResponse("Couldn't set mesh", 500);
      }

      return {
        device: deviceUpdate._id
      }
    } catch (error) {
      throw new ErrorResponse("Couldn't find this data from server", 500);
    }
  };

  history = async (req) => {
    const userid = await req.get("CLIENT_ID");
    const { mac, limit } = await req.params;

    const device = await DeviceModel.findOne({ mac: mac }).lean({});
    if (!device || device.ownUser != userid) {
      throw new ErrorResponse("This not your device!", 400);
    }

    const history = await HistoryModel.find(
      {
        device: device._id
      },
      null,
      {
        sort: { _id: -1 },
        limit: parseInt(limit),
        lean: {}
      }
    );
    return history
  }

  setstate = async (req) => {
    try {
      const { mac, state } = req.body;
      const userid = await req.get("CLIENT_ID");

      const device = await DeviceModel.findOne({ mac: mac }).lean({});
      if (!device || device.ownUser != userid) {
        throw new ErrorResponse("This not your device!", 400);
      }

      instanceMqtt.mqttClient.publish(
          mac,
          JSON.stringify(
            {
                "cmd": 4,
                "control": state == '1' ? 1 : 0
            }
          )
      )

      console.log(JSON.stringify(
        {
            "cmd": 4,
            "control": state == '1' ? 1 : 0
        }
      ))
    } catch (error) {
      throw new ErrorResponse("Couldn't set state", 500);
    }
  };
}

export default new DeviceUserService();
