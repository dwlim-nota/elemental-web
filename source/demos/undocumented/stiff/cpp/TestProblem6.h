// Copyright (C) 2004-2006 Anders Logg.
// Licensed under the GNU LGPL Version 2.1.
//
// First added:  2004
// Last changed: 2006-08-21

#include <cmath>
#include <dolfin.h>

using namespace dolfin;

class TestProblem6 : public ODE
{
public:

  TestProblem6() : ODE(2, 100.0)
  {
    info("Van der Pol's equation.");

    mu = 10.0;
  }

  void u0(real* u)
  {
    u[0] = 2.0;
    u[1] = 0.0;
  }

  void f(const real* u, real t, real* y)
  {
    y[0] = u[1];
    y[1] = mu*(1.0 - u[0]*u[0])*u[1] - u[0];
  }

private:

  real mu;

};