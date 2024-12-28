import DeviceModel from "../database/schema/device.model.js";
import TimerModel from "../database/schema/timer.rule.model.js";
import UserModel from "../database/schema/user.model.js";
import ErrorResponse from "../helpers/error.response.js";

class TimerUserService {
  all = async (req) => {
    const user = await UserModel.findById(await req.get("CLIENT_ID")).lean({});
    const devices = await DeviceModel.find({ ownUser: user._id });
    const allTimer = await TimerModel.find(
      { device: { $in: devices.map(e => e._id) } },
      null,
      { sort: { _id: -1 } }
    )
    return allTimer
  }

  new = async (req) => {
    const { mac, timeset, state, preset } = await req.body;
    const device = await DeviceModel.findOne({ mac: mac }).lean({});

    if (!device || device.ownUser != await req.get("CLIENT_ID")) {
      throw new ErrorResponse("You couldn't control this device!", 400);
    }

    const newTimer = await TimerModel.create(
      {
        device: device._id,
        mac: device.mac,
        timeset: timeset,
        state: state,
        retry: 5,
        preset: preset
      }
    )

    if (!newTimer) {
      throw new ErrorResponse("Couldn't complete this action", 400);
    }

    return newTimer
  }

  delete = async ({ id }) => {
    const deleteTimer = await TimerModel.findByIdAndDelete(id);
    return deleteTimer
  }
}

export default new TimerUserService();
