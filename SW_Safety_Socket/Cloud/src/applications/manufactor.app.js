import express from "express"
import swaggerUi from "swagger-ui-express"
import compression from "compression";
import Express from "express";
import helmet from "helmet";
import morgan from "morgan";
import cors from "cors";
import bodyParser from "body-parser";

import swaggerDocs from "../swagger/swagger.json" with { type: "json" };
import manufactorRouter from "../routes/manufactor/index.js";
import instanceMongoDB from "../database/init.mongodb.js";

const manufactorApp = express();

/**
 * Setup middleware
 */
manufactorApp.use(morgan("combined"));                            // System log
manufactorApp.use(helmet());                                      // Header protect
manufactorApp.use(compression());                                 // Compress output
manufactorApp.use(bodyParser.json());                             // Parsing application/json
manufactorApp.use(bodyParser.urlencoded({ extended: true }));     // Parsing application/x-www-form-urlencoded
manufactorApp.use(cors(
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

manufactorApp.use('/api-docs', swaggerUi.serve, swaggerUi.setup(swaggerDocs));
manufactorApp.use('/manf', manufactorRouter);

export default manufactorApp