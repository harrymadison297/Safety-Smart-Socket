import Express from "express"
import swaggerUi from "swagger-ui-express"
import compression from "compression";
import helmet from "helmet";
import morgan from "morgan";
import cors from "cors";
import bodyParser from "body-parser";

import swaggerDocs from "../swagger/swagger.json" with { type: "json" };
import manufactorRouter from "../routes/manufactor/index.js";
import instanceMongoDB from "../database/init.mongodb.js";

const manufactorApp = Express();

/**
 * Setup middleware
 */
manufactorApp.use(morgan("combined"));                            // System log
manufactorApp.use(helmet(
  {
    contentSecurityPolicy: {
        directives: {
            defaultSrc: ["'self'"],
            connectSrc: ["'self'", "http://localhost:3000"] // Add your API endpoint
        }
    }
}
));                                      // Header protect
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

/**
 * Manufactor swagger routers
 */
manufactorApp.use('/api-docs', swaggerUi.serve, swaggerUi.setup(swaggerDocs));

/**
 * Manufactor api routers
 */
manufactorApp.use('/manf', manufactorRouter);

/**
 * Handle URL exception
 */
manufactorApp.use("*", (req, res, next) => {
  next(new ErrorResponse(` Can't find ${req.originalUrl} on this server`, 404));
});

/**
 * Handle error exception
 */
manufactorApp.use((err, req, res, next) => {
  res.status(err?.code || 500).json({
    message: err?.message || "Interal server handler"
  });
});

export default manufactorApp