import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:go_router/go_router.dart';

import '../bloc/auth/auth_bloc.dart';
import '../screens/auth/login_screen.dart';
import '../screens/auth/register_screen.dart';
import '../screens/home/main_screen.dart';

class RouteName {
  static const String home = '/';
  static const String login = '/login';
  static const String postDetail = '/post/:id';
  static const String profile = '/profile';
  static const String register = '/register';

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
      return RouteName.home;
    }
    return RouteName.login;
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
  ],
);