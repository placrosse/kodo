.. _unittest:

Unit Testing
============

This section describes how the Kodo unit tests works, how to run them
and how to extend them.

Overview
--------

The purpose of the Kodo unit tests is to assert that the various Kodo
features work across a number of different platforms and
compilers. Having an extensive unit test suite furthermore makes it
easier for developers to ensure that their changes works as expected
and do not cause regressions in unexpected areas of the library.

The goal is that all features in Kodo should have a corresponding test
case. Often a test case is added while when implementing a new
feature. This makes it possible to assert that the new functionality
works as expected as early as possible.

The unit tests are implemented using the Google C++ Testing Framework
(gtest) which defines a bunch of helpers useful when writing
tests. You can find more information on gtest on their homepage.

Running the tests
-----------------
One of the first things you might want to try is to run the Kodo unit
tests on your own machine. There are roughly two ways to do this:

Run the test binary manually
............................

The test binary is built using the waf build scripts shipped as part
of the Kodo source code. You can read more about how you can get the
source code and how to build it in the :ref:`getting_started` section.

Once the code is built the test binary will be located in the
``build`` folder depending on the platform.

Linux
    ``build/linux/test/kodo_tests``

MacOS
    ``build/darwin/test/kodo_tests``
Windows
    ``build/win32/test/kodo_tests.exe``

If you are cross-compiling with a *mkspec* the resulting binary will
be located in:

mkspec
    ``build/mkspec/test/kodo_tests``

Run the test as part of the build
.................................

In some cases it is convenient to run the test binary as part of build
this can be done by passing the following options to the waf build scripts.

::

  python waf --options=run_tests,run_always

.. note:: If the ``run_always`` option is removed the unit tests will
          only run when the test binary changes.

Adding a new test
-----------------

When adding a new feature to Kodo is is usually also a good idea to
also add a corresponding unit test. The source code for the different
unit tests are placed in the ``test/src`` folder in the Kodo project.

All files with a ``.cpp`` file extension in the ``test/src`` will
automatically be included in the test executable produced when
building Kodo with waf.

In general we follow the following guidelines regarding unit tests:

1. Every class should have a corresponding unit test cpp file.
2. If the class is named ``my_fast_code`` then the unit test should be
   implemented in ``test/src/test_my_fast_code.cpp``

.. note:: In some cases we have headers containing only
          ``type-aliases`` for possible a set of layers we currently
          do not have explicit unit-test for these composite sets of
          layers.

          An example is
          ``src/kodo/partial_mutable_shallow_storage_layers.hpp``
          which only contains a single ``using`` declaration.

The purpose of this is to make it easy to find the unit test for a
specific class. In some cases it makes sense to have multiple classes
tested in the same file. In those cases we still make a place-holder
cpp file referring to the actual cpp file where the test can be
found. An example of this can be seen for some of the codecs e.g. the
class ``full_rlnc_encoder`` located in
``src/kodo/rlnc/full_rlnc_encoder.hpp`` is tested in
``full_rlnc_codes.cpp`` but the place-holder still exists..

The place-holder file in this cases
(``test/src/test_full_rlnc_encoder.cpp``) looks like the following:

.. literalinclude:: ../../test/src/test_full_rlnc_encoder.cpp
    :language: c++
    :linenos:

Once the ``.cpp`` test file has been created we can start to implement
the unit test code. This is done with the help of the gtest framework.

The Kodo library is build using the
``parameterized-inheritance``/ ``mixin-layers`` C++ design
technique. When unit testing a layer we try to isolate it as much as
possible. To do this we typically introduce dummy layers with the sole
purpose of satisfying the layer's dependencies. To see this in action
lets look at one of the existing unit tests.

The ``storage_bytes_used`` layer is used when we want add
functionality allowing us to keep track of how many useful bytes an
encoder or decoder contains.

.. note:: In general the amount of data which can be encoded or
          decoded will be determined by the number of symbols we are
          coding and the size of every symbol in bytes (we call this
          the block size). However, in practical applications we
          sometimes do not have enough data to fill an entire
          block. In those cases we can add the storage_bytes_used
          layer to embed in every encoder and decoder the ability to
          store the number of actual data bytes.

.. literalinclude:: ../../src/kodo/storage_bytes_used.hpp
    :language: c++
    :linenos:

As seen the layer depends on two functions being provided by the
``SuperCoder`` namely the:

1. ``SuperCoder::initialize(the_factory)``
2. ``SuperCoder::block_size()``

Using our Doxygen documentation it is possible to look-up what two
purpose of the two undefined functions are
(http://bongo.steinwurf.dk/static/files/doxygen/kodo/master/html/index.html)

In this case we want to check that the state is correctly updated when
calling ``set_bytes_used`` and that the state is correctly reset when
calling ``initialize``. The following unit test code was implemented
in ``test/src/test_storage_bytes_used.cpp`` to test this:

.. literalinclude:: ../../test/src/test_storage_bytes_used.cpp
    :language: c++
    :linenos:

In the above test code we use one test helper which allows use to
easily add testing stubs to the unit test.

http://en.wikipedia.org/wiki/Test_stub

The library is called stub and is freely available under the BSD license:

https://github.com/steinwurf/stub
