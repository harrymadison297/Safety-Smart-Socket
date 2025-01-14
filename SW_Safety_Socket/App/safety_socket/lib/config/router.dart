import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:go_router/go_router.dart';
import 'package:safety_socket/screens/add/new_screen.dart';

import '../bloc/auth/auth_bloc.dart';
import '../screens/auth/login_screen.dart';
import '../screens/auth/register_screen.dart';
import '../screens/main/main_screen.dart';
import '../screens/add/add_screen.dart';

class RouteName {
  static const String home = '/';
  static const String login = '/login';
  static const String register = '/register';
  static const String add = '/add';

  static const publicRoutes = [
    login,
    register,
  ];
}

final router = GoRouter(
  redirect: (context, state) {
    if (RouteName.publicRoutes.contains(state.fullPath)) {
      return null;
    }
    if (context.read<AuthBloc>().state is AuthLoginSuccess) {
      return null;
    } else {
      return RouteName.login;
    }
  },
  routes: [
    GoRoute(
      path: RouteName.home,
      builder: (context, state) => MainScreen(),
    ),
    GoRoute(
      path: RouteName.login,
      builder: (context, state) => LoginScreen(),
    ),
    GoRoute(
      path: RouteName.register,
      builder: (context, state) => RegisterScreen(),
    ),
    GoRoute(
      path: RouteName.add,
      builder: (context, state) => AddScreen(),
    ),
  ],
);