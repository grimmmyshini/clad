// RUN: %cladclang %s -lm -I%S/../../include -oMemberFunctions.out 2>&1 | FileCheck %s
// RUN: ./MemberFunctions.out | FileCheck -check-prefix=CHECK-EXEC %s
// RUN: %cladclang -std=c++14 %s -lm -I%S/../../include -oMemberFunctions-cpp14.out 2>&1 | FileCheck %s
// RUN: ./MemberFunctions-cpp14.out | FileCheck -check-prefix=CHECK-EXEC %s
// RUN: %cladclang -std=c++17 %s -lm -I%S/../../include -oMemberFunctions-cpp17.out 2>&1 | FileCheck %s
// RUN: ./MemberFunctions-cpp17.out | FileCheck -check-prefix=CHECK-EXEC %s

//CHECK-NOT: {{.*error|warning|note:.*}}
#include "clad/Differentiator/Differentiator.h"

class SimpleFunctions {
public:
  SimpleFunctions(double p_x = 0, double p_y = 0) : x(p_x), y(p_y) {}
  double x, y;
  double mem_fn(double i, double j)  { 
    return (x+y)*i + i*j; 
  } 

  // CHECK: void mem_fn_grad(double i, double j, double *_result) {
  // CHECK-NEXT:     double _t0;
  // CHECK-NEXT:     double _t1;
  // CHECK-NEXT:     double _t2;
  // CHECK-NEXT:     double _t3;
  // CHECK-NEXT:     _t1 = (this->x + this->y);
  // CHECK-NEXT:     _t0 = i;
  // CHECK-NEXT:     _t3 = i;
  // CHECK-NEXT:     _t2 = j;
  // CHECK-NEXT:     double mem_fn_return = _t1 * _t0 + _t3 * _t2;
  // CHECK-NEXT:     goto _label0;
  // CHECK-NEXT:   _label0:
  // CHECK-NEXT:     {
  // CHECK-NEXT:         double _r0 = 1 * _t0;
  // CHECK-NEXT:         double _r1 = _t1 * 1;
  // CHECK-NEXT:         _result[0UL] += _r1;
  // CHECK-NEXT:         double _r2 = 1 * _t2;
  // CHECK-NEXT:         _result[0UL] += _r2;
  // CHECK-NEXT:         double _r3 = _t3 * 1;
  // CHECK-NEXT:         _result[1UL] += _r3;
  // CHECK-NEXT:     }
  // CHECK-NEXT: }

  double const_mem_fn(double i, double j) const { 
    return (x+y)*i + i*j; 
  } 

  // CHECK: void const_mem_fn_grad(double i, double j, double *_result) {
  // CHECK-NEXT:     double _t0;
  // CHECK-NEXT:     double _t1;
  // CHECK-NEXT:     double _t2;
  // CHECK-NEXT:     double _t3;
  // CHECK-NEXT:     _t1 = (this->x + this->y);
  // CHECK-NEXT:     _t0 = i;
  // CHECK-NEXT:     _t3 = i;
  // CHECK-NEXT:     _t2 = j;
  // CHECK-NEXT:     double const_mem_fn_return = _t1 * _t0 + _t3 * _t2;
  // CHECK-NEXT:     goto _label0;
  // CHECK-NEXT:   _label0:
  // CHECK-NEXT:     {
  // CHECK-NEXT:         double _r0 = 1 * _t0;
  // CHECK-NEXT:         double _r1 = _t1 * 1;
  // CHECK-NEXT:         _result[0UL] += _r1;
  // CHECK-NEXT:         double _r2 = 1 * _t2;
  // CHECK-NEXT:         _result[0UL] += _r2;
  // CHECK-NEXT:         double _r3 = _t3 * 1;
  // CHECK-NEXT:         _result[1UL] += _r3;
  // CHECK-NEXT:     }
  // CHECK-NEXT: }

  double volatile_mem_fn(double i, double j) volatile { 
    return (x+y)*i + i*j; 
  } 

  // CHECK: void volatile_mem_fn_grad(double i, double j, double *_result) {
  // CHECK-NEXT:     double _t0;
  // CHECK-NEXT:     double _t1;
  // CHECK-NEXT:     double _t2;
  // CHECK-NEXT:     double _t3;
  // CHECK-NEXT:     _t1 = (this->x + this->y);
  // CHECK-NEXT:     _t0 = i;
  // CHECK-NEXT:     _t3 = i;
  // CHECK-NEXT:     _t2 = j;
  // CHECK-NEXT:     double volatile_mem_fn_return = _t1 * _t0 + _t3 * _t2;
  // CHECK-NEXT:     goto _label0;
  // CHECK-NEXT:   _label0:
  // CHECK-NEXT:     {
  // CHECK-NEXT:         double _r0 = 1 * _t0;
  // CHECK-NEXT:         double _r1 = _t1 * 1;
  // CHECK-NEXT:         _result[0UL] += _r1;
  // CHECK-NEXT:         double _r2 = 1 * _t2;
  // CHECK-NEXT:         _result[0UL] += _r2;
  // CHECK-NEXT:         double _r3 = _t3 * 1;
  // CHECK-NEXT:         _result[1UL] += _r3;
  // CHECK-NEXT:     }
  // CHECK-NEXT: }

  double const_volatile_mem_fn(double i, double j) const volatile { 
    return (x+y)*i + i*j; 
  } 

