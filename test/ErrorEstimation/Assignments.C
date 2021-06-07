// RUN: %cladclang %s -x c++ -lstdc++ -I%S/../../include -oAssignments.out 2>&1 | FileCheck %s
// RUN: ./Assignments.out
//CHECK-NOT: {{.*error|warning|note:.*}}

#include <iostream>
#include "clad/Differentiator/Differentiator.h"

float func(float x, float y) {
  x = x + y;
  y = x;
  return y;
}

// CHECK:   void func_grad(float x, float y, float *_result, double &_final_error) {
// CHECK-NEXT:     double _delta_x = 0;
// CHECK-NEXT:     clad::tape<float> _EERepl_x0 = {};
// CHECK-NEXT:     clad::push(_EERepl_x0, x);
// CHECK-NEXT:     double _delta_y = 0;
// CHECK-NEXT:     clad::tape<float> _EERepl_y0 = {};
// CHECK-NEXT:     clad::push(_EERepl_y0, y);
// CHECK-NEXT:     x = x + y;
// CHECK-NEXT:     clad::push(_EERepl_x0, x);
// CHECK-NEXT:     y = x;
// CHECK-NEXT:     clad::push(_EERepl_y0, y);
// CHECK-NEXT:     float func_return = y;
// CHECK-NEXT:     goto _label0;
// CHECK-NEXT:   _label0:
// CHECK-NEXT:     {
// CHECK-NEXT:         _result[1UL] += 1;
// CHECK-NEXT:         _delta_y += _result[1UL] * clad::back(_EERepl_y0) * {{.+}};
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         clad::pop(_EERepl_y0);
// CHECK-NEXT:         float _r_d1 = _result[1UL];
// CHECK-NEXT:         _result[0UL] += _r_d1;
// CHECK-NEXT:         _delta_x += _result[0UL] * clad::back(_EERepl_x0) * {{.+}};
// CHECK-NEXT:         _result[1UL] -= _r_d1;
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         clad::pop(_EERepl_x0);
// CHECK-NEXT:         float _r_d0 = _result[0UL];
// CHECK-NEXT:         _result[0UL] += _r_d0;
// CHECK-NEXT:         _delta_x += _result[0UL] * clad::back(_EERepl_x0) * {{.+}};
// CHECK-NEXT:         _result[1UL] += _r_d0;
// CHECK-NEXT:         _delta_y += _result[1UL] * clad::back(_EERepl_y0) * {{.+}};
// CHECK-NEXT:         _result[0UL] -= _r_d0;
// CHECK-NEXT:     }
// CHECK-NEXT:     _final_error = _delta_y + _delta_x;
// CHECK-NEXT: }

float func2(float x, int y){
  x = y * x;
  return x;
}

// CHECK: void func2_grad(float x, int y, float *_result, double &_final_error) {
// CHECK-NEXT:     double _delta_x = 0;
// CHECK-NEXT:     clad::tape<float> _EERepl_x0 = {};
// CHECK-NEXT:     clad::push(_EERepl_x0, x);
// CHECK-NEXT:     float _t0;
// CHECK-NEXT:     int _t1;
// CHECK-NEXT:     _t1 = y;
// CHECK-NEXT:     _t0 = x;
// CHECK-NEXT:     x = _t1 * _t0;
// CHECK-NEXT:     clad::push(_EERepl_x0, x);
// CHECK-NEXT:     float func2_return = x;
// CHECK-NEXT:     goto _label0;
// CHECK-NEXT:   _label0:
// CHECK-NEXT:     {
// CHECK-NEXT:         _result[0UL] += 1;
// CHECK-NEXT:         _delta_x += _result[0UL] * clad::back(_EERepl_x0) * {{.+}};
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         clad::pop(_EERepl_x0);
// CHECK-NEXT:         float _r_d0 = _result[0UL];
// CHECK-NEXT:         float _r0 = _r_d0 * _t0;
// CHECK-NEXT:         _result[1UL] += _r0;
// CHECK-NEXT:         float _r1 = _t1 * _r_d0;
// CHECK-NEXT:         _result[0UL] += _r1;
// CHECK-NEXT:         _delta_x += _result[0UL] * clad::back(_EERepl_x0) * {{.+}};
// CHECK-NEXT:         _result[0UL] -= _r_d0;
// CHECK-NEXT:     }
// CHECK-NEXT:     _final_error = _delta_x;
// CHECK-NEXT: }


