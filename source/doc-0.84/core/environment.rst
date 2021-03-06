Environment
===========

This section describes the routines and data structures which help set up 
Elemental's programming environment: it discusses initialization of Elemental,
call stack manipulation, a custom data structure for complex data, many routines
for manipulating real and complex data, a litany of custom enums, and a few 
useful routines for simplifying index calculations.

Build and version information
-----------------------------

Every Elemental driver with proper command-line argument processing will run
`PrintVersion` if the ``--version`` argument is used. If ``--build`` is used,
then all of the below information is reported.

.. cpp:function:: void PrintVersion( std::ostream& os=std::cout )

   Prints the Git revision, (pre-)release version, and build type. 
   For example::

    Elemental version information:
      Git revision: 3c6fbdaad901a554fc27a83378d63dab55af0dd3
      Version:      0.81-dev
      Build type:   PureDebug
   
.. cpp:function:: void PrintConfig( std::ostream& os=std::cout )

   Prints the relevant configuration details. For example::

    Elemental configuration:
      Math libraries: /usr/lib/liblapack.so;/usr/lib/libblas.so
      HAVE_F90_INTERFACE
      HAVE_MPI_REDUCE_SCATTER_BLOCK
      HAVE_MPI_IN_PLACE
      USE_BYTE_ALLGATHERS

.. cpp:function:: void PrintCCompilerInfo( std::ostream& os=std::cout )

   Prints the relevant C compilation information. For example::

    Elemental's C compiler info:
      CMAKE_C_COMPILER:    /usr/local/bin/gcc
      MPI_C_COMPILER:      /home/poulson/Install/bin/mpicc
      MPI_C_INCLUDE_PATH:  /home/poulson/Install/include
      MPI_C_COMPILE_FLAGS: 
      MPI_C_LINK_FLAGS:     -Wl,-rpath  -Wl,/home/poulson/Install/lib
      MPI_C_LIBRARIES:     /home/poulson/Install/lib/libmpich.so;/home/poulson/Install/lib/libopa.so;/home/poulson/Install/lib/libmpl.so;/usr/lib/i386-linux-gnu/librt.so;/usr/lib/i386-linux-gnu/libpthread.so

.. cpp:function:: void PrintCxxCompilerInfo( std::ostream& os=std::cout )

   Prints the relevant C++ compilation information. For example::

    Elemental's C++ compiler info:
      CMAKE_CXX_COMPILER:    /usr/local/bin/g++
      CXX_FLAGS:             -Wall
      MPI_CXX_COMPILER:      /home/poulson/Install/bin/mpicxx
      MPI_CXX_INCLUDE_PATH:  /home/poulson/Install/include
      MPI_CXX_COMPILE_FLAGS: 
      MPI_CXX_LINK_FLAGS:     -Wl,-rpath  -Wl,/home/poulson/Install/lib
      MPI_CXX_LIBRARIES:     /home/poulson/Install/lib/libmpichcxx.so;/home/poulson/Install/lib/libmpich.so;/home/poulson/Install/lib/libopa.so;/home/poulson/Install/lib/libmpl.so;/usr/lib/i386-linux-gnu/librt.so;/usr/lib/i386-linux-gnu/libpthread.so

Set up and clean up
-------------------

.. cpp:function:: void Initialize( int& argc, char**& argv )

   Initializes Elemental and (if necessary) MPI. The usage is very similar to 
   ``MPI_Init``, but the `argc` and `argv` can be directly passed in.

   .. code-block:: cpp

      #include "elemental.hpp"
      int main( int argc, char* argv[] )
      {
          elem::Initialize( argc, argv );
          // ...
          elem::Finalize();
          return 0;
      }

.. cpp:function:: void Finalize()

   Frees all resources allocated by Elemental and (if necessary) MPI.

.. cpp:function:: bool Initialized()

   Returns whether or not Elemental is currently initialized.

.. cpp:function:: void ReportException( std::exception& e )

   Used for handling Elemental's various exceptions, e.g.,

   .. code-block:: cpp

      #include "elemental.hpp"
      int main( int argc, char* argv[] )
      {
          elem::Initialize( argc, argv );
          try {
              // ...
          } catch( std::exception& e ) { ReportException(e); }
          elem::Finalize();
          return 0;
      }

Blocksize manipulation
----------------------

.. cpp:function:: int Blocksize()

   Return the currently chosen algorithmic blocksize. The optimal value 
   depends on the problem size, algorithm, and architecture; the default value
   is 128.

