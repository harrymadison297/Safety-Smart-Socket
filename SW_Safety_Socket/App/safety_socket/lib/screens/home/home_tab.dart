import 'dart:convert';

import 'package:flutter/material.dart';
import 'package:go_router/go_router.dart';
import 'package:google_fonts/google_fonts.dart';
import 'package:iconly/iconly.dart';
import 'package:provider/provider.dart';
import 'package:safety_socket/config/router.dart';
import 'package:safety_socket/screens/add/add_screen.dart';

import '../../datas/api/device_api.dart';
import '../../datas/models/device_model.dart';
import '../../datas/models/user_model.dart';
import '../../widgets/device_card_widget.dart';

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
    return Consumer<UserModel>(
        builder: (context, value, child) => Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [

        const SizedBox(height: 60),

        // welcome home
        Padding(
          padding: EdgeInsets.symmetric(horizontal: 30),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              Text(
                "Hi ${value.name},",
                style: GoogleFonts.varelaRound(
                    fontSize: 26,
                    color: Colors.grey.shade800,
                    fontWeight: FontWeight.w700,
                ),
              ),
              Text(
                "Welcome to your smart home",
                style: GoogleFonts.varelaRound(fontSize: 16, color: Colors.grey.shade800),
              ),
            ],
          ),
        ),

        const SizedBox(height: 25),

        const Padding(
          padding: EdgeInsets.symmetric(horizontal: 30.0),
          child: Divider(
            thickness: 1,
            color: Color.fromARGB(255, 204, 204, 204),
          ),
        ),

        const SizedBox(height: 25),

        // smart devices grid
        Padding(
          padding: const EdgeInsets.symmetric(horizontal: 30),
          child: Row(
            mainAxisAlignment: MainAxisAlignment.spaceBetween,
            children: [
              Text(
                "Smart Devices",
                style: TextStyle(
                  fontWeight: FontWeight.bold,
                  fontSize: 24,
                  color: Colors.grey.shade800,
                ),
              ),
              IconButton(
                  onPressed: () { Navigator.push(context, MaterialPageRoute(builder: (context) => const AddScreen())); },
                  icon: const Icon(IconlyLight.plus, size: 28,),
              )
            ],
          )
        ),
        const SizedBox(height: 10),

        // grid
        Expanded(
          child: GridView.builder(
            itemCount: listDevice.length,
            physics: const AlwaysScrollableScrollPhysics(),
            padding: const EdgeInsets.symmetric(horizontal: 20),
            gridDelegate: const SliverGridDelegateWithFixedCrossAxisCount(
              crossAxisCount: 2,
              childAspectRatio: 1 / 1.3,
            ),
            itemBuilder: (context, index) {
              return DeviceCard(device: listDevice[index],);
            },
          ),
        )
      ],
    ));
  }
}

