import 'package:safety_socket/config.dart';
import 'package:http/http.dart' as http;

class AuthData {
  Future<void> authLogin(email, password) async {
    const url = "$config_server/auth/login";
    try {
      final response = await http.post(
          Uri.parse(url),
        body: {
            email: email,
          password: password
        }
      );
      print(response);
    } catch (e) {}
  }
}