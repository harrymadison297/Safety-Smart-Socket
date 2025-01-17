import mongoose, { Schema } from "mongoose";

const DOCUMENT_NAME = 'Timer'
const COLLECTION_NAME = 'TIMER'
// Declare the User Schema of the Mongo model
const timerSchema = new mongoose.Schema({
    device: {
        type: Schema.Types.ObjectId,
        require: true,
        ref: "Device"
    },
    mac: {
        type: String,
        require: true
    },
    timeset: {
        type: Number,
        require: true
    },
    timedone:{
        type: Number,
        default: 0
    },
    state: {
        type: Boolean,
        require: true
    },
    active: {
        type: Boolean,
        require: true,
        default: true
    },
    retry: {
        type: Number,
        default: 1
    },
    preset: {
        type: Boolean,
        default: false
    }
}, {
    timestamps: true,
    collection: COLLECTION_NAME
});

const TimerModel = mongoose.model(DOCUMENT_NAME, timerSchema);
//Export the model
export default TimerModel