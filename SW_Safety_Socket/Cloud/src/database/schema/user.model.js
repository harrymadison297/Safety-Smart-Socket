import mongoose from "mongoose";

const DOCUMENT_NAME = 'User'
const COLLECTION_NAME = 'USERS'

const userSchema = new mongoose.Schema({
    name: {
        type: String,
        required: true,
        trim: true,
        maxLength: 150,
    },
    email: {
        type: String,
        required: true,
        unique: true,
    },
    password: {
        type: String,
        required: true,
    }
}, {
    collection: COLLECTION_NAME
});

const UserModel = mongoose.model(DOCUMENT_NAME, userSchema);
export default UserModel