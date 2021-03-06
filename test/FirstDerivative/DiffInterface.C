// RUN: %cladclang %s -I%S/../../include -fsyntax-only -Xclang -verify 2>&1 | FileCheck %s

#include "clad/Differentiator/Differentiator.h"

//CHECK-NOT: {{.*error|warning|note:.*}}

extern "C" int printf(const char* fmt, ...);

int f_1(float y) {
  int x = 1, z = 3;
  return x * y * z; // x * z
}

// CHECK: int f_1_darg0(float y) {
// CHECK-NEXT: float _d_y = 1;
// CHECK-NEXT: int _d_x = 0, _d_z = 0;
// CHECK-NEXT: int x = 1, z = 3;
// CHECK-NEXT: float _t0 = x * y;
// CHECK-NEXT: return (_d_x * y + x * _d_y) * z + _t0 * _d_z;
// CHECK-NEXT: }

int f_2(int x, float y, int z) {
  return x * y * z; // y * z;
}

// CHECK: int f_2_darg0(int x, float y, int z) {
// CHECK-NEXT: int _d_x = 1;
// CHECK-NEXT: float _d_y = 0;
// CHECK-NEXT: int _d_z = 0;
// CHECK-NEXT: float _t0 = x * y;
// CHECK-NEXT: return (_d_x * y + x * _d_y) * z + _t0 * _d_z;
// CHECK-NEXT: }

// x * z
// CHECK: int f_2_darg1(int x, float y, int z) {
// CHECK-NEXT: int _d_x = 0;
// CHECK-NEXT: float _d_y = 1;
// CHECK-NEXT: int _d_z = 0;
// CHECK-NEXT: float _t0 = x * y;
// CHECK-NEXT: return (_d_x * y + x * _d_y) * z + _t0 * _d_z;
// CHECK-NEXT: }

// x * y
// CHECK: int f_2_darg2(int x, float y, int z) {
// CHECK-NEXT: int _d_x = 0;
// CHECK-NEXT: float _d_y = 0;
// CHECK-NEXT: int _d_z = 1;
// CHECK-NEXT: float _t0 = x * y;
// CHECK-NEXT: return (_d_x * y + x * _d_y) * z + _t0 * _d_z;
// CHECK-NEXT: }

int f_3() {
  int x = 1, z = 3;
  float y = 2;
  return x * y * z; // should not be differentiated
}

int f_no_definition(int x);

int f_redeclared(int x) {
    return x;
}

int f_redeclared(int x);

// CHECK: int f_redeclared_darg0(int x) {
// CHECK-NEXT:   int _d_x = 1;
// CHECK-NEXT:   return _d_x;
// CHECK: }

int f_try_catch(int x)
  try { // expected-warning {{attempted to differentiate unsupported statement, no changes applied}}
    return x;
  }
  catch (int) {
    return 0;
  }

// CHECK: int f_try_catch_darg0(int x) {
// CHECK-NEXT:    int _d_x = 1;
// CHECK-NEXT:    try {
// CHECK-NEXT:        return x;
// CHECK-NEXT:    } catch (int) {
// CHECK-NEXT:        return 0;
// CHECK-NEXT:    }
// CHECK-NEXT: }

int main () {
  int x = 4 * 5;
  clad::differentiate(f_1, 0);

  clad::differentiate(f_2, 0);

  clad::differentiate(f_2, 1);

  clad::differentiate(f_2, 2);

  clad::differentiate(f_2, -1); // expected-error {{Invalid argument index -1 among 3 argument(s)}}

  clad::differentiate(f_2, -1); // expected-error {{Invalid argument index -1 among 3 argument(s)}}

  clad::differentiate(f_2, 3); // expected-error {{Invalid argument index 3 among 3 argument(s)}}

  clad::differentiate(f_2, 9); // expected-error {{Invalid argument index 9 among 3 argument(s)}}

  clad::differentiate(f_2, x); // expected-error {{Failed to parse the parameters, must be a string or numeric literal}}

  clad::differentiate(f_2, f_2); // expected-error {{Failed to parse the parameters, must be a string or numeric literal}}

  clad::differentiate(f_3, 0); // expected-error {{Invalid argument index 0 among 0 argument(s)}}

  float one = 1.0;
  clad::differentiate(f_2, one); // expected-error {{Failed to parse the parameters, must be a string or numeric literal}}

  clad::differentiate(f_no_definition, 0); // expected-error {{attempted differentiation of function 'f_no_definition', which does not have a definition}}

  clad::differentiate(f_redeclared, 0);

  clad::differentiate(f_try_catch, 0);

  clad::differentiate(f_2, "x");
  clad::differentiate(f_2, " y ");
  clad::differentiate(f_2, "z");

  clad::differentiate(f_2, "x, y"); // expected-error {{Forward mode differentiation w.r.t. several parameters at once is not supported, call 'clad::differentiate' for each parameter separately}}
  clad::differentiate(f_2, "t"); // expected-error {{Requested parameter name 't' was not found among function parameters}}
  clad::differentiate(f_2, "x, x"); // expected-error {{Requested parameter 'x' was specified multiple times}}

  return 0;
}