.. cpp:function:: void SetBlocksize( int blocksize )

   Change the algorithmic blocksize to the specified value.

.. cpp:function:: void PushBlocksizeStack( int blocksize )

   It is frequently useful to temporarily change the algorithmic blocksize, so 
   rather than having to manually store and reset the current state, one can 
   simply push a new value onto a stack 
   (and later pop the stack to reset the value).

.. cpp:function:: void PopBlocksizeStack() 

   Pops the stack of blocksizes. See above.

.. cpp:function:: int DefaultBlockHeight()
.. cpp:function:: int DefaultBlockWidth()

   Returns the default block height (width) for 
   :cpp:class:`BlockDistMatrix\<T,U,V>`.

.. cpp:function:: void SetDefaultBlockHeight( int mb )
.. cpp:function:: void SetDefaultBlockWidth( int nb )

   Change the default block height (width) for 
   :cpp:class:`BlockDistMatrix\<T,U,V>`.

Default process grid
--------------------

.. cpp:function:: Grid& DefaultGrid()

   Return a process grid built over :cpp:var:`mpi::COMM_WORLD`. This is 
   typically used as a means of allowing instances of the 
   :cpp:class:`DistMatrix\<T,MC,MR>` class to be constructed without having to 
   manually specify a process grid, e.g., 

   .. code-block:: cpp

      // Build a 10 x 10 distributed matrix over mpi::COMM_WORLD
      elem::DistMatrix<T,MC,MR> A( 10, 10 );

Call stack manipulation
-----------------------

.. note::

   The following call stack manipulation routines are only available in 
   non-release builds (i.e., PureDebug and HybridDebug) and are meant to allow 
   for the call stack to be printed (via :cpp:func:`DumpCallStack`) when an 
   exception is caught.

.. cpp:function:: void PushCallStack( std::string s )

   Push the given routine name onto the call stack.

.. cpp:function:: void PopCallStack()

   Remove the routine name at the top of the call stack.

.. cpp:function:: void DumpCallStack()

   Print (and empty) the contents of the call stack.

Custom exceptions
-----------------

.. cpp:class:: SingularMatrixException

   An extension of ``std::runtime_error`` which is meant to be thrown when 
   a singular matrix is unexpectedly encountered.

   .. cpp:function:: SingularMatrixException( const char* msg="Matrix was singular" )

      Builds an instance of the exception which allows one to optionally 
      specify the error message.

   .. code-block:: cpp

      throw elem::SingularMatrixException();

.. cpp:class:: NonHPDMatrixException 

   An extension of ``std::runtime_error`` which is meant to be thrown when
   a non positive-definite Hermitian matrix is unexpectedly encountered
   (e.g., during Cholesky factorization).

   .. cpp:function:: NonHPDMatrixException( const char* msg="Matrix was not HPD" )

      Builds an instance of the exception which allows one to optionally 
      specify the error message.

   .. code-block:: cpp

      throw elem::NonHPDMatrixException();

.. cpp:class:: NonHPSDMatrixException 

   An extension of ``std::runtime_error`` which is meant to be thrown when
   a non positive semi-definite Hermitian matrix is unexpectedly encountered
   (e.g., during computation of the square root of a Hermitian matrix).

   .. cpp:function:: NonHPSDMatrixException( const char* msg="Matrix was not HPSD" )

      Builds an instance of the exception which allows one to optionally 
      specify the error message.

   .. code-block:: cpp

      throw elem::NonHPSDMatrixException();

Complex data
------------

.. cpp:type:: Complex<Real>

   Currently a typedef of ``std::complex<Real>``

.. cpp:type:: Base<F>

   The underlying real datatype of the (potentially complex) datatype `F`.
   For example, ``Base<Complex<double>>`` and 
   ``Base<double>`` are both equivalent to ``double``.
   This is often extremely useful in implementing routines which are 
   templated over real and complex datatypes but still make use of real 
   datatypes.

.. cpp:function:: std::ostream& operator<<( std::ostream& os, Complex<Real> alpha )

   Pretty prints `alpha` in the form ``a+bi``.

.. cpp:type:: scomplex

   ``typedef Complex<float> scomplex;``

.. cpp:type:: dcomplex

   ``typedef Complex<double> dcomplex;``

Scalar manipulation
-------------------

