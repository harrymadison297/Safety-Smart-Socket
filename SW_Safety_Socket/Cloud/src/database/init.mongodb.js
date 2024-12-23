import { configDotenv } from "dotenv";
import mongoose from "mongoose"

/* Use variable from .env */
configDotenv();

const username = process.env.MONGO_USER
const password = process.env.MONGO_PASSWORD

const mongoConnectionString = `mongodb+srv://${username}:${password}@safetysocketcluster.urnhb.mongodb.net`

/* Singleton design pattern class to connect to MongoDB */
class Database {
    constructor() {
        this.connect()
    }

    connect(type = 'mongodb') {
        mongoose.connect(mongoConnectionString, {
            maxPoolSize: 50
        })
            .then(_ => console.log('Connected MongoDB'))
            .catch(err => console.log('Error connect to', mongoConnectionString, err))
    }

    static getInstance() {
        if (!Database.instance) {
            Database.instance = new Database()
        }
        return Database.instance
    }
}

const instanceMongoDB = Database.getInstance()

export default instanceMongoDB