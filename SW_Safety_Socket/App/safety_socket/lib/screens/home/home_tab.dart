import 'dart:convert';

import 'package:flutter/material.dart';

import '../../datas/api/device_api.dart';
import '../../datas/models/device_model.dart';

class HomeTab extends StatefulWidget {
  const HomeTab({super.key});

  @override
  State<HomeTab> createState() => _HomeTabState();
}

class _HomeTabState extends State<HomeTab> {

  List<DeviceModel> listDevice = [];

  @override
  void initState() {
    callApi();
    super.initState();
  }

  void callApi() async {
    final data = await DeviceData().deviceAll();
    final jsonData = await jsonDecode(data.body);
    if (jsonData != null)
      {
        for (var dev in jsonData)
          {
            listDevice.add(DeviceModel.fromJson(dev));
            setState(() {});
          }
      }
  }

  @override
  Widget build(BuildContext context) {
    return Column(
      children: [
        ...
        listDevice.map(
            (e) =>  Text(e.mac!)
        )
      ],
    );
  }
}