.. cpp:function:: Base<F> Abs( const F& alpha )

   Return the absolute value of the real or complex variable :math:`\alpha`.

.. cpp:function:: F FastAbs( const F& alpha )

   Return a cheaper norm of the real or complex :math:`\alpha`:

   .. math::
   
      |\alpha|_{\mbox{fast}} = |\mathcal{R}(\alpha)| + |\mathcal{I}(\alpha)|

.. cpp:function:: F RealPart( const F& alpha )
.. cpp:function:: F ImagPart( const F& alpha )

   Return the real (imaginary) part of the real or complex variable 
   :math:`\alpha`.

.. cpp:function:: void SetRealPart( F& alpha, Base<F>& beta )
.. cpp:function:: void SetImagPart( F& alpha, Base<F>& beta )

   Set the real (imaginary) part of the real or complex variable 
   :math:`\alpha` to :math:`\beta`. 
   If :math:`\alpha` has a real type, an error is thrown when an attempt is
   made to set the imaginary component.

.. cpp:function:: void UpdateRealPart( F& alpha, Base<F>& beta )
.. cpp:function:: void UpdateImagPart( F& alpha, Base<F>& beta )

   Update the real (imaginary) part of the real or complex variable 
   :math:`\alpha` to :math:`\beta`.
   If :math:`\alpha` has a real type, an error is thrown when an attempt is
   made to update the imaginary component.

.. cpp:function:: F Conj( const F& alpha )

   Return the complex conjugate of the real or complex variable :math:`\alpha`.

.. cpp:function:: F Sqrt( const F& alpha )

   Returns the square root or the real or complex variable :math:`\alpha`.

.. cpp:function:: F Cos( const F& alpha )

   Returns the cosine of the real or complex variable :math:`\alpha`.

.. cpp:function:: F Sin( const F& alpha )

   Returns the sine of the real or complex variable :math:`\alpha`.

.. cpp:function:: F Tan( const F& alpha )

   Returns the tangent of the real or complex variable :math:`\alpha`.

.. cpp:function:: F Cosh( const F& alpha )

   Returns the hyperbolic cosine of the real or complex variable :math:`\alpha`.

.. cpp:function:: F Sinh( const F& alpha )

   Returns the hyperbolic sine of the real or complex variable :math:`\alpha`.

.. cpp:function:: Base<F> Arg( const F& alpha )

   Returns the argument of the real or complex variable :math:`\alpha`.

.. cpp:function:: Complex<Real> Polar( const R& r, const R& theta=0 )

   Returns the complex variable constructed from the polar coordinates
   :math:`(r,\theta)`.

.. cpp:function:: F Exp( const F& alpha )

   Returns the exponential of the real or complex variable :math:`\alpha`.

.. cpp:function:: F Pow( const F& alpha, const F& beta )

   Returns :math:`\alpha^\beta` for real or complex :math:`\alpha` and 
   :math:`\beta`.

.. cpp:function:: F Log( const F& alpha )

   Returns the logarithm of the real or complex variable :math:`\alpha`.

Other typedefs and enums
------------------------

.. cpp:type:: byte

   ``typedef unsigned char byte;``

.. cpp:enum:: Conjugation

   .. cpp:enumerator:: CONJUGATED

   .. cpp:enumerator:: UNCONJUGATED

.. cpp:enum:: Distribution

   For specifying the distribution of a row or column of a distributed matrix:

   .. cpp:enumerator:: MC

      Column of a standard matrix distribution

   .. cpp:enumerator:: MD

      Diagonal of a standard matrix distribution

   .. cpp:enumerator:: MR
   
      Row of a standard matrix distribution

   .. cpp:enumerator:: VC

      Column-major vector distribution

   .. cpp:enumerator:: VR

      Row-major vector distribution

   .. cpp:enumerator:: STAR

      Redundantly stored on every process
  
   .. cpp:enumerator:: CIRC

      Stored on a single process

.. cpp:enum:: ForwardOrBackward

   .. cpp:enumerator:: FORWARD

   .. cpp:enumerator:: BACKWARD

.. cpp:enum:: GridOrder

   For specifying either a ``ROW_MAJOR`` or ``COLUMN_MAJOR`` ordering;
   it is used to decide how to construct process grids and is also useful for 
   tuning one of the algorithms in :cpp:func:`HermitianTridiag`
   which requires building a smaller square process grid from a rectangular 
   process grid, as the ordering of the processes can greatly impact 
   performance. See :cpp:func:`SetHermitianTridiagGridOrder`.

   .. cpp:enumerator:: ROW_MAJOR

   .. cpp:enumerator:: COLUMN_MAJOR