float func3(int x, int y){
  x = y;
  return y;
}

// CHECK: void func3_grad(int x, int y, float *_result, double &_final_error) {
// CHECK-NEXT:     x = y;
// CHECK-NEXT:     int func3_return = y;
// CHECK-NEXT:     goto _label0;
// CHECK-NEXT:   _label0:
// CHECK-NEXT:     _result[1UL] += 1;
// CHECK-NEXT:     {
// CHECK-NEXT:         float _r_d0 = _result[0UL];
// CHECK-NEXT:         _result[1UL] += _r_d0;
// CHECK-NEXT:         _result[0UL] -= _r_d0;
// CHECK-NEXT:     }
// CHECK-NEXT: }


float func4(float x, float y){
  double z = y;
  x = z + y;
  return x;
}

// CHECK: void func4_grad(float x, float y, float *_result, double &_final_error) {
// CHECK-NEXT:     double _delta_y = 0;
// CHECK-NEXT:     clad::tape<float> _EERepl_y0 = {};
// CHECK-NEXT:     clad::push(_EERepl_y0, y);
// CHECK-NEXT:     double _delta_z = 0;
// CHECK-NEXT:     clad::tape<double> _EERepl_z0 = {};
// CHECK-NEXT:     double _d_z = 0;
// CHECK-NEXT:     double _delta_x = 0;
// CHECK-NEXT:     clad::tape<float> _EERepl_x0 = {};
// CHECK-NEXT:     clad::push(_EERepl_x0, x);
// CHECK-NEXT:     double z = y;
// CHECK-NEXT:     clad::push(_EERepl_z0, z);
// CHECK-NEXT:     x = z + y;
// CHECK-NEXT:     clad::push(_EERepl_x0, x);
// CHECK-NEXT:     float func4_return = x;
// CHECK-NEXT:     goto _label0;
// CHECK-NEXT:   _label0:
// CHECK-NEXT:     {
// CHECK-NEXT:         _result[0UL] += 1;
// CHECK-NEXT:         _delta_x += _result[0UL] * clad::back(_EERepl_x0) * {{.+}};
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         clad::pop(_EERepl_x0);
// CHECK-NEXT:         float _r_d0 = _result[0UL];
// CHECK-NEXT:         _d_z += _r_d0;
// CHECK-NEXT:         _delta_z += _d_z * clad::back(_EERepl_z0) * {{.+}};
// CHECK-NEXT:         _result[1UL] += _r_d0;
// CHECK-NEXT:         _delta_y += _result[1UL] * clad::back(_EERepl_y0) * {{.+}};
// CHECK-NEXT:         _result[0UL] -= _r_d0;
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         _result[1UL] += _d_z;
// CHECK-NEXT:         _delta_y += _result[1UL] * clad::back(_EERepl_y0) * {{.+}};
// CHECK-NEXT:     }
// CHECK-NEXT:     _final_error = _delta_x + _delta_y + _delta_z;
// CHECK-NEXT: }

float func5(float x, float y){
  int z = 56;
  x = z + y;
  return x;
}

// CHECK: void func5_grad(float x, float y, float *_result, double &_final_error) {
// CHECK-NEXT:     int _d_z = 0;
// CHECK-NEXT:     double _delta_x = 0;
// CHECK-NEXT:     clad::tape<float> _EERepl_x0 = {};
// CHECK-NEXT:     clad::push(_EERepl_x0, x);
// CHECK-NEXT:     double _delta_y = 0;
// CHECK-NEXT:     clad::tape<float> _EERepl_y0 = {};
// CHECK-NEXT:     clad::push(_EERepl_y0, y);
// CHECK-NEXT:     int z = 56;
// CHECK-NEXT:     x = z + y;
// CHECK-NEXT:     clad::push(_EERepl_x0, x);
// CHECK-NEXT:     float func5_return = x;
// CHECK-NEXT:     goto _label0;
// CHECK-NEXT:   _label0:
// CHECK-NEXT:     {
// CHECK-NEXT:         _result[0UL] += 1;
// CHECK-NEXT:         _delta_x += _result[0UL] * clad::back(_EERepl_x0) * {{.+}};
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         clad::pop(_EERepl_x0);
// CHECK-NEXT:         float _r_d0 = _result[0UL];
// CHECK-NEXT:         _d_z += _r_d0;
// CHECK-NEXT:         _result[1UL] += _r_d0;
// CHECK-NEXT:         _delta_y += _result[1UL] * clad::back(_EERepl_y0) * {{.+}};
// CHECK-NEXT:         _result[0UL] -= _r_d0;
// CHECK-NEXT:     }
// CHECK-NEXT:     _final_error = _delta_y + _delta_x;
// CHECK-NEXT: }


