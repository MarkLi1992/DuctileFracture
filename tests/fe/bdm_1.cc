// ---------------------------------------------------------------------
//
// Copyright (C) 2003 - 2015 by the deal.II authors
//
// This file is part of the deal.II library.
//
// The deal.II library is free software; you can use it, redistribute
// it, and/or modify it under the terms of the GNU Lesser General
// Public License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// The full text of the license can be found in the file LICENSE at
// the top level of the deal.II distribution.
//
// ---------------------------------------------------------------------



// Show the shape functions of the BDM element on the unit cell
// Plots are gnuplot compatible if lines with desired prefix are selected.

#include "../tests.h"
#include <deal.II/fe/fe_bdm.h>

#include <vector>
#include <sstream>
#include <string>

#define PRECISION 8



template <int dim>
inline void
plot_shape_functions(const unsigned int degree)
{
  FE_BDM<dim> fe_bdm(degree);
  deallog.push(fe_bdm.get_name());

  const unsigned int div=2;
  for (unsigned int mz=0; mz<=((dim>2) ? div : 0) ; ++mz)
    for (unsigned int my=0; my<=((dim>1) ? div : 0) ; ++my)
      {
        for (unsigned int mx=0; mx<=div; ++mx)
          {
            const Point<dim> p = (dim==2 ?
                                  Point<dim>(1.*mx/div, 1.*my/div) :
                                  Point<dim>(1.*mx/div, 1.*my/div, 1.*mz/div));

            // Lines with function
            // values contain
            // quadrature point and one
            // vector of dim entries
            // for each shape function
            deallog << "value " << p;
            for (unsigned int i=0; i<fe_bdm.dofs_per_cell; ++i)
              {
                for (unsigned int c=0; c<dim; ++c)
                  deallog << " " << fe_bdm.shape_value_component(i,p,c);
                deallog << "  ";
              }
            deallog << std::endl << "grad "  << p;
            for (unsigned int i=0; i<fe_bdm.dofs_per_cell; ++i)
              {
                for (unsigned int c=0; c<dim; ++c)
                  {
                    deallog << ' ';
                    for (unsigned int d=0; d<dim; ++d)
                      deallog << ' ' << fe_bdm.shape_grad_component(i,p,c)[d];
                  }
              }
            deallog << std::endl;
          }
        deallog << "value " << std::endl;
        deallog << "grad " << std::endl;
      }

  deallog.pop();
}


int
main()
{
  std::ofstream logfile ("output");
  deallog << std::setprecision(PRECISION);
  deallog << std::fixed;
  deallog.attach(logfile);

  for (unsigned int degree=1; degree<4; ++degree)
    {
      plot_shape_functions<2>(degree);
      plot_shape_functions<3>(degree);
    }

  return 0;
}



