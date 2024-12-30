import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:flutter/widgets.dart';
import 'package:get/get.dart';
import 'package:iconly/iconly.dart';
import 'package:provider/provider.dart';
import 'package:safety_socket/config/theme.dart';
import 'package:safety_socket/datas/models/user_model.dart';
import 'package:safety_socket/screens/setting/setting_tab.dart';
import 'package:safety_socket/screens/timer/timer_tab.dart';
import 'package:zoom_tap_animation/zoom_tap_animation.dart';

import '../home/home_tab.dart';
import 'main_controller.dart';

class MainScreen extends StatelessWidget {
  MainScreen({super.key});
  final MainWrapperController controller = Get.put(MainWrapperController());

  @override
  Widget build(BuildContext context) {
    return Consumer<UserModel>(
        builder: (context, value, child) => Scaffold(
        body: GetMaterialApp(
          title: 'Safety Socket',
          debugShowCheckedModeBanner: false,
          theme: themeData,
          home: Scaffold(
            backgroundColor: Colors.white,
            body: PageView(
              physics: const BouncingScrollPhysics(),
              onPageChanged: controller.animateToTab,
              controller: controller.pageController,
              children: [
                HomeTab(),
                TimerTab(),
                SettingTab(),
              ],
            ),
            bottomNavigationBar: BottomAppBar(
              notchMargin: 10,
              elevation: 0,
              child: Container(
                padding: const EdgeInsets.symmetric(horizontal: 35),
                child: Obx(
                  () => Row(
                  mainAxisAlignment: MainAxisAlignment.spaceBetween,
                  children: [
                    _bottomAppBarItem(context, iconLight: IconlyLight.home, iconBold: IconlyBold.home, page: 0, label: "Home"),
                    _bottomAppBarItem(context, iconLight: IconlyLight.time_circle, iconBold: IconlyBold.time_circle, page: 1, label: "Timer"),
                    _bottomAppBarItem(context, iconLight: IconlyLight.user, iconBold: IconlyBold.user_2, page: 2, label: "Setting"),
                  ],
                ),
                )
              ),
            ),
          ),
        )
      )
    );
  }
  
  Widget _bottomAppBarItem(BuildContext context, {required iconLight, required iconBold, required page, required label}) {
    return ZoomTapAnimation(
        onTap: () => controller.goToTab(page),
        child: Column(
            mainAxisSize: MainAxisSize.min,
            children: [
              Icon(controller.currentPage == page ? iconBold : iconLight, color: controller.currentPage == page
                  ? Colors.black87
                  : Colors.grey,
                size: 20,),
              Text(label)
            ]
        ));
  }
}