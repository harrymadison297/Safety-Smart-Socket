import 'dart:async';
import 'dart:convert';
import 'dart:math';

import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:flutter_animate/flutter_animate.dart';
import 'package:google_fonts/google_fonts.dart';
import 'package:safety_socket/datas/api/device_api.dart';
import 'package:safety_socket/datas/models/device_model.dart';
import 'package:safety_socket/datas/models/history_model.dart';

class DeviceCard extends StatefulWidget {
  const DeviceCard({super.key, required this.device});

  final DeviceModel device;

  @override
  State<DeviceCard> createState() => _DeviceCardState();
}

class _DeviceCardState extends State<DeviceCard> {
  late Timer _timer;
  bool currentState = false;
  bool updatingData = false;
  bool alive = false;
  HistoryModel history = HistoryModel.fromJson({});

  _DeviceCardState () {
    _timer = Timer.periodic(const Duration(seconds: 1), (time) => thisDeviceState());
  }

  @override
  void initState() {
    thisDeviceState();
    super.initState();
  }

  @override
  void dispose() {
    super.dispose();
    _timer.cancel();
  }

  void thisDeviceState () async {
    try {
      final data = await DeviceData().deviceState(widget.device.mac);
      final dataJson = await jsonDecode(data.body);
      history = HistoryModel.fromJson(dataJson[0]);
      try {
        updatingData = false;
        DateTime lastUpdated = DateTime.parse(history.createdAt);
        DateTime nowTime = DateTime.now();
        alive = (nowTime.millisecondsSinceEpoch - lastUpdated.millisecondsSinceEpoch) < 5*60*1000;
        currentState = false;
        if (history.value.state != null) {
          currentState = dataJson[0]["value"]["state"];
        }
      } catch (e)
      {
        print(e);
      }
      setState(() {});
    } catch (error) {
      print(error.toString());
    }
  }

  Future sendControlData () async {
    await DeviceData().deviceControl(widget.device.mac, !currentState);
    updatingData = true;
    currentState = !currentState;
    setState(() {});
  }

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.all(10.0),
      child: Container(
        decoration: BoxDecoration(
          borderRadius: BorderRadius.circular(24),
          color: const Color.fromARGB(44, 164, 167, 189),
        ),
        child: Padding(
          padding: const EdgeInsets.symmetric(vertical: 20.0),
          child: Column(
            mainAxisAlignment: MainAxisAlignment.spaceBetween,
            children: [
              Padding(
                padding: const EdgeInsets.symmetric(horizontal: 10, vertical: 0),
                child: Row(
                  mainAxisAlignment: MainAxisAlignment.spaceBetween,
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    SizedBox(
                      width: 25,
                      height: 25,
                      child: updatingData ? Transform.scale(
                        scale: 0.5,
                        child: const CircularProgressIndicator(
                          color: Colors.orange,
                        ),
                      ) : Container(),
                    ),
                    Padding(
                        padding: const EdgeInsets.only(top: 20),
                      child: Image.asset(
                        "assets/images/socket.png",
                        width: 30,
                      ),
                    ),
                    Stack(
                      children: [
                        Container(
                          width: 25,
                          height: 25,
                          decoration: BoxDecoration(
                            color: alive ? Colors.green.shade100 : Colors.orange.shade100,
                            borderRadius: BorderRadius.circular(20), // Adjust the radius as needed
                          ),
                        )
                            .animate(onPlay: (controller) => controller.repeat(reverse: true))
                            .scale(duration: 1000.ms, begin: const Offset(0.7, 0.7), end: const Offset(1, 1)),
                        Positioned(
                          top: 6.5,
                          right: 6.5,
                          child: Container(
                            width: 12,
                            height: 12,
                            decoration: BoxDecoration(
                              color: alive ? Colors.green : Colors.orange,
                              borderRadius: BorderRadius.circular(15), // Adjust the radius as needed
                            ),
                          ),
                        )
                      ],
                    ),
                  ],
                ),
              ),
              Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Text(
                    history.createdAt != "" ? history.createdAt : "",
                    style: GoogleFonts.varelaRound(
                      fontSize: 10,
                      color: Colors.black,
                    ),
                  ),
                  Text(
                      "U: ${history.value.voltage}",
                    style: GoogleFonts.varelaRound(
                      fontSize: 10,
                      color: Colors.black,
                    ),
                  ),
                  Text(
                    "I: ${history.value.current}",
                    style: GoogleFonts.varelaRound(
                      fontSize: 10,
                      color: Colors.black,
                    ),
                  ),
                ],
              ),
              Row(
                children: [
                  Expanded(
                    child: Column(
                      crossAxisAlignment: CrossAxisAlignment.start,
                      children: [
                        Padding(
                          padding: const EdgeInsets.only(left: 15.0),
                          child: Text(
                            widget.device.name,
                            style: GoogleFonts.varelaRound(
                              fontWeight: FontWeight.bold,
                              fontSize: 20,
                              color: Colors.black,
                            ),
                          ),
                        ),
                        Padding(
                          padding: const EdgeInsets.only(left: 15.0),
                          child: Text(
                            widget.device.mac,
                            style: GoogleFonts.varelaRound(
                              fontSize: 12,
                              color: Colors.black,
                            ),
                          ),
                        )
                      ],
                    )
                  ),
                  Transform.rotate(
                    angle: pi / 2,
                    child: CupertinoSwitch(
                      value: currentState,
                      onChanged: (a) => {
                        sendControlData()
                      },
                    ),
                  ),
                ],
              )
            ],
          ),
        ),
      ),
    );
  }
}