  // CHECK: void const_volatile_mem_fn_grad(double i, double j, double *_result) {
  // CHECK-NEXT:     double _t0;
  // CHECK-NEXT:     double _t1;
  // CHECK-NEXT:     double _t2;
  // CHECK-NEXT:     double _t3;
  // CHECK-NEXT:     _t1 = (this->x + this->y);
  // CHECK-NEXT:     _t0 = i;
  // CHECK-NEXT:     _t3 = i;
  // CHECK-NEXT:     _t2 = j;
  // CHECK-NEXT:     double const_volatile_mem_fn_return = _t1 * _t0 + _t3 * _t2;
  // CHECK-NEXT:     goto _label0;
  // CHECK-NEXT:   _label0:
  // CHECK-NEXT:     {
  // CHECK-NEXT:         double _r0 = 1 * _t0;
  // CHECK-NEXT:         double _r1 = _t1 * 1;
  // CHECK-NEXT:         _result[0UL] += _r1;
  // CHECK-NEXT:         double _r2 = 1 * _t2;
  // CHECK-NEXT:         _result[0UL] += _r2;
  // CHECK-NEXT:         double _r3 = _t3 * 1;
  // CHECK-NEXT:         _result[1UL] += _r3;
  // CHECK-NEXT:     }
  // CHECK-NEXT: }

  double lval_ref_mem_fn(double i, double j) & { 
    return (x+y)*i + i*j; 
  } 

  // CHECK: void lval_ref_mem_fn_grad(double i, double j, double *_result) {
  // CHECK-NEXT:     double _t0;
  // CHECK-NEXT:     double _t1;
  // CHECK-NEXT:     double _t2;
  // CHECK-NEXT:     double _t3;
  // CHECK-NEXT:     _t1 = (this->x + this->y);
  // CHECK-NEXT:     _t0 = i;
  // CHECK-NEXT:     _t3 = i;
  // CHECK-NEXT:     _t2 = j;
  // CHECK-NEXT:     double lval_ref_mem_fn_return = _t1 * _t0 + _t3 * _t2;
  // CHECK-NEXT:     goto _label0;
  // CHECK-NEXT:   _label0:
  // CHECK-NEXT:     {
  // CHECK-NEXT:         double _r0 = 1 * _t0;
  // CHECK-NEXT:         double _r1 = _t1 * 1;
  // CHECK-NEXT:         _result[0UL] += _r1;
  // CHECK-NEXT:         double _r2 = 1 * _t2;
  // CHECK-NEXT:         _result[0UL] += _r2;
  // CHECK-NEXT:         double _r3 = _t3 * 1;
  // CHECK-NEXT:         _result[1UL] += _r3;
  // CHECK-NEXT:     }
  // CHECK-NEXT: }

  double const_lval_ref_mem_fn(double i, double j) const & { 
    return (x+y)*i + i*j; 
  } 

  // CHECK: void const_lval_ref_mem_fn_grad(double i, double j, double *_result) {
  // CHECK-NEXT:     double _t0;
  // CHECK-NEXT:     double _t1;
  // CHECK-NEXT:     double _t2;
  // CHECK-NEXT:     double _t3;
  // CHECK-NEXT:     _t1 = (this->x + this->y);
  // CHECK-NEXT:     _t0 = i;
  // CHECK-NEXT:     _t3 = i;
  // CHECK-NEXT:     _t2 = j;
  // CHECK-NEXT:     double const_lval_ref_mem_fn_return = _t1 * _t0 + _t3 * _t2;
  // CHECK-NEXT:     goto _label0;
  // CHECK-NEXT:   _label0:
  // CHECK-NEXT:     {
  // CHECK-NEXT:         double _r0 = 1 * _t0;
  // CHECK-NEXT:         double _r1 = _t1 * 1;
  // CHECK-NEXT:         _result[0UL] += _r1;
  // CHECK-NEXT:         double _r2 = 1 * _t2;
  // CHECK-NEXT:         _result[0UL] += _r2;
  // CHECK-NEXT:         double _r3 = _t3 * 1;
  // CHECK-NEXT:         _result[1UL] += _r3;
  // CHECK-NEXT:     }
  // CHECK-NEXT: }

  double volatile_lval_ref_mem_fn(double i, double j) volatile & { 
    return (x+y)*i + i*j; 
  } 

  // CHECK: void volatile_lval_ref_mem_fn_grad(double i, double j, double *_result) {
  // CHECK-NEXT:     double _t0;
  // CHECK-NEXT:     double _t1;
  // CHECK-NEXT:     double _t2;
  // CHECK-NEXT:     double _t3;
  // CHECK-NEXT:     _t1 = (this->x + this->y);
  // CHECK-NEXT:     _t0 = i;
  // CHECK-NEXT:     _t3 = i;
  // CHECK-NEXT:     _t2 = j;
  // CHECK-NEXT:     double volatile_lval_ref_mem_fn_return = _t1 * _t0 + _t3 * _t2;
  // CHECK-NEXT:     goto _label0;
  // CHECK-NEXT:   _label0:
  // CHECK-NEXT:     {
  // CHECK-NEXT:         double _r0 = 1 * _t0;
  // CHECK-NEXT:         double _r1 = _t1 * 1;
  // CHECK-NEXT:         _result[0UL] += _r1;
  // CHECK-NEXT:         double _r2 = 1 * _t2;
  // CHECK-NEXT:         _result[0UL] += _r2;
  // CHECK-NEXT:         double _r3 = _t3 * 1;
  // CHECK-NEXT:         _result[1UL] += _r3;
  // CHECK-NEXT:     }
  // CHECK-NEXT: }

