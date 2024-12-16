import compression from "compression";
import Express from "express";
import helmet from "helmet";
import morgan from "morgan";
import cors from "cors";
import bodyParser from "body-parser";
import ErrorResponse from "../ultis/ErrorResponse.js";
import DefaultRouter from "../routes/index.js";

const DeviceOutputService = Express();

// init middleware **************************************
DeviceOutputService.use(morgan("combined")); // System log: combined for product, dev for dev
DeviceOutputService.use(helmet()); // Header protect
DeviceOutputService.use(compression()); // Compress output
DeviceOutputService.use(bodyParser.json()); // Parsing application/json
DeviceOutputService.use(bodyParser.urlencoded({ extended: true })); // Parsing application/x-www-form-urlencoded
DeviceOutputService.use(cors());

// init routers *****************************************
DeviceOutputService.use("", DefaultRouter);

// Use for detected unknown URL *************************
DeviceOutputService.use("*", (req, res, next) => {
  next(new ErrorResponse(` Can't find ${req.originalUrl} on this server`, 404));
});


// handling error ***************************************
DeviceOutputService.use((err, req, res, next) => {
  res.status(err?.code || 500).json({
    status: err?.code || 500,
    message: err?.message || "Interal server handler",
    err: err?.name,
    stack: err?.stack,
  });
});

export default DeviceOutputService;