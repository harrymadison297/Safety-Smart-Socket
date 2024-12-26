import compression from "compression";
import Express from "express";
import helmet from "helmet";
import morgan from "morgan";
import cors from "cors";
import bodyParser from "body-parser";

import ErrorResponse from "../helpers/error.response.js"
import userRouter from "../routes/user/index.js";
import instanceMongoDB from "../database/init.mongodb.js";

/**
 * Create Express Server instance
 */
const userApp = Express();

/**
 * Setup middleware
 */
userApp.use(morgan("combined"));                            // System log
userApp.use(helmet(                                         // Header protect
  {
    contentSecurityPolicy: {
        directives: {
            defaultSrc: ["'self'"],
            connectSrc: ["'self'", "http://localhost:3000"] // Add your API endpoint
        }
    }
}
));                                                         
userApp.use(compression());                                 // Compress output
userApp.use(bodyParser.json());                             // Parsing application/json
userApp.use(bodyParser.urlencoded({ extended: true }));     // Parsing application/x-www-form-urlencoded
userApp.use(cors(
  {
    "origin": "*",
    "methods": "GET,HEAD,PUT,PATCH,POST,DELETE",
    "preflightContinue": false,
    "optionsSuccessStatus": 204
  }
));

/**
 * Connect to MongoDB
 */
instanceMongoDB;

/**
 * User app routers
 */
userApp.use("", userRouter);

/**
 * Handle URL exception
 */
userApp.use("*", (req, res, next) => {
  next(new ErrorResponse(` Can't find ${req.originalUrl} on this server`, 404));
});

/**
 * Handle error exception
 */
userApp.use((err, req, res, next) => {
  res.status(err?.code || 500).json({
    message: err?.message || "Interal server handler"
  });
});

export default userApp;