  double const_volatile_lval_ref_mem_fn(double i, double j) const volatile & { 
    return (x+y)*i + i*j; 
  } 

  // CHECK: void const_volatile_lval_ref_mem_fn_grad(double i, double j, double *_result) {
  // CHECK-NEXT:     double _t0;
  // CHECK-NEXT:     double _t1;
  // CHECK-NEXT:     double _t2;
  // CHECK-NEXT:     double _t3;
  // CHECK-NEXT:     _t1 = (this->x + this->y);
  // CHECK-NEXT:     _t0 = i;
  // CHECK-NEXT:     _t3 = i;
  // CHECK-NEXT:     _t2 = j;
  // CHECK-NEXT:     double const_volatile_lval_ref_mem_fn_return = _t1 * _t0 + _t3 * _t2;
  // CHECK-NEXT:     goto _label0;
  // CHECK-NEXT:   _label0:
  // CHECK-NEXT:     {
  // CHECK-NEXT:         double _r0 = 1 * _t0;
  // CHECK-NEXT:         double _r1 = _t1 * 1;
  // CHECK-NEXT:         _result[0UL] += _r1;
  // CHECK-NEXT:         double _r2 = 1 * _t2;
  // CHECK-NEXT:         _result[0UL] += _r2;
  // CHECK-NEXT:         double _r3 = _t3 * 1;
  // CHECK-NEXT:         _result[1UL] += _r3;
  // CHECK-NEXT:     }
  // CHECK-NEXT: }

  double rval_ref_mem_fn(double i, double j) && { 
    return (x+y)*i + i*j; 
  } 

  // CHECK: void rval_ref_mem_fn_grad(double i, double j, double *_result) {
  // CHECK-NEXT:     double _t0;
  // CHECK-NEXT:     double _t1;
  // CHECK-NEXT:     double _t2;
  // CHECK-NEXT:     double _t3;
  // CHECK-NEXT:     _t1 = (this->x + this->y);
  // CHECK-NEXT:     _t0 = i;
  // CHECK-NEXT:     _t3 = i;
  // CHECK-NEXT:     _t2 = j;
  // CHECK-NEXT:     double rval_ref_mem_fn_return = _t1 * _t0 + _t3 * _t2;
  // CHECK-NEXT:     goto _label0;
  // CHECK-NEXT:   _label0:
  // CHECK-NEXT:     {
  // CHECK-NEXT:         double _r0 = 1 * _t0;
  // CHECK-NEXT:         double _r1 = _t1 * 1;
  // CHECK-NEXT:         _result[0UL] += _r1;
  // CHECK-NEXT:         double _r2 = 1 * _t2;
  // CHECK-NEXT:         _result[0UL] += _r2;
  // CHECK-NEXT:         double _r3 = _t3 * 1;
  // CHECK-NEXT:         _result[1UL] += _r3;
  // CHECK-NEXT:     }
  // CHECK-NEXT: }

  double const_rval_ref_mem_fn(double i, double j) const && { 
    return (x+y)*i + i*j; 
  } 

  // CHECK: void const_rval_ref_mem_fn_grad(double i, double j, double *_result) {
  // CHECK-NEXT:     double _t0;
  // CHECK-NEXT:     double _t1;
  // CHECK-NEXT:     double _t2;
  // CHECK-NEXT:     double _t3;
  // CHECK-NEXT:     _t1 = (this->x + this->y);
  // CHECK-NEXT:     _t0 = i;
  // CHECK-NEXT:     _t3 = i;
  // CHECK-NEXT:     _t2 = j;
  // CHECK-NEXT:     double const_rval_ref_mem_fn_return = _t1 * _t0 + _t3 * _t2;
  // CHECK-NEXT:     goto _label0;
  // CHECK-NEXT:   _label0:
  // CHECK-NEXT:     {
  // CHECK-NEXT:         double _r0 = 1 * _t0;
  // CHECK-NEXT:         double _r1 = _t1 * 1;
  // CHECK-NEXT:         _result[0UL] += _r1;
  // CHECK-NEXT:         double _r2 = 1 * _t2;
  // CHECK-NEXT:         _result[0UL] += _r2;
  // CHECK-NEXT:         double _r3 = _t3 * 1;
  // CHECK-NEXT:         _result[1UL] += _r3;
  // CHECK-NEXT:     }
  // CHECK-NEXT: }

  double volatile_rval_ref_mem_fn(double i, double j) volatile && { 
    return (x+y)*i + i*j; 
  } 

