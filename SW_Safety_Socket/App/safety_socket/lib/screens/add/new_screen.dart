import 'dart:async';
import 'dart:convert';

import 'package:flutter/material.dart';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';
import 'package:google_fonts/google_fonts.dart';
import 'package:iconly/iconly.dart';

import '../../datas/api/device_api.dart';
import '../../datas/api/request_common.dart';

class NewScreen extends StatefulWidget {
  const NewScreen({super.key, required this.device});

  final BluetoothDevice device;
  @override
  State<NewScreen> createState() => _NewScreenState();
}

class _NewScreenState extends State<NewScreen> {
  int phase = 0;
  late List<BluetoothService> _services;
  late StreamSubscription<List<int>> _lastValueSubscription;
  List<int> _ble_value = [];
  late String publicKey;
  late String privateKey;
  late String deviceMac;
  Map<String, int> phaseIdInt = {
    "requestMTU": 0,
    "checkValidDevice": 1,
    "exchangeDeviceSecret": 2,
    "setupDeviceMesh": 3,
    "wifiProvision": 4,
    "completed": 5,
  };

  @override
  void initState() {
    phaseProcess();
    super.initState();
  }

  @override
  void dispose() {
    _lastValueSubscription.cancel();
    super.dispose();
  }

  Future phaseProcess() async {
    await requestMTU();
  }

  Future requestMTU() async {
    try {
      await widget.device.requestMtu(512, predelay: 0);
      phase = phase+1;
      setState(() {});
      await checkValidDevice();
    } catch (e) {
      print(e);
    }
  }

  Future initBLEConnect() async {
    _services = await widget.device.discoverServices();
    while (!await _services.last.characteristics.last.setNotifyValue(true)) {
      Future.delayed(const Duration(milliseconds: 300));
    };
    _lastValueSubscription = _services.last.characteristics.last.lastValueStream.listen((value) {
      _ble_value = value;
      setState(() {});
    });
  }

  Future checkValidDevice() async {
    //if (phase != phaseIdInt["checkValidDevice"]) { return; }
    await initBLEConnect();
    await Future.delayed(const Duration(milliseconds: 100));
    await _services.last.characteristics.last.read();
    deviceMac = utf8.encode(String.fromCharCodes(_ble_value)).map((e) => e.toRadixString(16)).join();
    final data = await DeviceData().deviceClaim(deviceMac);
    final jsonData = await jsonDecode(data.body);
    if (jsonData != null)
    {
      publicKey = jsonData["devicePublic"].toString();
      setState(() {
        phase++;
      });
      await exchangeDeviceSecret();

    }
  }

  Future exchangeDeviceSecret() async {
    //if (phase != phaseIdInt["exchangeDeviceSecret"]) { return; }
    setState(() {
      phase++;
    });
    await setupDeviceMesh();
  }

  Future setupDeviceMesh() async {
    //if (phase != phaseIdInt["setupDeviceMesh"]) { return; }
    // final data = await DeviceData().deviceSetMesh(deviceMac);
    setState(() {
      phase++;
    });
    await wifiSetup();
  }

