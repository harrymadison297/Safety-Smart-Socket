part of 'auth_bloc.dart';

sealed class AuthState {}

class AuthInitial extends AuthState {}

class AuthLoginInProgress extends AuthState {}

class AuthLoginSuccess extends AuthState {
  AuthLoginSuccess(this.id, this.token, this.name, this.email);

  final String id;
  final String name;
  final String email;
  final String token;
}

class AuthLoginFailure extends AuthState {
  AuthLoginFailure(this.message);

  final String message;
}

class AuthRegisterInProgress extends AuthState {}

class AuthRegisterSuccess extends AuthState {}

class AuthRegisterFailure extends AuthState {
  AuthRegisterFailure(this.message);

  final String message;
}