  // CHECK: void volatile_rval_ref_mem_fn_grad(double i, double j, double *_result) {
  // CHECK-NEXT:     double _t0;
  // CHECK-NEXT:     double _t1;
  // CHECK-NEXT:     double _t2;
  // CHECK-NEXT:     double _t3;
  // CHECK-NEXT:     _t1 = (this->x + this->y);
  // CHECK-NEXT:     _t0 = i;
  // CHECK-NEXT:     _t3 = i;
  // CHECK-NEXT:     _t2 = j;
  // CHECK-NEXT:     double volatile_rval_ref_mem_fn_return = _t1 * _t0 + _t3 * _t2;
  // CHECK-NEXT:     goto _label0;
  // CHECK-NEXT:   _label0:
  // CHECK-NEXT:     {
  // CHECK-NEXT:         double _r0 = 1 * _t0;
  // CHECK-NEXT:         double _r1 = _t1 * 1;
  // CHECK-NEXT:         _result[0UL] += _r1;
  // CHECK-NEXT:         double _r2 = 1 * _t2;
  // CHECK-NEXT:         _result[0UL] += _r2;
  // CHECK-NEXT:         double _r3 = _t3 * 1;
  // CHECK-NEXT:         _result[1UL] += _r3;
  // CHECK-NEXT:     }
  // CHECK-NEXT: }

  double const_volatile_rval_ref_mem_fn(double i, double j) const volatile && { 
    return (x+y)*i + i*j; 
  } 

  // CHECK: void const_volatile_rval_ref_mem_fn_grad(double i, double j, double *_result) {
  // CHECK-NEXT:     double _t0;
  // CHECK-NEXT:     double _t1;
  // CHECK-NEXT:     double _t2;
  // CHECK-NEXT:     double _t3;
  // CHECK-NEXT:     _t1 = (this->x + this->y);
  // CHECK-NEXT:     _t0 = i;
  // CHECK-NEXT:     _t3 = i;
  // CHECK-NEXT:     _t2 = j;
  // CHECK-NEXT:     double const_volatile_rval_ref_mem_fn_return = _t1 * _t0 + _t3 * _t2;
  // CHECK-NEXT:     goto _label0;
  // CHECK-NEXT:   _label0:
  // CHECK-NEXT:     {
  // CHECK-NEXT:         double _r0 = 1 * _t0;
  // CHECK-NEXT:         double _r1 = _t1 * 1;
  // CHECK-NEXT:         _result[0UL] += _r1;
  // CHECK-NEXT:         double _r2 = 1 * _t2;
  // CHECK-NEXT:         _result[0UL] += _r2;
  // CHECK-NEXT:         double _r3 = _t3 * 1;
  // CHECK-NEXT:         _result[1UL] += _r3;
  // CHECK-NEXT:     }
  // CHECK-NEXT: }

  double noexcept_mem_fn(double i, double j) noexcept { 
    return (x+y)*i + i*j; 
  } 

  // CHECK: void noexcept_mem_fn_grad(double i, double j, double *_result) {
  // CHECK-NEXT:     double _t0;
  // CHECK-NEXT:     double _t1;
  // CHECK-NEXT:     double _t2;
  // CHECK-NEXT:     double _t3;
  // CHECK-NEXT:     _t1 = (this->x + this->y);
  // CHECK-NEXT:     _t0 = i;
  // CHECK-NEXT:     _t3 = i;
  // CHECK-NEXT:     _t2 = j;
  // CHECK-NEXT:     double noexcept_mem_fn_return = _t1 * _t0 + _t3 * _t2;
  // CHECK-NEXT:     goto _label0;
  // CHECK-NEXT:   _label0:
  // CHECK-NEXT:     {
  // CHECK-NEXT:         double _r0 = 1 * _t0;
  // CHECK-NEXT:         double _r1 = _t1 * 1;
  // CHECK-NEXT:         _result[0UL] += _r1;
  // CHECK-NEXT:         double _r2 = 1 * _t2;
  // CHECK-NEXT:         _result[0UL] += _r2;
  // CHECK-NEXT:         double _r3 = _t3 * 1;
  // CHECK-NEXT:         _result[1UL] += _r3;
  // CHECK-NEXT:     }
  // CHECK-NEXT: }

  double const_noexcept_mem_fn(double i, double j) const noexcept { 
    return (x+y)*i + i*j; 
  } 

  // CHECK: void const_noexcept_mem_fn_grad(double i, double j, double *_result) {
  // CHECK-NEXT:     double _t0;
  // CHECK-NEXT:     double _t1;
  // CHECK-NEXT:     double _t2;
  // CHECK-NEXT:     double _t3;
  // CHECK-NEXT:     _t1 = (this->x + this->y);
  // CHECK-NEXT:     _t0 = i;
  // CHECK-NEXT:     _t3 = i;
  // CHECK-NEXT:     _t2 = j;
  // CHECK-NEXT:     double const_noexcept_mem_fn_return = _t1 * _t0 + _t3 * _t2;
  // CHECK-NEXT:     goto _label0;
  // CHECK-NEXT:   _label0:
  // CHECK-NEXT:     {
  // CHECK-NEXT:         double _r0 = 1 * _t0;
  // CHECK-NEXT:         double _r1 = _t1 * 1;
  // CHECK-NEXT:         _result[0UL] += _r1;
  // CHECK-NEXT:         double _r2 = 1 * _t2;
  // CHECK-NEXT:         _result[0UL] += _r2;
  // CHECK-NEXT:         double _r3 = _t3 * 1;
  // CHECK-NEXT:         _result[1UL] += _r3;
  // CHECK-NEXT:     }
  // CHECK-NEXT: }

  double volatile_noexcept_mem_fn(double i, double j) volatile noexcept { 
    return (x+y)*i + i*j; 
  } 

