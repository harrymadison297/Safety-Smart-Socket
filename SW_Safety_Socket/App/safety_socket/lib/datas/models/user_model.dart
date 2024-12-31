import 'package:flutter/cupertino.dart';

import '../api/request_common.dart';

class UserModel extends ChangeNotifier {
  late String _email;
  late String _id;
  late String _name;
  late String _token;

  String get email => _email;
  String get id => _id;
  String get name => _name;
  String get token => _token;

  void updateUserModel (String id, String email, String name, String token) {
    _email = email;
    _id = id;
    _name = name;
    _token = token;
    userModelData['_id'] = _id;
    userModelData['_email'] = _email;
    userModelData['_name'] = _name;
    userModelData['_token'] = _token;
    notifyListeners();
  }
}