float func6(float x, float y){
  double z;
  z = 79.7;
  x = z + y;
  return z;
}

// CHECK: void func6_grad(float x, float y, float *_result, double &_final_error) {
// CHECK-NEXT:     double _delta_z = 0;
// CHECK-NEXT:     clad::tape<double> _EERepl_z0 = {};
// CHECK-NEXT:     double _d_z = 0;
// CHECK-NEXT:     double _delta_x = 0;
// CHECK-NEXT:     clad::tape<float> _EERepl_x0 = {};
// CHECK-NEXT:     clad::push(_EERepl_x0, x);
// CHECK-NEXT:     double _delta_y = 0;
// CHECK-NEXT:     clad::tape<float> _EERepl_y0 = {};
// CHECK-NEXT:     clad::push(_EERepl_y0, y);
// CHECK-NEXT:     double z;
// CHECK-NEXT:     clad::push(_EERepl_z0, z);
// CHECK-NEXT:     z = 79.700000000000002;
// CHECK-NEXT:     clad::push(_EERepl_z0, z);
// CHECK-NEXT:     x = z + y;
// CHECK-NEXT:     clad::push(_EERepl_x0, x);
// CHECK-NEXT:     double func6_return = z;
// CHECK-NEXT:     goto _label0;
// CHECK-NEXT:   _label0:
// CHECK-NEXT:     {
// CHECK-NEXT:         _d_z += 1;
// CHECK-NEXT:         _delta_z += _d_z * clad::back(_EERepl_z0) * {{.+}};
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         clad::pop(_EERepl_x0);
// CHECK-NEXT:         float _r_d1 = _result[0UL];
// CHECK-NEXT:         _d_z += _r_d1;
// CHECK-NEXT:         _delta_z += _d_z * clad::back(_EERepl_z0) * {{.+}};
// CHECK-NEXT:         _result[1UL] += _r_d1;
// CHECK-NEXT:         _delta_y += _result[1UL] * clad::back(_EERepl_y0) * {{.+}};
// CHECK-NEXT:         _result[0UL] -= _r_d1;
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         clad::pop(_EERepl_z0);
// CHECK-NEXT:         double _r_d0 = _d_z;
// CHECK-NEXT:         _d_z -= _r_d0;
// CHECK-NEXT:     }
// CHECK-NEXT:     _final_error = _delta_y + _delta_z + _delta_x;
// CHECK-NEXT: }

float func7(float x, float y){
  double z;
  z = x;
  x = z + y;
  return z;
}

