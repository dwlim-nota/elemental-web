Build system
************
Elemental's build system relies on `CMake <http://www.cmake.org>`__ 
in order to manage a large number of configuration options in a 
platform-independent manner; it can be easily configured to build on Linux and 
Unix environments (including Darwin), and, at least in theory, various versions
of Microsoft Windows. A relatively up-to-date C++11 compiler 
(e.g., gcc >= 4.7) is required in all cases.

Elemental's main external dependencies are

1. `CMake <http://www.cmake.org/>`__ 
2. `MPI <http://en.wikipedia.org/wiki/Message_Passing_Interface>`__ 
3. `BLAS <http://netlib.org/blas>`__ 
4. `LAPACK <http://netlib.org/lapack>`__.

Included within the project are

1. `PMRRR <http://code.google.com/p/pmrrr>`__, which Elemental depends upon for
   parallel symmetric tridiagonal eigensolvers, which is included within the 
   `external/pmrrr` folder, and
2. `METIS 5.1.0 <http://glaros.dtc.umn.edu/gkhome/metis/metis/overview>`__, 
   which is used for graph partitioning and is included within the 
   `external/metis` folder.

Furthermore, there are several optional external dependencies:

1. `libFLAME <http://www.cs.utexas.edu/users/flame/>`_ is recommended 
for faster SVD's due to its high-performance bidiagonal QR algorithm 
implementation, and 
2. `Qt5 <http://qt-project.org>`_ is required for matrix visualization.

Dependencies
============

CMake
-----
Elemental uses several new CMake modules, so it is important to ensure that 
version 2.8.8 or later is installed. Thankfully the 
`installation process <http://www.cmake.org/cmake/help/install.html>`_
is extremely straightforward: either download a platform-specific binary from
the `downloads page <http://www.cmake.org/cmake/resources/software.html>`_,
or instead grab the most recent stable tarball and have CMake bootstrap itself.
In the simplest case, the bootstrap process is as simple as running the 
following commands::

    ./bootstrap
    make
    make install

Note that recent versions of `Ubuntu <http://www.ubuntu.com/>`__ (e.g., version 13.10) have sufficiently up-to-date
versions of CMake, and so the following command is sufficient for installation::

    sudo apt-get install cmake

If you do install from source, there are two important issues to consider

1. By default, ``make install`` attempts a system-wide installation 
   (e.g., into ``/usr/bin``) and will likely require administrative privileges.
   A different installation folder may be specified with the ``--prefix`` 
   option to the ``bootstrap`` script, e.g.,::

    ./bootstrap --prefix=/home/your_username
    make
    make install

   Afterwards, it is a good idea to make sure that the environment variable 
   ``PATH`` includes the ``bin`` subdirectory of the installation folder, e.g.,
   ``/home/your_username/bin``.

2. Some highly optimizing compilers will not correctly build CMake, but the GNU
   compilers nearly always work. You can specify which compilers to use by
   setting the environment variables ``CC`` and ``CXX`` to the full paths to 
   your preferred C and C++ compilers before running the ``bootstrap`` script.

Basic usage
^^^^^^^^^^^
Though many configuration utilities, like 
`autoconf <http://www.gnu.org/software/autoconf/>`_, are designed such that
the user need only invoke ``./configure && make && make install`` from the
top-level source directory, CMake targets *out-of-source* builds, which is 
to say that the build process occurs away from the source code. The 
out-of-source build approach is ideal for projects that offer several 
different build modes, as each version of the project can be built in a 
separate folder.

A common approach is to create a folder named ``build`` in the top-level of 
the source directory and to invoke CMake from within it::

    mkdir build
    cd build
    cmake ..

The last line calls the command line version of CMake, ``cmake``,
and tells it that it should look in the parent directory for the configuration
instructions, which should be in a file named ``CMakeLists.txt``. Users that 
would prefer a graphical interface from the terminal (through ``curses``)
should instead use ``ccmake`` (on Unix platforms) or ``CMakeSetup`` 
(on Windows platforms). In addition, a GUI version is available through 
``cmake-gui``. 

Though running ``make clean`` will remove all files generated from running 
``make``, it will not remove configuration files. Thus, the best approach for
completely cleaning a build is to remove the entire build folder. On \*nix 
machines, this is most easily accomplished with::

    cd .. 
    rm -rf build

This is a better habit than simply running ``rm -rf *`` since, 
if accidentally run from the wrong directory, the former will most likely fail.

MPI
---
An implementation of the Message Passing Interface (MPI) is required for 
building Elemental. The two most commonly used implementations are

1. `MPICH2 <http://www.mcs.anl.gov/research/projects/mpich2/>`_
2. `OpenMPI <http://www.open-mpi.org/>`_

If your cluster uses `InfiniBand <http://en.wikipedia.org/wiki/InfiniBand>`_ as its interconnect, you may want to look into 
`MVAPICH2 <http://mvapich.cse.ohio-state.edu/overview/mvapich2/>`_.

