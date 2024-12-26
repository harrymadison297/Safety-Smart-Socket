import mongoose, { Schema } from "mongoose";

const DOCUMENT_NAME = "Device";
const COLLECTION_NAME = "DEVICES";
// Declare the Schema of the Mongo model
// switchDevice
var deviceSchema = new mongoose.Schema(
  {
    name: {
      type: String,
      required: [true, "A device must have name"],
      trim: true,
      maxLength: 20,
      minLength: 5,
    },
    mac: {
      type: String,
      require: true,
      unique: true,
    },
    devicePublic: {
      type: String,
      require: true,
    },
    deviceSecret: {
      type: String,
      require: true,
    },
    value: {
      type: Object,
    },
    ownUser: {
      type: Schema.Types.ObjectId,
      ref: "User",
    },
    meshNetwork: {
      type: Schema.Types.ObjectId,
      ref: "Mesh",
    }
  },
  {
    collection: COLLECTION_NAME,
  }
);

const DeviceModel = mongoose.model(DOCUMENT_NAME, deviceSchema);

export default DeviceModel;
