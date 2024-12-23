import mongoose, { Schema } from "mongoose";

const DOCUMENT_NAME = "Mesh";
const COLLECTION_NAME = "MESHS";
// Declare the User Schema of the Mongo model
const meshSchema = new mongoose.Schema(
  {
    user: {
      type: Schema.Types.ObjectId,
      required: true,
      ref: "User",
    },
    name: {
      type: String,
      trim: true,
    },
    meshId: {
      type: String,
      trim: true,
      unique: true,
    },
    meshPass: {
      type: String,
      trim: true,
    },
  },
  {
    collection: COLLECTION_NAME,
  }
);

const keyTokenModel = mongoose.model(DOCUMENT_NAME, keyTokenSchema);
//Export the model
export default keyTokenModel;
