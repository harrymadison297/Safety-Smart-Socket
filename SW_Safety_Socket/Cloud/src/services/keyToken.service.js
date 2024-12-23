import { configDotenv } from "dotenv";
import jwt from "jsonwebtoken";
import keyTokenModel from "../database/schema/keyToken.model.js"

configDotenv();

const updateAccessToken = async (userId, accessToken) => {
  const filter = {
    user: userId,
  };

  const update = {
    accessToken: accessToken,
  };

  const options = {
    new: true,
    upsert: true
  };

  const updateToken = await keyTokenModel.findOneAndUpdate(
    filter,
    update,
    options
  );
  return updateToken;
};

const signTokenID = async (userID) => {
  return jwt.sign({ userId: userID }, process.env.JWT_SECRET, {
    expiresIn: "2d",
  });
};



export { signTokenID, updateAccessToken };
