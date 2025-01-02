class DeviceModel {
  String id = "";
  String name = "";
  String mac = "";
  String devicePublic = "";
  String deviceSecret = "";
  String meshNetwork = "";
  String ownUser = "";

  DeviceModel.fromJson(Map<String, dynamic> json) {
    id = json['_id'];
    name = json['name'];
    mac = json['mac'];
    devicePublic = json['devicePublic'];
    deviceSecret = json['deviceSecret'];
    meshNetwork = json['meshNetwork'];
    ownUser = json['ownUser'];
  }
}