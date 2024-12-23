import mongoose, { Schema } from "mongoose";

const DOCUMENT_NAME = "Notifi";
const COLLECTION_NAME = "NOTIFIS";
// Declare the User Schema of the Mongo model
const notifiSchema = new mongoose.Schema(
  {
    user: {
      type: Schema.Types.ObjectId,
      required: true,
      ref: "User",
    },
    message: {
      type: String,
      require: true,
    },
  },
  {
    timestamps: true,
    collection: COLLECTION_NAME,
  }
);

const notifiModel = mongoose.model(DOCUMENT_NAME, notifiSchema);
//Export the model
export default notifiModel;