.. cpp:enum:: LeftOrRight

   .. cpp:enumerator:: LEFT

   .. cpp:enumerator:: RIGHT

.. cpp:enum:: SortType

   For specifying a sorting strategy:

   .. cpp:enumerator:: UNSORTED

      Do not sort

   .. cpp:enumerator:: DESCENDING

      Largest values first

   .. cpp:enumerator:: ASCENDING

      Smallest values first 

.. cpp:enum:: NormType

   .. cpp:enumerator:: ONE_NORM

      .. math:: 

         \|A\|_1 = \max_{\|x\|_1=1} \|Ax\|_1 
                 = \max_j \sum_{i=0}^{m-1} |\alpha_{i,j}|

   .. cpp:enumerator:: INFINITY_NORM

      .. math:: 

         \|A\|_{\infty} = \max_{\|x\|_{\infty}=1} \|Ax\|_{\infty} 
                        = \max_i \sum_{j=0}^{n-1} |\alpha_{i,j}|

   .. cpp:enumerator:: ENTRYWISE_ONE_NORM

      .. math::

        \|\text{vec}(A)\|_1 = \sum_{i,j} |\alpha_{i,j}|

   .. cpp:enumerator:: MAX_NORM

      .. math::
     
         \|A\|_{\mbox{max}} = \max_{i,j} |\alpha_{i,j}|

   .. cpp:enumerator:: NUCLEAR_NORM

      .. math::

         \|A\|_* = \sum_{i=0}^{\min(m,n)} \sigma_i(A)

   .. cpp:enumerator:: FROBENIUS_NORM

      .. math::

         \|A\|_F = \sqrt{\sum_{i=0}^{m-1} \sum_{j=0}^{n-1} |\alpha_{i,j}|^2}
                 = \sum_{i=0}^{\min(m,n)} \sigma_i(A)^2

   .. cpp:enumerator:: TWO_NORM

      .. math::

         \|A\|_2 = \max_i \sigma_i(A)
  
.. cpp:enum:: Orientation

   .. cpp:enumerator:: NORMAL

      Do not transpose or conjugate

   .. cpp:enumerator:: TRANSPOSE

      Transpose without conjugation

   .. cpp:enumerator:: ADJOINT

      Transpose with conjugation

.. cpp:enum:: UnitOrNonUnit

   .. cpp:enumerator:: UNIT

   .. cpp:enumerator:: NON_UNIT

.. cpp:enum:: UpperOrLower

   .. cpp:enumerator:: LOWER

   .. cpp:enumerator:: UPPER

.. cpp:enum:: VerticalOrHorizontal

   .. cpp:enumerator:: VERTICAL

   .. cpp:enumerator:: HORIZONTAL

Indexing utilities
------------------

.. cpp:function:: int Shift( int rank, int firstRank, int numProcs )

   Given a element-wise cyclic distribution over `numProcs` processes, 
   where the first entry is owned by the process with rank `firstRank`, 
   this routine returns the first entry owned by the process with rank
   `rank`.

.. cpp:function:: int Length( int n, int shift, int numProcs )

   Given a vector with :math:`n` entries distributed over `numProcs` 
   processes with shift as defined above, this routine returns the number of 
   entries of the vector which are owned by this process.

.. cpp:function:: int Length( int n, int rank, int firstRank, int numProcs )

   Given a vector with :math:`n` entries distributed over `numProcs` 
   processes, with the first entry owned by process `firstRank`, this routine
   returns the number of entries locally owned by the process with rank 
   `rank`.

.. cpp:function:: int MaxLength( int n, int numProcs )

   The maximum result of :cpp:func:`Length` with the given parameters.
   This is useful for padding collective communication routines which are
   almost regular.

.. cpp:function:: int Mod( int a, int b )

   An extension of C++'s ``%`` operator which handles cases where `a` is 
   negative and still returns a result in :math:`[0,b)`.

.. cpp:function:: int GCD( int a, int b )

   Return the greatest common denominator of the integers `a` and `b`.

.. cpp:function:: unsigned Log2( unsigned n )

   Return the base-two logarithm of a positive integer.

.. cpp:function:: bool PowerOfTwo( unsigned n )

   Return whether or not a positive integer is an integer power of two.
