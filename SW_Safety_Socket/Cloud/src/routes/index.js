import express from "express";

const DefaultRouter = express.Router();

DefaultRouter
    .use("/", async (req, res, next) => {
        return res.status(200).json({
            message: "Hello"
        })
    })




export default DefaultRouter