import { configDotenv } from "dotenv";
import mongoose from "mongoose"

// init environment *************************************
configDotenv();

const username = process.env.MONGODB_USERNAME
const password = process.env.MONGODB_PASSWORD
const mongoConnectionString = `mongodb+srv://${username}:${password}@cluster0.ob0weth.mongodb.net/Smarthome`

// Singleton design pattern class to connect to MongoDB
class Database {
    constructor() {
        this.connect()
    }

    connect(type = 'mongodb') {
        mongoose.connect(mongoConnectionString, {
            maxPoolSize: 50
        })
            .then(_ => console.log('Connected MongoDB'))
            .catch(err => console.log('Error connect to', mongoConnectionString))
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