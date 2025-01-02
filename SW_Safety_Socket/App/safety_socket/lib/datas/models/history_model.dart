class HistoryModel {
  String createdAt = "";
  HistoryValueModel value = HistoryValueModel.fromJson({});

  HistoryModel.fromJson(Map<String, dynamic> json) {
    try {
      createdAt = json['createdAt'];
      value = HistoryValueModel.fromJson(json['value']);
    } catch (e) {
      print(e);
    }
  }
}

class HistoryValueModel {
  String? mac = "";
  double? voltage = 0;
  double? current = 0;
  bool? state = false;


  HistoryValueModel.fromJson(Map<String, dynamic> json) {
    try {
      mac = json["mac"];
      voltage = json["voltage"];
      current = json["current"];
      state = json["state"];
    } catch (e) {
      print(e);
    }
  }

}