  Future wifiSetup() async {
    //if (phase != phaseIdInt["wifiProvision"]) { return; }
    final _ssidController = TextEditingController();
    final _passwordController = TextEditingController();
      showModalBottomSheet(context: context, builder: (BuildContext context) {
        return Container(
          margin: const EdgeInsets.only(left: 30, top: 30, right: 30),
          child: Column(
            children: <Widget>[
              Container(
                padding: const EdgeInsets.only(left: 8.0),
                decoration: const BoxDecoration(
                    border: Border(left: BorderSide(
                        color: Color.fromRGBO(143, 148, 251, 1)))
                ),
                child: TextField(
                  controller: _ssidController,
                  decoration: InputDecoration(
                      border: InputBorder.none,
                      hintText: "Wifi SSID",
                      hintStyle: TextStyle(color: Colors.grey[700])
                  ),
                ),
              ),
              const SizedBox(height: 10,),
              Container(
                padding: const EdgeInsets.only(left: 8.0),
                decoration: const BoxDecoration(
                    border: Border(left: BorderSide(
                        color: Color.fromRGBO(143, 148, 251, 1)))
                ),
                child: TextField(
                  controller: _passwordController,
                  obscureText: true,
                  decoration: InputDecoration(
                      border: InputBorder.none,
                      hintText: "Password",
                      hintStyle: TextStyle(color: Colors.grey[700])
                  ),
                ),
              ),
              const SizedBox(height: 30,),
              TextButton(
                  onPressed: () {
                    Map<String, String> wifi_prov_data = {
                      "ble_pw": "123456",
                      "router_ssid": _ssidController.text,
                      "router_pw": _passwordController.text,
                      "clientid": userModelData['_id'].toString()
                    };
                    String write_data = jsonEncode(wifi_prov_data);
                    _services.last.characteristics.last.write(
                        utf8.encode(write_data));
                    Navigator.pop(context);
                  },
                  style: TextButton.styleFrom(
                      padding: const EdgeInsets.fromLTRB(0, 0, 0, 0)
                  ),
                  child: Container(
                    height: 50,
                    decoration: BoxDecoration(
                        borderRadius: BorderRadius.circular(10),
                        gradient: const LinearGradient(
                            colors: [
                              Color.fromRGBO(143, 148, 251, 1),
                              Color.fromRGBO(143, 148, 251, .6),
                            ]
                        )
                    ),
                    child: const Center(
                      child: Text("Confirm", style: TextStyle(
                          color: Colors.white, fontWeight: FontWeight.bold),),
                    ),
                  )),
            ],
          ),
        );
      });
    while (true) {
      Future.delayed(Duration(milliseconds: 10));
      _services.last.characteristics.last.read();
      if (AsciiDecoder(allowInvalid: true).convert(_ble_value).toString() == "OK")
        {
          setState(() {
            phase++;
          });
        }
      if (AsciiDecoder(allowInvalid: true).convert(_ble_value).toString() == "FAIL")
      {
        return false;
      }
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: Colors.white,
      body: Container(
        margin: const EdgeInsets.only(top: 60),
        child: Padding(
          padding: const EdgeInsets.symmetric(horizontal: 30),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              Text(
                "Setting up device ${widget.device.advName}",
                style: GoogleFonts.varelaRound(
                  fontSize: 26,
                  color: Colors.grey.shade800,
                  fontWeight: FontWeight.w700,
                ),
              ),
              const SizedBox(height: 5),
              Text(
                "${widget.device.remoteId.str}",
                style: GoogleFonts.varelaRound(fontSize: 16, color: Colors.grey.shade800),
              ),
              const SizedBox(height: 5),
              Text(
                "MTU: ${widget.device.mtuNow}",
                style: GoogleFonts.varelaRound(fontSize: 16, color: Colors.grey.shade800),
              ),
              const SizedBox(height: 30),
              Phase(0, "Request MTU"),
              const SizedBox(height: 15),
              Phase(1, "Check valid device"),
              const SizedBox(height: 15),
              Phase(2, "Exchange device secret"),
              const SizedBox(height: 15),
              Phase(3, "Allow this device join your mesh"),
              const SizedBox(height: 15),
              Phase(4, "Give device Wifi SSID and Password"),
              const SizedBox(height: 15),
              Phase(5, "Set up new device completed"),

              const SizedBox(height: 30),
              Row(
                mainAxisAlignment: MainAxisAlignment.center,
                children: [
                  TextButton(
                      onPressed: () {
                        widget.device.disconnect();
                        Navigator.of(context).pop();
                        },
                    style: TextButton.styleFrom(
                        padding: const EdgeInsets.fromLTRB(30, 10, 30, 10),
                      backgroundColor: Colors.red.shade100
                    ),
                      child: Text(
                        "Cancel",
                        style: GoogleFonts.varelaRound(
                          fontSize: 26,
                          color: Colors.red,
                          fontWeight: FontWeight.w700,
                        ),
                      ),
                  )
                ],
              )
            ],
          ),
        ),
      ),
    );
  }

  Widget Phase(int phaseId, String title){
    Widget icon = const Icon(IconlyLight.tick_square, color: Colors.grey, size: 30);
    if (phase == phaseId) {
      icon = const Icon(IconlyLight.more_circle, color: Colors.black, size: 30);
    }
    if (phase > phaseId) {
      icon = const Icon(IconlyLight.tick_square, color: Colors.green, size: 30);
    }
    return Row(
      crossAxisAlignment: CrossAxisAlignment.center,
      children: [
        Container(
          height: 50,
          width: 50,
          decoration: BoxDecoration(
            shape: BoxShape.circle,
            color: Colors.orange.shade100,
          ),
          child: icon
        ),
        const SizedBox(width: 10),
        SizedBox(
          width: 200,
          child: Text(
            title,
            style: GoogleFonts.varelaRound(
              fontSize: 16,
              color: Colors.grey.shade800,
            ),
            softWrap: true,
          ),
        )
      ],
    );
  }
}
