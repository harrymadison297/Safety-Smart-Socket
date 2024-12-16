import compression from "compression";
import Express from "express";
import helmet from "helmet";
import morgan from "morgan";
import cors from "cors";
import bodyParser from "body-parser";
import ErrorResponse from "../ultis/ErrorResponse.js";
import DefaultRouter from "../routes/index.js";

const ApplicationService = Express();

// init middleware **************************************
ApplicationService.use(morgan("combined")); // System log: combined for product, dev for dev
ApplicationService.use(helmet()); // Header protect
ApplicationService.use(compression()); // Compress output
ApplicationService.use(bodyParser.json()); // Parsing application/json
ApplicationService.use(bodyParser.urlencoded({ extended: true })); // Parsing application/x-www-form-urlencoded
ApplicationService.use(cors());

// init routers *****************************************
ApplicationService.use("", DefaultRouter);

// Use for detected unknown URL *************************
ApplicationService.use("*", (req, res, next) => {
  next(new ErrorResponse(` Can't find ${req.originalUrl} on this server`, 404));
});


// handling error ***************************************
ApplicationService.use((err, req, res, next) => {
  res.status(err?.code || 500).json({
    status: err?.code || 500,
    message: err?.message || "Interal server handler",
    err: err?.name,
    stack: err?.stack,
  });
});

export default ApplicationService;