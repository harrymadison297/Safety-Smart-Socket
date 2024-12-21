import express from "express"
import swaggerUi from "swagger-ui-express"
import swaggerJSDoc from "swagger-jsdoc";
import swaggerDocs from "../swagger/swagger.json" with { type: "json" };

const manufactorApp = express();

const options = {
    definition: {
        openapi: "3.1.0",
        info: {
            title: "Safety Socket Express API with Swagger",
            version: "0.1.0",
            description:
                "This is a simple CRUD API application made with Express and documented with Swagger",
            license: {
                name: "MIT",
                url: "https://spdx.org/licenses/MIT.html",
            },
            contact: {
                name: "Harry Madison",
                url: "https://harrymadison297.vercel.app",
                email: "harrymadison297@gmail.com",
            },
        },
        servers: [
            {
              url: "http://localhost:3000",
            },
          ],
        tags: [
            {
                name: "user",
                description: "User API"
            },
            {
                name: "manu",
                description: "Manufactor API"
            }
        ], 
    },
    apis: ["./src/routes/*/*.js"],
};

const specs = swaggerJSDoc(options);

manufactorApp.use('/api-docs', swaggerUi.serve, swaggerUi.setup(swaggerDocs));

export default manufactorApp