  // CHECK: void volatile_noexcept_mem_fn_grad(double i, double j, double *_result) {
  // CHECK-NEXT:     double _t0;
  // CHECK-NEXT:     double _t1;
  // CHECK-NEXT:     double _t2;
  // CHECK-NEXT:     double _t3;
  // CHECK-NEXT:     _t1 = (this->x + this->y);
  // CHECK-NEXT:     _t0 = i;
  // CHECK-NEXT:     _t3 = i;
  // CHECK-NEXT:     _t2 = j;
  // CHECK-NEXT:     double volatile_noexcept_mem_fn_return = _t1 * _t0 + _t3 * _t2;
  // CHECK-NEXT:     goto _label0;
  // CHECK-NEXT:   _label0:
  // CHECK-NEXT:     {
  // CHECK-NEXT:         double _r0 = 1 * _t0;
  // CHECK-NEXT:         double _r1 = _t1 * 1;
  // CHECK-NEXT:         _result[0UL] += _r1;
  // CHECK-NEXT:         double _r2 = 1 * _t2;
  // CHECK-NEXT:         _result[0UL] += _r2;
  // CHECK-NEXT:         double _r3 = _t3 * 1;
  // CHECK-NEXT:         _result[1UL] += _r3;
  // CHECK-NEXT:     }
  // CHECK-NEXT: }

  double const_volatile_noexcept_mem_fn(double i, double j) const volatile noexcept { 
    return (x+y)*i + i*j; 
  } 

  // CHECK: void const_volatile_noexcept_mem_fn_grad(double i, double j, double *_result) {
  // CHECK-NEXT:     double _t0;
  // CHECK-NEXT:     double _t1;
  // CHECK-NEXT:     double _t2;
  // CHECK-NEXT:     double _t3;
  // CHECK-NEXT:     _t1 = (this->x + this->y);
  // CHECK-NEXT:     _t0 = i;
  // CHECK-NEXT:     _t3 = i;
  // CHECK-NEXT:     _t2 = j;
  // CHECK-NEXT:     double const_volatile_noexcept_mem_fn_return = _t1 * _t0 + _t3 * _t2;
  // CHECK-NEXT:     goto _label0;
  // CHECK-NEXT:   _label0:
  // CHECK-NEXT:     {
  // CHECK-NEXT:         double _r0 = 1 * _t0;
  // CHECK-NEXT:         double _r1 = _t1 * 1;
  // CHECK-NEXT:         _result[0UL] += _r1;
  // CHECK-NEXT:         double _r2 = 1 * _t2;
  // CHECK-NEXT:         _result[0UL] += _r2;
  // CHECK-NEXT:         double _r3 = _t3 * 1;
  // CHECK-NEXT:         _result[1UL] += _r3;
  // CHECK-NEXT:     }
  // CHECK-NEXT: }

  double lval_ref_noexcept_mem_fn(double i, double j) & noexcept { 
    return (x+y)*i + i*j; 
  } 

  // CHECK: void lval_ref_noexcept_mem_fn_grad(double i, double j, double *_result) {
  // CHECK-NEXT:     double _t0;
  // CHECK-NEXT:     double _t1;
  // CHECK-NEXT:     double _t2;
  // CHECK-NEXT:     double _t3;
  // CHECK-NEXT:     _t1 = (this->x + this->y);
  // CHECK-NEXT:     _t0 = i;
  // CHECK-NEXT:     _t3 = i;
  // CHECK-NEXT:     _t2 = j;
  // CHECK-NEXT:     double lval_ref_noexcept_mem_fn_return = _t1 * _t0 + _t3 * _t2;
  // CHECK-NEXT:     goto _label0;
  // CHECK-NEXT:   _label0:
  // CHECK-NEXT:     {
  // CHECK-NEXT:         double _r0 = 1 * _t0;
  // CHECK-NEXT:         double _r1 = _t1 * 1;
  // CHECK-NEXT:         _result[0UL] += _r1;
  // CHECK-NEXT:         double _r2 = 1 * _t2;
  // CHECK-NEXT:         _result[0UL] += _r2;
  // CHECK-NEXT:         double _r3 = _t3 * 1;
  // CHECK-NEXT:         _result[1UL] += _r3;
  // CHECK-NEXT:     }
  // CHECK-NEXT: }

  double const_lval_ref_noexcept_mem_fn(double i, double j) const & noexcept { 
    return (x+y)*i + i*j; 
  } 

  // CHECK: void const_lval_ref_noexcept_mem_fn_grad(double i, double j, double *_result) {
  // CHECK-NEXT:     double _t0;
  // CHECK-NEXT:     double _t1;
  // CHECK-NEXT:     double _t2;
  // CHECK-NEXT:     double _t3;
  // CHECK-NEXT:     _t1 = (this->x + this->y);
  // CHECK-NEXT:     _t0 = i;
  // CHECK-NEXT:     _t3 = i;
  // CHECK-NEXT:     _t2 = j;
  // CHECK-NEXT:     double const_lval_ref_noexcept_mem_fn_return = _t1 * _t0 + _t3 * _t2;
  // CHECK-NEXT:     goto _label0;
  // CHECK-NEXT:   _label0:
  // CHECK-NEXT:     {
  // CHECK-NEXT:         double _r0 = 1 * _t0;
  // CHECK-NEXT:         double _r1 = _t1 * 1;
  // CHECK-NEXT:         _result[0UL] += _r1;
  // CHECK-NEXT:         double _r2 = 1 * _t2;
  // CHECK-NEXT:         _result[0UL] += _r2;
  // CHECK-NEXT:         double _r3 = _t3 * 1;
  // CHECK-NEXT:         _result[1UL] += _r3;
  // CHECK-NEXT:     }
  // CHECK-NEXT: }

