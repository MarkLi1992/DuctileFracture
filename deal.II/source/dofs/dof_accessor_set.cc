// ---------------------------------------------------------------------
// $Id$
//
// Copyright (C) 1998 - 2014 by the deal.II authors
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

#include <deal.II/lac/vector.h>
#include <deal.II/lac/block_vector.h>
#include <deal.II/lac/parallel_vector.h>
#include <deal.II/lac/parallel_block_vector.h>
#include <deal.II/lac/petsc_vector.h>
#include <deal.II/lac/petsc_block_vector.h>
#include <deal.II/lac/trilinos_vector.h>
#include <deal.II/lac/trilinos_block_vector.h>
#include <deal.II/lac/sparse_matrix.h>

#include <deal.II/dofs/dof_accessor.h>
#include <deal.II/dofs/dof_handler.h>
#include <deal.II/dofs/dof_levels.h>
#include <deal.II/hp/dof_handler.h>
#include <deal.II/grid/tria_boundary.h>
#include <deal.II/grid/tria_iterator.h>
#include <deal.II/grid/tria_iterator.templates.h>
#include <deal.II/fe/fe.h>

#include <vector>

DEAL_II_NAMESPACE_OPEN



template <class DH, bool lda>
template <class OutputVector, typename number>
void
DoFCellAccessor<DH,lda>::
set_dof_values_by_interpolation (const Vector<number> &local_values,
                                 OutputVector         &values,
                                 const unsigned int fe_index) const
{
  if (!this->has_children())
    // if this cell has no children: simply set the values on this cell
    this->set_dof_values (local_values, values);
  else
    // otherwise distribute them to the children
    {
      // we are on a non-active cell. these do not have any finite
      // element associated with them in the hp context (in the non-hp
      // context, we can simply assume that the FE space to from which we
      // want to interpolate is the same as for all elements in the
      // mesh). consequently, we cannot interpolate to children's FE
      // space from this cell's (unknown) FE space unless an explicit
      // fe_index is given
      Assert ((dynamic_cast<DoFHandler<DH::dimension,DH::space_dimension>*>
               (this->dof_handler)
               != 0)
              ||
              (fe_index != DH::default_fe_index),
              ExcMessage ("You cannot call this function on non-active cells "
                          "of hp::DoFHandler objects unless you provide an explicit "
                          "finite element index because they do not have naturally "
                          "associated finite element spaces associated: degrees "
                          "of freedom are only distributed on active cells for which "
                          "the active_fe_index has been set."));

      const unsigned int dofs_per_cell = this->get_fe().dofs_per_cell;

      Assert (this->dof_handler != 0,
	      typename BaseClass::ExcInvalidObject());
      Assert (&this->get_fe() != 0,
	      typename BaseClass::ExcInvalidObject());
      Assert (local_values.size() == dofs_per_cell,
	      typename BaseClass::ExcVectorDoesNotMatch());
      Assert (values.size() == this->dof_handler->n_dofs(),
	      typename BaseClass::ExcVectorDoesNotMatch());

      Vector<number> tmp(dofs_per_cell);

      for (unsigned int child=0; child<this->n_children(); ++child)
        {
          Assert (this->child(child)->get_fe().dofs_per_cell == dofs_per_cell,
                  ExcNotImplemented());

          // prolong the given data to the present cell. FullMatrix only wants
          // us to call vmult if the matrix size is actually non-zero, so
          // check that case
          if (tmp.size() > 0)
            {
              this->get_fe().get_prolongation_matrix(child, this->refinement_case())
              .vmult (tmp, local_values);

              this->child(child)->set_dof_values_by_interpolation (tmp, values);
            }
        }
    }
}



// --------------------------------------------------------------------------
// explicit instantiations
#include "dof_accessor_set.inst"

DEAL_II_NAMESPACE_CLOSE
