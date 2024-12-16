import compression from "compression";
import Express from "express";
import helmet from "helmet";
import morgan from "morgan";
import cors from "cors";
import bodyParser from "body-parser";
import ErrorResponse from "../ultis/ErrorResponse.js";
import DefaultRouter from "../routes/index.js";

const DeviceInputService = Express();

// init middleware **************************************
DeviceInputService.use(morgan("combined")); // System log: combined for product, dev for dev
DeviceInputService.use(helmet()); // Header protect
DeviceInputService.use(compression()); // Compress output
DeviceInputService.use(bodyParser.json()); // Parsing application/json
DeviceInputService.use(bodyParser.urlencoded({ extended: true })); // Parsing application/x-www-form-urlencoded
DeviceInputService.use(cors());

// init routers *****************************************
DeviceInputService.use("", DefaultRouter);

// Use for detected unknown URL *************************
DeviceInputService.use("*", (req, res, next) => {
  next(new ErrorResponse(` Can't find ${req.originalUrl} on this server`, 404));
});


// handling error ***************************************
DeviceInputService.use((err, req, res, next) => {
  res.status(err?.code || 500).json({
    status: err?.code || 500,
    message: err?.message || "Interal server handler",
    err: err?.name,
    stack: err?.stack,
  });
});

export default DeviceInputService;