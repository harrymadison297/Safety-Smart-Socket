// Define an ErrorResponse class that extends the built-in Error class.
class ErrorResponse extends Error {
    constructor(message, code) {
        super(message)
        this.code = code
    }
}

export default ErrorResponse