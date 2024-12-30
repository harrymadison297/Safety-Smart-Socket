import 'package:animate_do/animate_do.dart';
import 'package:flutter/material.dart';
import 'package:go_router/go_router.dart';

import '../../config/router.dart';

class RegisterScreen extends StatelessWidget {
  const RegisterScreen({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
        backgroundColor: Colors.white,
        body: SingleChildScrollView(
          child: Column(
            children: <Widget>[
              Container(
                height: 350,
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
                          child: Text("Sign up", style: TextStyle(color: Colors.white, fontSize: 40, fontWeight: FontWeight.bold),),
                        ),
                      )),
                    )
                  ],
                ),
              ),
              Padding(
                padding: const EdgeInsets.symmetric(horizontal: 30.0),
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
                              decoration: InputDecoration(
                                  border: InputBorder.none,
                                  hintText: "What's your name?",
                                  hintStyle: TextStyle(color: Colors.grey[700])
                              ),
                            ),
                          ),
                          Container(
                            padding: const EdgeInsets.all(8.0),
                            decoration: const BoxDecoration(
                                border: Border(bottom: BorderSide(color:  Color.fromRGBO(143, 148, 251, 1)))
                            ),
                            child: TextField(
                              decoration: InputDecoration(
                                  border: InputBorder.none,
                                  hintText: "Enter your email",
                                  hintStyle: TextStyle(color: Colors.grey[700])
                              ),
                            ),
                          ),
                          Container(
                            padding: const EdgeInsets.all(8.0),
                            decoration: const BoxDecoration(
                                border: Border(bottom: BorderSide(color:  Color.fromRGBO(143, 148, 251, 1)))
                            ),
                            child: TextField(
                              obscureText: true,
                              decoration: InputDecoration(
                                  border: InputBorder.none,
                                  hintText: "Enter your password",
                                  hintStyle: TextStyle(color: Colors.grey[700])
                              ),
                            ),
                          ),
                          Container(
                            padding: const EdgeInsets.all(8.0),
                            child: TextField(
                              obscureText: true,
                              decoration: InputDecoration(
                                  border: InputBorder.none,
                                  hintText: "Re-enter your password",
                                  hintStyle: TextStyle(color: Colors.grey[700])
                              ),
                            ),
                          )
                        ],
                      ),
                    )),
                    const SizedBox(height: 30,),
                    FadeInDown(duration: const Duration(milliseconds: 400), child: Container(
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
                        child: Text("Sign up", style: TextStyle(color: Colors.white, fontWeight: FontWeight.bold),),
                      ),
                    )),
                    const SizedBox(height: 70,),
                    FadeInDown(
                        duration: const Duration(milliseconds: 500),
                        child: TextButton(
                            onPressed: () {
                              context.go(RouteName.login);
                            },
                            child: const Text("Already has an account? Log in", style: TextStyle(color: Color.fromRGBO(143, 148, 251, 1)),
                            )
                        )),
                  ],
                ),
              )
            ],
          ),
        )
    );
  }
}
