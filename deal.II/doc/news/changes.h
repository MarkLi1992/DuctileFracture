 // ---------------------------------------------------------------------
// $Id$
//
// Copyright (C) 2013, 2014 by the deal.II authors
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

/**
// * @page changes_after_8_1 Changes after Version 8.1

<p>
This is the list of changes made after the release of
deal.II version 8.1.0.
All entries are signed with the names of the authors.
</p>



<!-- ----------- INCOMPATIBILITIES ----------------- -->

<a name="incompatible"></a>
<h3 style="color:red">Incompatibilities</h3>

<p style="color:red">
Following are a few modifications to the library that unfortunately
are incompatible with previous versions of the library, but which we
deem necessary for the future maintainability of the
library. Unfortunately, some of these changes will require
modifications to application programs. We apologize for the
inconvenience this causes.
</p>

<ol>
</ol>


<!-- ----------- GENERAL IMPROVEMENTS ----------------- -->

<a name="general"></a>
<h3>General</h3>


<ol>
  <li> New: deal.II now links with the
  <a href="http://www.boost.org/doc/libs/1_55_0/libs/iostreams/doc/index.html">BOOST
  Iostreams</a> library (at least if the libz and libbz2 libraries
  can be found that are necessary for BOOST Iostreams).
  Among many other things, this allows to easily
  read files that have been compressed, as in the following code snippet:
  @code
    #include <boost/iostreams/filtering_stream.hpp>
    #include <boost/iostreams/filter/gzip.hpp>
    #include <boost/iostreams/device/file.hpp>

    ...

    boost::iostreams::filtering_istream in;
    in.push(boost::iostreams::basic_gzip_decompressor<>());
    in.push(boost::iostreams::file_source("myfile.gz"));

    int i;
    in >> i;
  @endcode
  More documentation on how to use BOOST Iostream can be found
  in the documentation link referenced above.
  <br>
  (Wolfgang Bangerth, 2013/12/21)
  </li>
</ol>


<!-- ----------- SPECIFIC IMPROVEMENTS ----------------- -->

<a name="specific"></a>
<h3>Specific improvements</h3>

<ol>
  <li> Fixed: PETScWrappers::MPI::Vector::all_zero() was broken with more than
  one processor (illegal memory access) and did not communicate between all
  processors. Documentation for many vector types of all_zero() has been extended.
  <br>
  (Timo Heister, 2014/01/17)
  </li>

  <li> Fixed/new: DoFCellAccessor::set_dof_values_by_interpolation and
  DoFCellAccessor::get_interpolated_dof_values could previously be
  called for hp::DoFHandler objects on cells that are non-active. This
  makes no sense since these cells have no associated finite element
  space. Doing so now raises an exception.
  <br>
  However, there are legitimate cases where one may want to interpolate
  from children to a parent's finite element space. Since in the hp
  case no finite element space is naturally associated with an inactive
  cell, it is now possible to pass an explicit finite element index
  argument to the function specifying which element of an hp::FECollection
  object describes the space onto which you want to interpolate.
  <br>
  (Wolfgang Bangerth, 2014/01/18)
  </li>

  <li> Fixed: The methods IndexSet::do_compress() and
  IndexSet::add_indices(IndexSet&) had quadratic complexity in the number of
  ranges. The algorithms have been changed into linear complexity ones.
  <br>
  (Martin Kronbichler, 2014/01/15)
  </li>

  <li> Fixed: There were several bugs in functions like
  FEValues::get_function_values() where the code did not properly handle the
  case of FE_Nothing. This is now fixed.
  <br>
  (Wolfgang Bangerth, 2014/01/08)
  </li>

  <li> Fixed: DataOut got confused in some situations where one uses FE_Nothing.
  This is now fixed.
  <br>
  (Minh Do-Quang, Wolfgang Bangerth, 2014/01/08)
  </li>

  <li> Fixed: FESystem::get_interpolation_matrix, a function that is among
  other places used by SolutionTransfer, had a bug that prevented it from
  running correctly in some situations where one uses FE_Nothing.
  This is now fixed.
  <br>
  (Minh Do-Quang, Wolfgang Bangerth, 2014/01/08)
  </li>

  <li> Improved: When you call WorkStream::run with an empty function object
  for the copier, operations on individual cells are essentially all independent.
  In other words, you have a massively parallel collection of jobs. In this
  case, a parallel for loop over all elements is better suited than the
  pipeline approach currently used. This has now been implemented.
  <br>
  (Wolfgang Bangerth, 2013/12/26)
  </li>

  <li> New: The new function VectorTools::interpolate_based_on_material_id()
  can be used to interpolate several functions onto a mesh, based on the
  material id of each cell individually.
  <br>
  (Valentin Zingan, 2013/12/26)
  </li>

  <li> New: A new reinit() method has been introduced to
  TrilinosWrappers::SparsityPattern that takes all rows that are possibly
  written into as an optional argument. This allows for pre-allocating all
  possible entries right away, which makes writing into the matrix from
  several threads possible (otherwise, only one processor at a time can write
  off-processor data). Similarly, TrilinosWrappers::MPI::Vector objects can
  be initialized with hints to ghost elements for a writable vector that can
  be added into from multiple threads.
  <br>
  (Martin Kronbichler, 2013/12/23)
  </li>

  <li> New: The TableBase::fill function has become more powerful in that
  it now doesn't just take pointers to initializing elements but can deal
  with arbitrary input iterators. It now also takes a flag that denotes the
  order in which table elements are initialized, allowing to switch between
  C- and Fortran-style table layouts.
  <br>
  Along with the TableBase::fill function, the Table classes of various
  ranks have also gotten constructors that allow the in-place construction
  not only of a table of correct size, but already initialized from
  somewhere. This finally allows to mark Table objects as const by creating
  them already with the correct content.
  <br>
  (Wolfgang Bangerth, 2013/12/21)
  </li>

  <li> New: There is now a new class Functions::InterpolatedTensorProductGridData that can
  be used to (bi-/tri-)linearly interpolate data given on a tensor product
  mesh of $x$ (and $y$ and $z$) values, for example to evaluate experimentally
  determined coefficients, or to assess the accuracy of a solution by
  comparing with a solution generated by a different code and written in
  gridded data. There is also a new class Functions::InterpolatedUniformGridData that
  can perform the same task more efficiently if the data is stored on meshes
  that are uniform in each coordinate direction.
  <br>
  (Wolfgang Bangerth, 2013/12/20)
  </li>

  <li> Fixed: ParameterHandler::get_double() and ParameterHandler::get_integer()
  had bugs in that they didn't detect if they were asked to return a number
  for a parameter whose value was in fact not a number but some general
  text. This is now fixed.
  <br>
  (Wolfgang Bangerth, 2013/12/19)
  </li>

  <li> Fixed: VectorTools::project_boundary_values could not deal with
  function values close to (but not exactly equal to) zero. This is now fixed.
  <br>
  (Martin Kronbichler, 2013/12/16)
  </li>

  <li> New: It is now possible to select between different smoothers and coarse
  solvers in the Trilinos AMG preconditioners by a string to the smoother's name.
  <br>
  (Andrew Baker, 2013/12/14)
  </li>

</ol>


*/
