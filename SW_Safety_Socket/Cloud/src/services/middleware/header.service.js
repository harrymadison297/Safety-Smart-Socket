import jwt from "jsonwebtoken";
import { promisify } from "util";
import ErrorResponse from "../../helpers/error.response.js";
import UserModel from "../../database/schema/user.model.js";
import { configDotenv } from "dotenv";
configDotenv();

export const AuthenticationHeader = async (req, res, next) => {
    const userID = await req.get("CLIENT_ID");
    const accessToken = await req.get("ACCESS_TOKEN");
    // Check access Token
    if (!accessToken) {
        throw new ErrorResponse(
            "You are not logged in, Please login to get access"
        );
    }

    // verify token
    const decoded = await promisify(jwt.verify)(
        accessToken,
        process.env.JWT_SECRET
    );

    // check if user exists
    const currentUser = await UserModel.findById(decoded.userId);
    if (!currentUser) {
        throw new ErrorResponse(
            "The Token belong to this user no longer exits yet"
        );
    }

    if (currentUser._id != userID) {
        throw new ErrorResponse(`you are not belong to this page`, 404);
    }

    // Check user

    next();
};
