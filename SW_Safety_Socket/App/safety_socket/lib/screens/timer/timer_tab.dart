import 'dart:convert';

import 'package:flutter/material.dart';
import 'package:safety_socket/datas/api/device_api.dart';

class TimerTab extends StatefulWidget {
  const TimerTab({super.key});

  @override
  State<TimerTab> createState() => _TimerTabState();
}

class _TimerTabState extends State<TimerTab> {

  @override
  void initState() {
    callApi();
    super.initState();
  }

  void callApi() async {
    final data = await DeviceData().deviceAll();
    final jsondata = jsonDecode(data.body);
    print(jsondata);
  }

  @override
  Widget build(BuildContext context) {
    return const Placeholder();
  }
}

