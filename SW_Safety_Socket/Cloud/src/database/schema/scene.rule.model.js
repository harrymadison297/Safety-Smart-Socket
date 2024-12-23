import mongoose, { Schema } from "mongoose";

const DOCUMENT_NAME = 'Scene'
const COLLECTION_NAME = 'SCENE'
// Declare the User Schema of the Mongo model
const sceneSchema = new mongoose.Schema({
    name: {
        type: String,
        required: true,
        trim: true,
        maxLength: 150,
    },
    user: {
        type: Schema.Types.ObjectId,
        required: true,
        ref: "User",
    },
    control: {
        type: Array,
        default: []
    }
}, {
    collection: COLLECTION_NAME
});

const SceneModel = mongoose.model(DOCUMENT_NAME, sceneSchema);
//Export the model
export default SceneModel