// CHECK: void func7_grad(float x, float y, float *_result, double &_final_error) {
// CHECK-NEXT:     double _delta_z = 0;
// CHECK-NEXT:     clad::tape<double> _EERepl_z0 = {};
// CHECK-NEXT:     double _d_z = 0;
// CHECK-NEXT:     double _delta_x = 0;
// CHECK-NEXT:     clad::tape<float> _EERepl_x0 = {};
// CHECK-NEXT:     clad::push(_EERepl_x0, x);
// CHECK-NEXT:     double _delta_y = 0;
// CHECK-NEXT:     clad::tape<float> _EERepl_y0 = {};
// CHECK-NEXT:     clad::push(_EERepl_y0, y);
// CHECK-NEXT:     double z;
// CHECK-NEXT:     clad::push(_EERepl_z0, z);
// CHECK-NEXT:     z = x;
// CHECK-NEXT:     clad::push(_EERepl_z0, z);
// CHECK-NEXT:     x = z + y;
// CHECK-NEXT:     clad::push(_EERepl_x0, x);
// CHECK-NEXT:     double func7_return = z;
// CHECK-NEXT:     goto _label0;
// CHECK-NEXT:   _label0:
// CHECK-NEXT:     {
// CHECK-NEXT:         _d_z += 1;
// CHECK-NEXT:         _delta_z += _d_z * clad::back(_EERepl_z0) * {{.+}};
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         clad::pop(_EERepl_x0);
// CHECK-NEXT:         float _r_d1 = _result[0UL];
// CHECK-NEXT:         _d_z += _r_d1;
// CHECK-NEXT:         _delta_z += _d_z * clad::back(_EERepl_z0) * {{.+}};
// CHECK-NEXT:         _result[1UL] += _r_d1;
// CHECK-NEXT:         _delta_y += _result[1UL] * clad::back(_EERepl_y0) * {{.+}};
// CHECK-NEXT:         _result[0UL] -= _r_d1;
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         clad::pop(_EERepl_z0);
// CHECK-NEXT:         double _r_d0 = _d_z;
// CHECK-NEXT:         _result[0UL] += _r_d0;
// CHECK-NEXT:         _delta_x += _result[0UL] * clad::back(_EERepl_x0) * {{.+}};
// CHECK-NEXT:         _d_z -= _r_d0;
// CHECK-NEXT:     }
// CHECK-NEXT:     _final_error = _delta_y + _delta_z + _delta_x;
// CHECK-NEXT: }

float func8(float x, float y){
  return x;
}

//CEHCK: void func8_grad(float x, float y, float *_result, double &_final_error) {
//CEHCK-NEXT:     double _delta_x = 0;
//CEHCK-NEXT:     clad::tape<float> _EERepl_x0 = {};
//CEHCK-NEXT:     clad::push(_EERepl_x0, x);
//CEHCK-NEXT:     float func8_return = x;
//CEHCK-NEXT:     goto _label0;
//CEHCK-NEXT:   _label0:
//CEHCK-NEXT:     {
//CEHCK-NEXT:         _result[0UL] += 1;
//CEHCK-NEXT:         _delta_x += _result[0UL] * clad::back(_EERepl_x0) * {{.+}};
//CEHCK-NEXT:     }
//CEHCK-NEXT:     _final_error = _delta_x;
//CEHCK-NEXT: }


float func9(float x, float y){
  return (x = y);
}

// CHECK: void func9_grad(float x, float y, float *_result, double &_final_error) {
// CHECK-NEXT:     double _delta_x = 0;
// CHECK-NEXT:     clad::tape<float> _EERepl_x0 = {};
// CHECK-NEXT:     clad::push(_EERepl_x0, x);
// CHECK-NEXT:     double _delta_y = 0;
// CHECK-NEXT:     clad::tape<float> _EERepl_y0 = {};
// CHECK-NEXT:     clad::push(_EERepl_y0, y);
// CHECK-NEXT:     clad::push(_EERepl_x0, x);
// CHECK-NEXT:     float func9_return = (x = y);
// CHECK-NEXT:     goto _label0;
// CHECK-NEXT:   _label0:
// CHECK-NEXT:     {
// CHECK-NEXT:         clad::pop(_EERepl_x0);
// CHECK-NEXT:         _result[0UL] += 1;
// CHECK-NEXT:         float _r_d0 = _result[0UL];
// CHECK-NEXT:         _result[1UL] += _r_d0;
// CHECK-NEXT:         _delta_y += _result[1UL] * clad::back(_EERepl_y0) * {{.+}};
// CHECK-NEXT:         _result[0UL] -= _r_d0;
// CHECK-NEXT:     }
// CHECK-NEXT:     _final_error = _delta_y + _delta_x;
// CHECK-NEXT: }


int main(){

  clad::error_estimation(func);
  clad::error_estimation(func2);
  clad::error_estimation(func3);
  clad::error_estimation(func4);
  clad::error_estimation(func5);
  clad::error_estimation(func6);
  clad::error_estimation(func7);
  clad::error_estimation(func8);
  clad::error_estimation(func9);
}