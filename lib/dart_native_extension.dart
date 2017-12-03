import 'dart-ext:helloworld';

/// Returns '5'.
///
/// An example showcasing calling of native functions from Dart.
int get5() native "get5";

/// Adds [a] and [b] and returns results.
///
/// An example showcasing passing arguments and getting return values to native
/// functions from Dart.
int add(int a, int b) native "add";