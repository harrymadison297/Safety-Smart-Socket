import 'package:safety_socket/config.dart';
import 'package:http/http.dart' as http;
import 'package:safety_socket/datas/api/request_common.dart';

class DeviceData {
  Future<http.Response> deviceAll () async {
    const url = "$config_server/device/all";
    Map<String, String> requestHeaders = getHeader();

    try {
      final response = await http.get(
          Uri.parse(url),
        headers: requestHeaders
      );
      return response;
    } catch (e) {
      return http.Response({"message": "Fail to connect"} as String, 500);
    }
  }
}