import 'package:safety_socket/config.dart';
import 'package:http/http.dart' as http;

class AuthData {
  Future<http.Response> authLogin(email, password) async {
    const url = "$config_server/auth/login";
    try {
      final response = await http.post(
          Uri.parse(url),
        body: {
            "email": email,
          "password": password
        }
      );
      return response;
    } catch (e) {
      return http.Response({"message": "Fail to connect"} as String, 500);
    }
  }
}