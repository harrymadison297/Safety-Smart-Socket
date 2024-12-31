import 'package:bloc/bloc.dart';
import 'package:flutter/material.dart';
import 'package:flutter_animate/flutter_animate.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:safety_socket/datas/api/auth_api.dart';
import 'dart:convert';

import 'package:safety_socket/datas/models/user_model.dart';

part 'auth_event.dart';
part 'auth_state.dart';

class AuthBloc extends Bloc<AuthEvent, AuthState> {
  AuthBloc() : super(AuthInitial()) {
    on<AuthStarted>(_onStarted);
    on<AuthLoginStarted>(_onLoginStarted);
    on<AuthRegisterStarted>(_onRegisterStarted);
    on<AuthLogoutStarted>(_onLogout);
  }

  void _onStarted(AuthStarted event, Emitter<AuthState> emit) async {
    emit(AuthInitial());
  }

  void _onLoginStarted(AuthLoginStarted event, Emitter<AuthState> emit) async {
    emit(AuthLoginInProgress());
    await Future.delayed(1.seconds);
    final authResponse = await AuthData().authLogin(event.email, event.password);
    final parse = jsonDecode(authResponse.body);
    if (authResponse.statusCode == 200)
      {
        emit(AuthLoginSuccess(parse['id'], parse['token'], parse['name'], parse['email']));
      }
    else {
      emit(AuthLoginFailure(parse['message']));
    }
  }

  void _onRegisterStarted(
      AuthRegisterStarted event, Emitter<AuthState> emit) async {
    emit(AuthRegisterInProgress());
    emit(AuthRegisterSuccess());
  }

  void _onLogout(AuthLogoutStarted event, Emitter<AuthState> emit) async {
    emit(AuthInitial());
  }
}
