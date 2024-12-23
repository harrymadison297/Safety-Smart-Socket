import mongoose, { Schema } from "mongoose";

const DOCUMENT_NAME = "Key";
const COLLECTION_NAME = "KEYS";
// Declare the User Schema of the Mongo model
const keyTokenSchema = new mongoose.Schema(
  {
    user: {
      type: Schema.Types.ObjectId,
      required: true,
      ref: "User",
    },
    accessToken: {
      type: String,
      require: true,
    },
  },
  {
    collection: COLLECTION_NAME,
  }
);

const keyTokenModel = mongoose.model(DOCUMENT_NAME, keyTokenSchema);
//Export the model
export default keyTokenModel;