  double volatile_lval_ref_noexcept_mem_fn(double i, double j) volatile & noexcept { 
    return (x+y)*i + i*j; 
  } 

  // CHECK: void volatile_lval_ref_noexcept_mem_fn_grad(double i, double j, double *_result) {
  // CHECK-NEXT:     double _t0;
  // CHECK-NEXT:     double _t1;
  // CHECK-NEXT:     double _t2;
  // CHECK-NEXT:     double _t3;
  // CHECK-NEXT:     _t1 = (this->x + this->y);
  // CHECK-NEXT:     _t0 = i;
  // CHECK-NEXT:     _t3 = i;
  // CHECK-NEXT:     _t2 = j;
  // CHECK-NEXT:     double volatile_lval_ref_noexcept_mem_fn_return = _t1 * _t0 + _t3 * _t2;
  // CHECK-NEXT:     goto _label0;
  // CHECK-NEXT:   _label0:
  // CHECK-NEXT:     {
  // CHECK-NEXT:         double _r0 = 1 * _t0;
  // CHECK-NEXT:         double _r1 = _t1 * 1;
  // CHECK-NEXT:         _result[0UL] += _r1;
  // CHECK-NEXT:         double _r2 = 1 * _t2;
  // CHECK-NEXT:         _result[0UL] += _r2;
  // CHECK-NEXT:         double _r3 = _t3 * 1;
  // CHECK-NEXT:         _result[1UL] += _r3;
  // CHECK-NEXT:     }
  // CHECK-NEXT: }

  double const_volatile_lval_ref_noexcept_mem_fn(double i, double j) const volatile & noexcept { 
    return (x+y)*i + i*j; 
  } 

  // CHECK: void const_volatile_lval_ref_noexcept_mem_fn_grad(double i, double j, double *_result) {
  // CHECK-NEXT:     double _t0;
  // CHECK-NEXT:     double _t1;
  // CHECK-NEXT:     double _t2;
  // CHECK-NEXT:     double _t3;
  // CHECK-NEXT:     _t1 = (this->x + this->y);
  // CHECK-NEXT:     _t0 = i;
  // CHECK-NEXT:     _t3 = i;
  // CHECK-NEXT:     _t2 = j;
  // CHECK-NEXT:     double const_volatile_lval_ref_noexcept_mem_fn_return = _t1 * _t0 + _t3 * _t2;
  // CHECK-NEXT:     goto _label0;
  // CHECK-NEXT:   _label0:
  // CHECK-NEXT:     {
  // CHECK-NEXT:         double _r0 = 1 * _t0;
  // CHECK-NEXT:         double _r1 = _t1 * 1;
  // CHECK-NEXT:         _result[0UL] += _r1;
  // CHECK-NEXT:         double _r2 = 1 * _t2;
  // CHECK-NEXT:         _result[0UL] += _r2;
  // CHECK-NEXT:         double _r3 = _t3 * 1;
  // CHECK-NEXT:         _result[1UL] += _r3;
  // CHECK-NEXT:     }
  // CHECK-NEXT: }

  double rval_ref_noexcept_mem_fn(double i, double j) && noexcept { 
    return (x+y)*i + i*j; 
  } 

  // CHECK: void rval_ref_noexcept_mem_fn_grad(double i, double j, double *_result) {
  // CHECK-NEXT:     double _t0;
  // CHECK-NEXT:     double _t1;
  // CHECK-NEXT:     double _t2;
  // CHECK-NEXT:     double _t3;
  // CHECK-NEXT:     _t1 = (this->x + this->y);
  // CHECK-NEXT:     _t0 = i;
  // CHECK-NEXT:     _t3 = i;
  // CHECK-NEXT:     _t2 = j;
  // CHECK-NEXT:     double rval_ref_noexcept_mem_fn_return = _t1 * _t0 + _t3 * _t2;
  // CHECK-NEXT:     goto _label0;
  // CHECK-NEXT:   _label0:
  // CHECK-NEXT:     {
  // CHECK-NEXT:         double _r0 = 1 * _t0;
  // CHECK-NEXT:         double _r1 = _t1 * 1;
  // CHECK-NEXT:         _result[0UL] += _r1;
  // CHECK-NEXT:         double _r2 = 1 * _t2;
  // CHECK-NEXT:         _result[0UL] += _r2;
  // CHECK-NEXT:         double _r3 = _t3 * 1;
  // CHECK-NEXT:         _result[1UL] += _r3;
  // CHECK-NEXT:     }
  // CHECK-NEXT: }

  double const_rval_ref_noexcept_mem_fn(double i, double j) const && noexcept { 
    return (x+y)*i + i*j; 
  } 