Each of the respective websites contains installation instructions, but, on recent versions of `Ubuntu <http://www.ubuntu.com/>`__ (such as version 12.04), 
MPICH2 can be installed with ::

    sudo apt-get install libmpich2-dev

and OpenMPI can be installed with ::

    sudo apt-get install libopenmpi-dev

BLAS and LAPACK
---------------
The Basic Linear Algebra Subprograms (BLAS) and Linear Algebra PACKage (LAPACK) 
are both used heavily within Elemental. On most installations of `Ubuntu <http://www.ubuntu.com>`__, the following command should suffice for their installation::

    sudo apt-get install libatlas-dev liblapack-dev

The reference implementation of LAPACK can be found at

    http://www.netlib.org/lapack/

and the reference implementation of BLAS can be found at

    http://www.netlib.org/blas/

However, it is better to install an optimized version of these libraries,
especialy for the BLAS. The most commonly used open source versions are 
`ATLAS <http://math-atlas.sourceforge.net/>`__ and `OpenBLAS <https://github.com/xianyi/OpenBLAS>`__. Support for `BLIS <http://code.google.com/p/blis>`__ is
planned in the near future.

PMRRR
-----
PMRRR is a parallel implementation of the MRRR algorithm introduced by 
`Inderjit Dhillon <http://www.cs.utexas.edu/~inderjit/>`_ and 
`Beresford Parlett <http://math.berkeley.edu/~parlett/>`_ for computing 
:math:`k` eigenvectors of a tridiagonal matrix of size :math:`n` in 
:math:`\mathcal{O}(nk)` time. PMRRR was written by 
`Matthias Petschow <http://www.aices.rwth-aachen.de/people/petschow>`_ and 
`Paolo Bientinesi <http://www.aices.rwth-aachen.de/people/bientinesi>`_ and,
while it is included within Elemental, it is also available at:

    http://code.google.com/p/pmrrr

Note that PMRRR currently requires support for pthreads.

libFLAME
--------
`libFLAME` is an open source library made available as part of the FLAME 
project. Its stated objective is to

.. epigraph::
   ...transform the development of dense linear algebra libraries from an art 
   reserved for experts to a science that can be understood by novice and 
   expert alike.

Elemental's current implementation of parallel SVD is dependent upon a serial 
kernel for the bidiagonal SVD. A high-performance implementation of this 
kernel was recently introduced in 
"Restructuring the QR Algorithm for Performance", by Field G. van Zee, Robert 
A. van de Geijn, and Gregorio Quintana-Orti. It can be found at

    http://www.cs.utexas.edu/users/flame/pubs/RestructuredQRTOMS.pdf

Installation of `libFLAME` is fairly straightforward. It is recommended that 
you download the latest nightly snapshot from

    http://www.cs.utexas.edu/users/flame/snapshots/

and then installation should simply be a matter of running::

    ./configure
    make
    make install

Qt5
---
Qt is an open source cross-platform library for creating Graphical User 
Interfaces (GUIs) in C++. Elemental currently supports using version 5.1.1 of 
the library to display and save images of matrices.

Please visit Qt Project's `download page <http://qt-project.org/downloads>`__
for download and installation instructions. Note that, if Elemental is launched
with the `-no-gui` command-line option, then Qt5 will be started without GUI
support. This supports using Elemental on clusters whose compute nodes do not
run display servers, but PNG's of matrices need to be created using Qt5's 
simple interface.

Getting Elemental's source 
==========================
There are two basic approaches:

1. Download a tarball of a recent version from 
   `libelemental.org/releases <http://libelemental.org/releases/>`_. 
   A new version is typically released every one to two months.

2. Install `git <http://git-scm.com/>`_ and check out a copy of 
   the development repository by running ::

    git clone git://github.com/elemental/Elemental.git

Building Elemental
==================
On \*nix machines with `BLAS <http://www.netlib.org/blas/>`__, 
`LAPACK <http://www.netlib.org/lapack/>`__, and 
`MPI <http://en.wikipedia.org/wiki/Message_Passing_Interface>`__ installed in 
standard locations, building Elemental can be as simple as::

    cd elemental
    mkdir build
    cd build
    cmake ..
    make
    make install

As with the installation of CMake, the default install location is 
system-wide, e.g., ``/usr/local``. The installation directory can be changed
at any time by running::

    cmake -D CMAKE_INSTALL_PREFIX=/your/desired/install/path ..
    make install


Though the above instructions will work on many systems, it is common to need
to manually specify several build options, especially when multiple versions of
libraries or several different compilers are available on your system. For 
instance, any C++, C, or Fortran compiler can respectively be set with the 
``CMAKE_CXX_COMPILER``, ``CMAKE_C_COMPILER``, and ``CMAKE_Fortran_COMPILER`` 
variables, e.g., ::

    cmake -D CMAKE_CXX_COMPILER=/usr/bin/g++ \
          -D CMAKE_C_COMPILER=/usr/bin/gcc   \
          -D CMAKE_Fortran_COMPILER=/usr/bin/gfortran ..
    
