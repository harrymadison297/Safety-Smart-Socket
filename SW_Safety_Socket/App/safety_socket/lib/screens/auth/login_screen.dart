import 'package:animate_do/animate_do.dart';
import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:flutter/painting.dart';
import 'package:flutter/rendering.dart';
import 'package:flutter/widgets.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:go_router/go_router.dart';
import 'package:provider/provider.dart';
import 'package:safety_socket/bloc/auth/auth_bloc.dart';
import 'package:safety_socket/datas/models/user_model.dart';

import '../../config/router.dart';

class LoginScreen extends StatelessWidget {
  const LoginScreen({super.key});

  @override
  Widget build(BuildContext context) {
    final _emailController = TextEditingController();
    final _passwordController = TextEditingController();

    void _handleLogin(BuildContext context) {
      context.read<AuthBloc>().add(
        AuthLoginStarted(email: _emailController.text, password: _passwordController.text)
      );
    }

    return Consumer(builder: (context, value, child) => Scaffold(
        backgroundColor: Colors.white,
        body: SingleChildScrollView(
          child: BlocBuilder<AuthBloc, AuthState>(
            builder: (context, state) {

              final initialLoginWidget = Column(
                children: <Widget>[
                  Container(
                    height: 400,
                    decoration: const BoxDecoration(
                        image: DecorationImage(
                            image: AssetImage('assets/images/background.png'),
                            fit: BoxFit.fill
                        )
                    ),
                    child: Stack(
                      children: <Widget>[
                        Positioned(
                          left: 30,
                          width: 80,
                          height: 200,
                          child: FadeInDown(duration: const Duration(seconds: 1), child: Container(
                            decoration: const BoxDecoration(
                                image: DecorationImage(
                                    image: AssetImage('assets/images/light-1.png')
                                )
                            ),
                          )),
                        ),
                        Positioned(
                          left: 140,
                          width: 80,
                          height: 150,
                          child: FadeInDown(duration: const Duration(milliseconds: 100), child: Container(
                            decoration: const BoxDecoration(
                                image: DecorationImage(
                                    image: AssetImage('assets/images/light-2.png')
                                )
                            ),
                          )),
                        ),
                        Positioned(
                          right: 40,
                          top: 40,
                          width: 80,
                          height: 150,
                          child: FadeInDown(duration: const Duration(milliseconds: 100), child: Container(
                            decoration: const BoxDecoration(
                                image: DecorationImage(
                                    image: AssetImage('assets/images/clock.png')
                                )
                            ),
                          )),
                        ),
                        Positioned(
                          child: FadeInDown(duration: const Duration(milliseconds: 200), child: Container(
                            margin: const EdgeInsets.only(top: 50),
                            child: const Center(
                              child: Text("Login", style: TextStyle(color: Colors.white, fontSize: 40, fontWeight: FontWeight.bold),),
                            ),
                          )),
                        )
                      ],
                    ),
                  ),
                  Padding(
                    padding: const EdgeInsets.all(30.0),
                    child: Column(
                      children: <Widget>[
                        FadeInDown(duration: const Duration(milliseconds: 300), child: Container(
                          padding: const EdgeInsets.all(5),
                          decoration: BoxDecoration(
                              color: Colors.white,
                              borderRadius: BorderRadius.circular(10),
                              border: Border.all(color: const Color.fromRGBO(143, 148, 251, 1)),
                              boxShadow: const [
                                BoxShadow(
                                    color: Color.fromRGBO(143, 148, 251, .2),
                                    blurRadius: 20.0,
                                    offset: Offset(0, 10)
                                )
                              ]
                          ),
                          child: Column(
                            children: <Widget>[
                              Container(
                                padding: const EdgeInsets.all(8.0),
                                decoration: const BoxDecoration(
                                    border: Border(bottom: BorderSide(color:  Color.fromRGBO(143, 148, 251, 1)))
                                ),
                                child: TextField(
                                  controller: _emailController,
                                  decoration: InputDecoration(
                                      border: InputBorder.none,
                                      hintText: "Email",
                                      hintStyle: TextStyle(color: Colors.grey[700])
                                  ),
                                ),
                              ),
                              Container(
                                padding: const EdgeInsets.all(8.0),
                                child: TextField(
                                  controller: _passwordController,
                                  obscureText: true,
                                  decoration: InputDecoration(
                                      border: InputBorder.none,
                                      hintText: "Password",
                                      hintStyle: TextStyle(color: Colors.grey[700])
                                  ),
                                ),
                              )
                            ],
                          ),
                        )),
                        const SizedBox(height: 30,),
                        FadeInDown(
                            duration: const Duration(milliseconds: 400),
                            child: TextButton(
                                onPressed: () { _handleLogin(context); },
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
                                    child: Text("Login", style: TextStyle(color: Colors.white, fontWeight: FontWeight.bold),),
                                  ),
                                ))),
                        const SizedBox(height: 70,),
                        FadeInDown(
                            duration: const Duration(milliseconds: 500),
                            child: TextButton(
                                onPressed: () {
                                  context.go(RouteName.register);
                                },
                                child: const Text("Don't have an account? Sign up", style: TextStyle(color: Color.fromRGBO(143, 148, 251, 1)),
                                )
                            )),
                      ],
                    ),
                  )
                ],
              );

              final loadingLoginWidget = Column(
                children: <Widget>[
                  Container(
                    height: 400,
                    decoration: const BoxDecoration(
                        image: DecorationImage(
                            image: AssetImage('assets/images/background.png'),
                            fit: BoxFit.fill
                        )
                    ),
                    child: Stack(
                      children: <Widget>[
                        Positioned(
                          left: 30,
                          width: 80,
                          height: 200,
                          child: FadeInDown(duration: const Duration(seconds: 1), child: Container(
                            decoration: const BoxDecoration(
                                image: DecorationImage(
                                    image: AssetImage('assets/images/light-1.png')
                                )
                            ),
                          )),
                        ),
                        Positioned(
                          left: 140,
                          width: 80,
                          height: 150,
                          child: FadeInDown(duration: const Duration(milliseconds: 100), child: Container(
                            decoration: const BoxDecoration(
                                image: DecorationImage(
                                    image: AssetImage('assets/images/light-2.png')
                                )
                            ),
                          )),
                        ),
                        Positioned(
                          right: 40,
                          top: 40,
                          width: 80,
                          height: 150,
                          child: FadeInDown(duration: const Duration(milliseconds: 100), child: Container(
                            decoration: const BoxDecoration(
                                image: DecorationImage(
                                    image: AssetImage('assets/images/clock.png')
                                )
                            ),
                          )),
                        ),
                        Positioned(
                          child: FadeInDown(duration: const Duration(milliseconds: 200), child: Container(
                            margin: const EdgeInsets.only(top: 50),
                            child: const Center(
                              child: Text("Login", style: TextStyle(color: Colors.white, fontSize: 40, fontWeight: FontWeight.bold),),
                            ),
                          )),
                        )
                      ],
                    ),
                  ),
                  const CircularProgressIndicator()
                ],
              );

              return (
                BlocListener<AuthBloc, AuthState>(
                  listener: (context, state) {
                    switch (state) {
                      case AuthLoginSuccess():
                        context.read<UserModel>().updateUserModel(state.id, state.email, state.name, state.token);
                        context.go(RouteName.home);
                        break;
                      case AuthLoginFailure():
                        ScaffoldMessenger.of(context).showSnackBar(
                          SnackBar(
                            content: const Text('Wrong email or password!'),
                            action: SnackBarAction(label: 'Hide', onPressed: ScaffoldMessenger.of(context).hideCurrentSnackBar),
                            backgroundColor: Colors.red
                          ),
                        );
                      default:
                    }
                  },
                  child: switch (state) {
                    AuthInitial() => initialLoginWidget,
                    AuthLoginInProgress() => loadingLoginWidget,
                    AuthLoginFailure() => initialLoginWidget,
                    _ => Container()
                  },
                )
              );
            },
          )
        )
    )
    );
  }
}
