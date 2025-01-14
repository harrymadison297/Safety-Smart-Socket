import 'dart:convert';

import 'package:safety_socket/config.dart';
import 'package:http/http.dart' as http;
import 'package:safety_socket/datas/api/request_common.dart';

class DeviceData {
  Future<http.Response> deviceAll () async {
    const url = "$config_server/device/all";
    Map<String, String> requestHeaders = getHeader();

    print(requestHeaders.toString());

    try {
      final response = await http.get(
          Uri.parse(url),
        headers: requestHeaders
      );
      return response;
    } catch (e) {
      return http.Response(jsonEncode({"message": "Fail to connect"}), 500);
    }
  }

  Future<http.Response> deviceOne (String id) async {
    final url = "$config_server/device/id/$id";
    Map<String, String> requestHeaders = getHeader();

    try {
      final response = await http.get(
          Uri.parse(url),
          headers: requestHeaders
      );
      return response;
    } catch (e) {
      return http.Response(jsonEncode({"message": "Fail to connect"}), 500);
    }
  }

  Future<http.Response> deviceState (String mac) async {
    final url = "$config_server/device/history/$mac/limit/1";
    Map<String, String> requestHeaders = getHeader();

    try {
      final response = await http.get(
          Uri.parse(url),
          headers: requestHeaders
      );
      return response;
    } catch (e) {
      return http.Response(jsonEncode({"message": "Fail to connect"}), 500);
    }
  }

  Future<http.Response> deviceClaim (String mac) async {
    const url = "$config_server/device/claim";
    Map<String, String> requestHeaders = getHeader();

    try {
      final response = await http.post(
          Uri.parse(url),
          headers: requestHeaders,
          body: {
            "mac": mac
          }
      );
      return response;
    } catch (e) {
      return http.Response(jsonEncode({"message": "Fail to connect"}), 500);
    }
  }

  Future<http.Response> deviceSetMesh (String mac) async {
    const url = "$config_server/device/setmesh";
    Map<String, String> requestHeaders = getHeader();

    try {
      final response = await http.post(
          Uri.parse(url),
          headers: requestHeaders,
          body: {
            "name": "mesh1",
            "mac": mac,
            "meshid": userModelData['_id'].toString()
          }
      );
      return response;
    } catch (e) {
      return http.Response(jsonEncode({"message": "Fail to connect"}), 500);
    }
  }

  Future<http.Response> deviceControl (String mac, bool state) async {
    const url = "$config_server/device/setstate";
    Map<String, String> requestHeaders = getHeader();

    try {
      final response = await http.post(
          Uri.parse(url),
          headers: requestHeaders,
        body: {
            "mac": mac,
          "state": state ? "1" : "0"
        }
      );
      return response;
    } catch (e) {
      print(e);
      return http.Response(jsonEncode({"message": "Fail to connect"}), 500);
    }
  }
}