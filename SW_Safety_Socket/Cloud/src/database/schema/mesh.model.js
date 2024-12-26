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
    meshid: {
      type: String,
      trim: true,
      unique: true,
    },
    meshpass: {
      type: String,
      trim: true,
    },
  },
  {
    collection: COLLECTION_NAME,
  }
);

const MeshModel = mongoose.model(DOCUMENT_NAME, meshSchema);
//Export the model
export default MeshModel;
