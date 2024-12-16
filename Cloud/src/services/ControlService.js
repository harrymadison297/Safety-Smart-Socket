import compression from "compression";
import Express from "express";
import helmet from "helmet";
import morgan from "morgan";
import cors from "cors";
import bodyParser from "body-parser";
import ErrorResponse from "../ultis/ErrorResponse.js";
import DefaultRouter from "../routes/index.js";

const ControlService = Express();

// init middleware **************************************
ControlService.use(morgan("combined")); // System log: combined for product, dev for dev
ControlService.use(helmet()); // Header protect
ControlService.use(compression()); // Compress output
ControlService.use(bodyParser.json()); // Parsing application/json
ControlService.use(bodyParser.urlencoded({ extended: true })); // Parsing application/x-www-form-urlencoded
ControlService.use(cors());

// init routers *****************************************
ControlService.use("", DefaultRouter);

// Use for detected unknown URL *************************
ControlService.use("*", (req, res, next) => {
  next(new ErrorResponse(` Can't find ${req.originalUrl} on this server`, 404));
});


// handling error ***************************************
ControlService.use((err, req, res, next) => {
  res.status(err?.code || 500).json({
    status: err?.code || 500,
    message: err?.message || "Interal server handler",
    err: err?.name,
    stack: err?.stack,
  });
});

export default ControlService;