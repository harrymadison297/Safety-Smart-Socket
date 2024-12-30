import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:provider/provider.dart';
import 'package:safety_socket/bloc/auth/auth_bloc.dart';
import 'package:safety_socket/config/router.dart';
import 'package:safety_socket/config/theme.dart';
import 'package:safety_socket/datas/models/user_model.dart';

void main() {
  runApp(
      ChangeNotifierProvider(
          create: (context) => UserModel(),
          child: const MyApp(),
      )
  );
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return BlocProvider(
      create: (context) => AuthBloc(),
      child: const App()
    );
  }
}

class App extends StatelessWidget {
  const App({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp.router(
      debugShowCheckedModeBanner: false,
      theme: themeData,
      routerConfig: router,
    );
  }
}
