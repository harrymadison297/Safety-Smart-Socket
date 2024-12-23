import mongoose from "mongoose";

const DOCUMENT_NAME = 'Timer'
const COLLECTION_NAME = 'TIMER'
// Declare the User Schema of the Mongo model
const timerSchema = new mongoose.Schema({
    deviceID: {
        type: String,
        require: true
    },
    timeset: {
        type: Date,
        require: true
    },
    value: {
        type: Object,
        require: true
    },
    status: {
        type: String,
        require: true,
    }
}, {
    timestamps: true,
    collection: COLLECTION_NAME
});

const TimerModel = mongoose.model(DOCUMENT_NAME, timerSchema);
//Export the model
export default TimerModel