  // CHECK: void const_rval_ref_noexcept_mem_fn_grad(double i, double j, double *_result) {
  // CHECK-NEXT:     double _t0;
  // CHECK-NEXT:     double _t1;
  // CHECK-NEXT:     double _t2;
  // CHECK-NEXT:     double _t3;
  // CHECK-NEXT:     _t1 = (this->x + this->y);
  // CHECK-NEXT:     _t0 = i;
  // CHECK-NEXT:     _t3 = i;
  // CHECK-NEXT:     _t2 = j;
  // CHECK-NEXT:     double const_rval_ref_noexcept_mem_fn_return = _t1 * _t0 + _t3 * _t2;
  // CHECK-NEXT:     goto _label0;
  // CHECK-NEXT:   _label0:
  // CHECK-NEXT:     {
  // CHECK-NEXT:         double _r0 = 1 * _t0;
  // CHECK-NEXT:         double _r1 = _t1 * 1;
  // CHECK-NEXT:         _result[0UL] += _r1;
  // CHECK-NEXT:         double _r2 = 1 * _t2;
  // CHECK-NEXT:         _result[0UL] += _r2;
  // CHECK-NEXT:         double _r3 = _t3 * 1;
  // CHECK-NEXT:         _result[1UL] += _r3;
  // CHECK-NEXT:     }
  // CHECK-NEXT: }

  double volatile_rval_ref_noexcept_mem_fn(double i, double j) volatile && noexcept { 
    return (x+y)*i + i*j; 
  } 

  // CHECK: void volatile_rval_ref_noexcept_mem_fn_grad(double i, double j, double *_result) {
  // CHECK-NEXT:     double _t0;
  // CHECK-NEXT:     double _t1;
  // CHECK-NEXT:     double _t2;
  // CHECK-NEXT:     double _t3;
  // CHECK-NEXT:     _t1 = (this->x + this->y);
  // CHECK-NEXT:     _t0 = i;
  // CHECK-NEXT:     _t3 = i;
  // CHECK-NEXT:     _t2 = j;
  // CHECK-NEXT:     double volatile_rval_ref_noexcept_mem_fn_return = _t1 * _t0 + _t3 * _t2;
  // CHECK-NEXT:     goto _label0;
  // CHECK-NEXT:   _label0:
  // CHECK-NEXT:     {
  // CHECK-NEXT:         double _r0 = 1 * _t0;
  // CHECK-NEXT:         double _r1 = _t1 * 1;
  // CHECK-NEXT:         _result[0UL] += _r1;
  // CHECK-NEXT:         double _r2 = 1 * _t2;
  // CHECK-NEXT:         _result[0UL] += _r2;
  // CHECK-NEXT:         double _r3 = _t3 * 1;
  // CHECK-NEXT:         _result[1UL] += _r3;
  // CHECK-NEXT:     }
  // CHECK-NEXT: }

  double const_volatile_rval_ref_noexcept_mem_fn(double i, double j) const volatile && noexcept { 
    return (x+y)*i + i*j; 
  } 

  // CHECK: void const_volatile_rval_ref_noexcept_mem_fn_grad(double i, double j, double *_result) {
  // CHECK-NEXT:     double _t0;
  // CHECK-NEXT:     double _t1;
  // CHECK-NEXT:     double _t2;
  // CHECK-NEXT:     double _t3;
  // CHECK-NEXT:     _t1 = (this->x + this->y);
  // CHECK-NEXT:     _t0 = i;
  // CHECK-NEXT:     _t3 = i;
  // CHECK-NEXT:     _t2 = j;
  // CHECK-NEXT:     double const_volatile_rval_ref_noexcept_mem_fn_return = _t1 * _t0 + _t3 * _t2;
  // CHECK-NEXT:     goto _label0;
  // CHECK-NEXT:   _label0:
  // CHECK-NEXT:     {
  // CHECK-NEXT:         double _r0 = 1 * _t0;
  // CHECK-NEXT:         double _r1 = _t1 * 1;
  // CHECK-NEXT:         _result[0UL] += _r1;
  // CHECK-NEXT:         double _r2 = 1 * _t2;
  // CHECK-NEXT:         _result[0UL] += _r2;
  // CHECK-NEXT:         double _r3 = _t3 * 1;
  // CHECK-NEXT:         _result[1UL] += _r3;
  // CHECK-NEXT:     }
  // CHECK-NEXT: }

  void mem_fn_grad(double i, double j, double *_result) ; 
  void const_mem_fn_grad(double i, double j, double *_result) ; 
  void volatile_mem_fn_grad(double i, double j, double *_result) ; 
  void const_volatile_mem_fn_grad(double i, double j, double *_result) ; 
  void lval_ref_mem_fn_grad(double i, double j, double *_result) ; 
  void const_lval_ref_mem_fn_grad(double i, double j, double *_result) ; 
  void volatile_lval_ref_mem_fn_grad(double i, double j, double *_result) ; 
  void const_volatile_lval_ref_mem_fn_grad(double i, double j, double *_result) ; 
  void rval_ref_mem_fn_grad(double i, double j, double *_result) ; 
  void const_rval_ref_mem_fn_grad(double i, double j, double *_result) ; 
  void volatile_rval_ref_mem_fn_grad(double i, double j, double *_result) ; 
  void const_volatile_rval_ref_mem_fn_grad(double i, double j, double *_result) ; 
  void noexcept_mem_fn_grad(double i, double j, double *_result) ; 
  void const_noexcept_mem_fn_grad(double i, double j, double *_result) ; 
  void volatile_noexcept_mem_fn_grad(double i, double j, double *_result) ; 
  void const_volatile_noexcept_mem_fn_grad(double i, double j, double *_result) ; 
  void lval_ref_noexcept_mem_fn_grad(double i, double j, double *_result) ; 
  void const_lval_ref_noexcept_mem_fn_grad(double i, double j, double *_result) ; 
  void volatile_lval_ref_noexcept_mem_fn_grad(double i, double j, double *_result) ; 
  void const_volatile_lval_ref_noexcept_mem_fn_grad(double i, double j, double *_result) ; 
  void rval_ref_noexcept_mem_fn_grad(double i, double j, double *_result) ; 
  void const_rval_ref_noexcept_mem_fn_grad(double i, double j, double *_result) ; 
  void volatile_rval_ref_noexcept_mem_fn_grad(double i, double j, double *_result) ; 
  void const_volatile_rval_ref_noexcept_mem_fn_grad(double i, double j, double *_result) ; 

};