It is also common to need to specify which libraries need to be linked in order
to provide serial BLAS and LAPACK routines (and, if SVD is important, libFLAME).
The ``MATH_LIBS`` variable was introduced for this purpose and an example 
usage for configuring with BLAS and LAPACK libraries in ``/usr/lib`` would be ::

    cmake -D MATH_LIBS="-L/usr/lib -llapack -lblas -lm" ..

It is important to ensure that if library A depends upon library B, A should 
be specified to the left of B; in this case, LAPACK depends upon BLAS, so 
``-llapack`` is specified to the left of ``-lblas``.

If `libFLAME <http://www.cs.utexas.edu/users/flame/>`__ is 
available at ``/path/to/libflame.a``, then the above link line should be changed
to ::

    cmake -D MATH_LIBS="/path/to/libflame.a;-L/usr/lib -llapack -lblas -lm" ..

Elemental's performance in Singular Value Decompositions (SVD's) is 
greatly improved on many architectures when libFLAME is linked.

Build modes
-----------
Elemental currently has four different build modes:

* **PureDebug** - An MPI-only build that maintains a call stack and provides 
  more error checking.
* **PureRelease** - An optimized MPI-only build suitable for production use.
* **HybridDebug** - An MPI+OpenMP build that maintains a call stack and provides
  more error checking.
* **HybridRelease** - An optimized MPI+OpenMP build suitable for production use.

The build mode can be specified with the ``CMAKE_BUILD_TYPE`` option, e.g., 
``-D CMAKE_BUILD_TYPE=PureDebug``. If this option is not specified, Elemental
defaults to the **PureRelease** build mode.

Once the build mode is selected, one might also want to manually set the 
optimization level of the compiler, e.g., via the CMake option 
``-D CXX_FLAGS="-O3"``.

Testing the installation
========================
Once Elemental has been installed, it is a good idea to verify that it is 
functioning properly. An example of generating a random distributed matrix, 
computing its Singular Value Decomposition (SVD), and checking for numerical 
error is available in `examples/lapack_like/SVD.cpp <https://github.com/elemental/Elemental/blob/master/examples/lapack_like/SVD.cpp>`__.

As you can see, the only required header is ``El.hpp``, which must be
in the include path when compiling this simple driver, ``SVD.cpp``. 
If Elemental was installed in ``/usr/local``, then 
``/usr/local/conf/ElVars`` can be used to build a simple Makefile::

    include /usr/local/conf/ElVars

    SVD: SVD.cpp
        ${CXX} ${EL_COMPILE_FLAGS} $< -o $@ ${EL_LINK_FLAGS} ${EL_LIBS}

As long as ``SVD.cpp`` and this ``Makefile`` are in the current directory,
simply typing ``make`` should build the driver. 

The executable can then typically be run with a single process (generating a 
:math:`300 \times 300` distributed matrix, using ::

    ./SVD --height 300 --width 300

and the output should be similar to ::
    
    ||A||_max   = 0.999997
    ||A||_1     = 165.286
    ||A||_oo    = 164.116
    ||A||_F     = 173.012
    ||A||_2     = 19.7823

    ||A - U Sigma V^H||_max = 2.20202e-14
    ||A - U Sigma V^H||_1   = 1.187e-12
    ||A - U Sigma V^H||_oo  = 1.17365e-12
    ||A - U Sigma V^H||_F   = 1.10577e-12
    ||A - U Sigma V_H||_F / (max(m,n) eps ||A||_2) = 1.67825

The driver can be run with several processes using the MPI launcher provided
by your MPI implementation; a typical way to run the ``SVD`` driver on 
eight processes would be::

    mpirun -np 8 ./SVD --height 300 --width 300

You can also build a wide variety of example and test drivers 
(unfortunately the line is a little blurred) by using the CMake options::

    -D EL_EXAMPLES=ON

and/or ::

    -D EL_TESTS=ON  

Elemental as a subproject
=========================
Adding Elemental as a dependency into a project which uses CMake for its build 
system is relatively straightforward: simply put an entire copy of the 
Elemental source tree in a subdirectory of your main project folder, say 
``external/elemental``, and then create a ``CMakeLists.txt`` file in your main 
project folder that builds off of the following snippet::

    cmake_minimum_required(VERSION 2.8.8) 
    project(Foo)

    add_subdirectory(external/elemental)
    include_directories("${PROJECT_BINARY_DIR}/external/El/include")
    include_directories(${MPI_CXX_INCLUDE_PATH})

    # Build your project here
    # e.g., 
    #   add_library(foo ${LIBRARY_TYPE} ${FOO_SRC})
    #   target_link_libraries(foo El)

Troubleshooting
===============
If you run into build problems, please email 
`maint@libelemental.org <mailto:maint@libelemental.org>`_ 
and make sure to attach the file ``include/El/config.h``, which should 
be generated within your build directory. 
Please only direct usage questions to 
`users@libelemental.org <mailto:users@libelemental.org>`_, 
and development questions to 
`dev@libelemental.org <mailto:dev@libelemental.org>`_.
