part of 'auth_bloc.dart';

class AuthEvent {}

class AuthStarted extends AuthEvent {}

class AuthLoginStarted extends AuthEvent {
  AuthLoginStarted({
    required this.email,
    required this.password,
  });

  final String email;
  final String password;
}

class AuthRegisterStarted extends AuthEvent {
  AuthRegisterStarted({
    required this.name,
    required this.email,
    required this.password,
  });

  final String name;
  final String email;
  final String password;
}

class AuthLogoutStarted extends AuthEvent {
  AuthLogoutStarted();
}