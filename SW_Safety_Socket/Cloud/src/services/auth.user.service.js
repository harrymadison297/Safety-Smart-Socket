import UserModel from "../database/schema/user.model.js";
import bcrypt from "bcrypt";
import ErrorResponse from "../helpers/error.response.js";
import { signTokenID, updateAccessToken } from "./keyToken.service.js";

class AuthUserService {

  login = async ({ email, password }) => {
    // Check email
    const foundUser = await UserModel.findOne({ email: email });
    if (!foundUser) {
      throw new ErrorResponse(`Wrong authentication infomation!`, 403);
    }

    // Check password
    const match = await bcrypt.compare(password, foundUser.password);
    if (!match) {
      throw new ErrorResponse("Wrong authentication infomation!", 403);
    }

    const tokens = await signTokenID(foundUser._id);
    const update = await updateAccessToken(foundUser._id, tokens);
    if(!update){
      throw new ErrorResponse("Cannot update token to server", 404);
    }
    return {
      id: foundUser._id,
      name: foundUser.name,
      email: foundUser.email,
      token: tokens,
    };
  };

  signup = async ({ name, email, password }) => {
    // Check email exit?
    const holderUser = await UserModel.findOne({ email: email }).lean({});
    if (holderUser) {
      throw new ErrorResponse("User exited!", 201);
    }

    // Encrypt password by hash with salt = 10
    const passwordHash = await bcrypt.hash(password, 10);

    // Save to MongoDB
    const newUser = await UserModel.create({
      name: name,
      email: email,
      password: passwordHash,
    });

    if (newUser) {
      return { name: newUser.name, email: newUser.email };
    } else {
      throw new ErrorResponse("Couldn't completed", 401);
    }
  };
}

export default new AuthUserService();