double fn(double i,double j) {
  return i*i*j;
}

// CHECK: void fn_grad(double i, double j, double *_result) {
// CHECK-NEXT:     double _t0;
// CHECK-NEXT:     double _t1;
// CHECK-NEXT:     double _t2;
// CHECK-NEXT:     double _t3;
// CHECK-NEXT:     _t2 = i;
// CHECK-NEXT:     _t1 = i;
// CHECK-NEXT:     _t3 = _t2 * _t1;
// CHECK-NEXT:     _t0 = j;
// CHECK-NEXT:     double fn_return = _t3 * _t0;
// CHECK-NEXT:     goto _label0;
// CHECK-NEXT:   _label0:
// CHECK-NEXT:     {
// CHECK-NEXT:         double _r0 = 1 * _t0;
// CHECK-NEXT:         double _r1 = _r0 * _t1;
// CHECK-NEXT:         _result[0UL] += _r1;
// CHECK-NEXT:         double _r2 = _t2 * _r0;
// CHECK-NEXT:         _result[0UL] += _r2;
// CHECK-NEXT:         double _r3 = _t3 * 1;
// CHECK-NEXT:         _result[1UL] += _r3;
// CHECK-NEXT:     }
// CHECK-NEXT: }

int main() {
  auto d_mem_fn = clad::gradient(&SimpleFunctions::mem_fn);
  auto d_const_mem_fn = clad::gradient(&SimpleFunctions::const_mem_fn);
  auto d_volatile_mem_fn = clad::gradient(&SimpleFunctions::volatile_mem_fn);
  auto d_const_volatile_mem_fn = clad::gradient(&SimpleFunctions::const_volatile_mem_fn);
  auto d_lval_ref_mem_fn = clad::gradient(&SimpleFunctions::lval_ref_mem_fn);
  auto d_const_lval_ref_mem_fn = clad::gradient(&SimpleFunctions::const_lval_ref_mem_fn);
  auto d_volatile_lval_ref_mem_fn = clad::gradient(&SimpleFunctions::volatile_lval_ref_mem_fn);
  auto d_const_volatile_lval_ref_mem_fn = clad::gradient(&SimpleFunctions::const_volatile_lval_ref_mem_fn);
  auto d_rval_ref_mem_fn = clad::gradient(&SimpleFunctions::rval_ref_mem_fn);
  auto d_const_rval_ref_mem_fn = clad::gradient(&SimpleFunctions::const_rval_ref_mem_fn);
  auto d_volatile_rval_ref_mem_fn = clad::gradient(&SimpleFunctions::volatile_rval_ref_mem_fn);
  auto d_const_volatile_rval_ref_mem_fn = clad::gradient(&SimpleFunctions::const_volatile_rval_ref_mem_fn);
  auto d_noexcept_mem_fn = clad::gradient(&SimpleFunctions::noexcept_mem_fn);
  auto d_const_noexcept_mem_fn = clad::gradient(&SimpleFunctions::const_noexcept_mem_fn);
  auto d_volatile_noexcept_mem_fn = clad::gradient(&SimpleFunctions::volatile_noexcept_mem_fn);
  auto d_const_volatile_noexcept_mem_fn = clad::gradient(&SimpleFunctions::const_volatile_noexcept_mem_fn);
  auto d_lval_ref_noexcept_mem_fn = clad::gradient(&SimpleFunctions::lval_ref_noexcept_mem_fn);
  auto d_const_lval_ref_noexcept_mem_fn = clad::gradient(&SimpleFunctions::const_lval_ref_noexcept_mem_fn);
  auto d_volatile_lval_ref_noexcept_mem_fn = clad::gradient(&SimpleFunctions::volatile_lval_ref_noexcept_mem_fn);
  auto d_const_volatile_lval_ref_noexcept_mem_fn = clad::gradient(&SimpleFunctions::const_volatile_lval_ref_noexcept_mem_fn);
  auto d_rval_ref_noexcept_mem_fn = clad::gradient(&SimpleFunctions::rval_ref_noexcept_mem_fn);
  auto d_const_rval_ref_noexcept_mem_fn = clad::gradient(&SimpleFunctions::const_rval_ref_noexcept_mem_fn);
  auto d_volatile_rval_ref_noexcept_mem_fn = clad::gradient(&SimpleFunctions::volatile_rval_ref_noexcept_mem_fn);
  auto d_const_volatile_rval_ref_noexcept_mem_fn = clad::gradient(&SimpleFunctions::const_volatile_rval_ref_noexcept_mem_fn);

  auto d_fn = clad::gradient(fn);
  double result[2];
  d_fn.execute(4,5,result);
  for(unsigned i=0;i<2;++i) {
    printf("%.2f ",result[i]);  //CHECK-EXEC: 40.00 16.00
  }
}