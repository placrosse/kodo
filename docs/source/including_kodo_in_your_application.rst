.. _including-kodo-in-your-application:

Including Kodo in Your Application
==================================
Following sections describe what you need to do to include Kodo in your
application / project, and how to handle the Kodo dependencies using our
build system.

.. contents:: Table of Contents
   :local:

Including Kodo and Its Dependencies
------------------------------------
Kodo is a header only library. This means that both Kodo and its dependencies
needs to be included in the application which is to use Kodo.

The easiest way to do so is by using our build system. You can find more
information about how to do so in :ref:`this tutorial<hello_kodo>`.

If for some reason this approach is not feasible for you. The following will
guide you through one of the many other ways you can include Kodo in your
application.

.. note:: If you have issues please double check you have all the requirements
          specified in the :ref:`getting_started` section. And if so write the
          `mailing list <http://groups.google.com/group/steinwurf-dev>`_, and
          we'll be happy to help.

#. If you haven't already, start by cloning the Kodo repository to a folder of
   your choice::

    cd folder/of/our/choice
    git clone https:://github.com/steinwurf/kodo

#. Now change directory to the kodo repository::

    cd kodo

#. Configure kodo using our build system. The Kodo build system is capable of
   automatically downloading its dependencies. This is further elaborated in
   `Kodo Dependency Management`_::

     python waf configure

#. Assuming everything went as planned, you should now have a folder called
   ``bundle_dependencies`` in your kodo repository. You are now ready to build
   Kodo and it's dependencies::

      python waf build

#. And just for good measure, run the unit tests::

      python waf --options=run_tests,run_always

#. You should now have what's needed to include Kodo in your application. Lets
   use one of the examples from Kodo in place of your application.
   Change the directory to the decode_encode_simple example::

      cd examples/decode_encode_simple

#. Compile the example using the following (rather long) commandline::

      g++ \
      -O2 \
      -ftree-vectorize \
      -Wextra \
      -Wall \
      -std=c++0x \
      -pedantic \
      -finline-functions \
      -Wno-inline \
      -Wno-long-long \
      -I../../src \
      -I../../build/linux/bundle_dependencies/boost-91e411/1.5.0 \
      -I../../build/linux/bundle_dependencies/cpuid-a4173a/3.1.0/src \
      -I../../build/linux/bundle_dependencies/fifi-f85dcd/13.0.0/src \
      -I../../build/linux/bundle_dependencies/platform-e774c1/1.0.0/src \
      -I../../build/linux/bundle_dependencies/sak-2baed8/12.0.0/src \
      -I../../bundle_dependencies/boost-91e411/1.5.0 \
      -I../../bundle_dependencies/cpuid-a4173a/3.1.0/src \
      -I../../bundle_dependencies/fifi-f85dcd/13.0.0/src \
      -I../../bundle_dependencies/platform-e774c1/1.0.0/src \
      -I../../bundle_dependencies/sak-2baed8/12.0.0/src \
      -DBOOST_ALL_NO_LIB=1 \
      -DBOOST_DETAIL_NO_CONTAINER_FWD \
      -DBOOST_NO_CXX11_NOEXCEPT \
      decode_simple.cpp \
      -c \
      -o decode_simple.cpp.1.o

#. And finally link the example using the following command::

      g++ \
      -s decode_simple.cpp.1.o \
      -o decode_simple \
      -Wl,-Bstatic \
      -L../../build/linux/bundle_dependencies/fifi-f85dcd/13.0.0/src/fifi \
      -L../../build/linux/bundle_dependencies/cpuid-a4173a/3.1.0/src/cpuid \
      -L../../build/linux/bundle_dependencies/sak-2baed8/12.0.0/src/sak \
      -lfifi \
      -lcpuid \
      -lsak \
      -Wl,-Bdynamic


Using a Makefile
----------------

If you would like to see an example of building an application with
Kodo without using our build system, we provide a small makefile
which shows how to invoke the ``g++`` compiler. The example can be found
in the ``examples/sample_makefile`` folder in the `Kodo repository`_.

.. _`Kodo repository`: https://github.com/steinwurf/kodo

By default, the example makefile assumes that the required libraries are
downloaded side-by-side with Kodo itself.
To achieve this, you can clone the projects in the same directory::

    git clone https://github.com/steinwurf/cpuid.git
    git clone https://github.com/steinwurf/boost.git
    git clone https://github.com/steinwurf/fifi.git
    git clone https://github.com/steinwurf/platform.git
    git clone https://github.com/steinwurf/sak.git
    git clone https://github.com/steinwurf/kodo.git

Then you can build this example::

    cd kodo/examples/sample_makefile
    make

And execute the ``example`` binary::

    ./example


.. _kodo-dependencies:

Kodo Dependency Management
--------------------------
Kodo relies on a number of external libraries, these must be available
in order to successfully compile an application including Kodo.

The easiest way to get these is by using the Kodo build scripts. It will
automatically download at setup the dependencies and build the Kodo library.

The libraries are:

#. **Cpuid**: this library contains functionality to detect CPU features.

   https://github.com/steinwurf/cpuid

#. **Boost** C++ libraries: this library contains a wide range
   of C++ utilities. We use only a subset of this functionality, such as
   smart pointers.

   https://github.com/steinwurf/boost

#. **Fifi**: this library contains
   finite field arithmetics used in ECC (Error Correcting Code) algorithms.

   https://github.com/steinwurf/fifi

#. **Platform**: this library contains functionality to detect the build
   platform.

   https://github.com/steinwurf/platform

#. **Sak**: this library contains a few
   utility functions used in Kodo such as endian conversion.

   https://github.com/steinwurf/sak

.. note:: Additional libraries, besides the ones listed above, will be
   downloaded if you use the Kodo build script. These libraries are only needed
   when/if you want to compile the Kodo unit tests or benchmarks.
   So you don't have to link with these when using Kodo in your application.

.. _selecting-the-correct-versions:

Selecting the Correct Versions
..............................
If you use the Kodo build script to build Kodo. The latest compatible version of
its dependencies will automatically be downloaded. If you download
the dependencies manually, you will have to select a compatible version
yourself. This information is stored in the ``wscript`` file found in Kodo's
root folder.

Within that file, in the ``options`` function, you will find all Kodo's
dependencies specified in the following way:

.. code-block:: python

    bundle.add_dependency(opt, resolve.ResolveGitMajorVersion(
            name='fifi',
            git_repository='github.com/steinwurf/fifi.git',
            major_version=13))

The above command sets up a dependency for the Fifi library. The version
required is specified in the ``major_version = 13`` line. This means that Kodo
requires version ``13.x.y`` of the Fifi library, where ``x.y`` should be
selected to pick the newest available version. You can get a list of available
versions by visiting the download page at GitHub for the Fifi library:

* https://github.com/steinwurf/fifi/releases

At the time of writing, the appropriate version for Fifi, when using Kodo
(master branch), would be version ``13.0.0``. Note these version numbers are
available as ``git tags`` if you choose to manually checkout the git
repositories.

.. _download-kodo-dependencies:

Download Kodo Dependencies
..........................

There are several ways in which you may get the Kodo library and its
dependencies.

#. As shown in the :ref:`getting_started` section, the Kodo build scripts
   supports downloading the dependency repositories automatically. The build
   script with do a ``git clone`` and checkout the latest compatible tagged
   version of the dependency.

#. You may wish to manually download Kodo's dependencies as separate git
   repositories, see `Download Using Git`_.

#. You can also download the Kodo dependencies as zip or tar.gz archives
   from the dependencies corresponding GitHub page, see
   `Download as zip/tar.gz archives`_.

.. note:: Downloading all dependencies is only necessary if you wish to build
          the Kodo using our build system. If you simply want to use Kodo
          in your application you only need to download the libraries listed
          under `Kodo Dependency Management`_.

Download Using Git
^^^^^^^^^^^^^^^^^^

#. Create a suitable directory for the projects (optional)::

     mkdir dev
     cd dev

#. Clone and download the libraries by running::

      git clone https://github.com/steinwurf/cpuid.git
      git clone https://github.com/steinwurf/boost.git
      git clone https://github.com/steinwurf/fifi.git
      git clone https://github.com/steinwurf/platform.git
      git clone https://github.com/steinwurf/sak.git

      git clone https://github.com/steinwurf/gauge.git
      git clone https://github.com/steinwurf/gtest.git
      git clone https://github.com/steinwurf/tables.git
      git clone https://github.com/steinwurf/waf-tools.git

Now we have to select the correct versions for all the downloaded dependencies
e.g. for Fifi, first list the available tags::

    cd fifi
    git tag -l

Using the information from the ``wscript`` (described in
`Selecting the correct versions`_) we can checkout a tagged version::

    git checkout 13.0.0

We now do this for all the downloaded repositories.

Download as zip/tar.gz archives
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Here we have to visit the download pages of the different dependencies
and download the correct versions (described in `Selecting the correct
versions`_):

#. Cpuid: https://github.com/steinwurf/cpuid/releases
#. Boost: https://github.com/steinwurf/boost/releases
#. Fifi: https://github.com/steinwurf/fifi/releases
#. Platform: https://github.com/steinwurf/platform/releases
#. Sak: https://github.com/steinwurf/sak/releases

#. Gauge: https://github.com/steinwurf/gauge/releases
#. Gtest: https://github.com/steinwurf/gtest/releases
#. Tables: https://github.com/steinwurf/tables/releases
#. Waf-tools: https://github.com/steinwurf/waf-tools/releases


Configuring Kodo With Manually Downloaded Dependencies
......................................................

After downloading all the dependencies manually, we have to inform the
Kodo build scripts to use those instead of trying to automatically downloading
them. Assuming you've located the kodo repository along side the downloaded
dependencies, this can be done using the following command::

  python waf configure --bundle=NONE \
  --cpuid-path=../cpuid \
  --boost-path=../boost \
  --fifi-path=../fifi \
  --platform-path=../platform \
  --sak-path=../sak \
  --gauge-path=../gauge \
  --gtest-path=../gtest \
  --tables-path=../tables \
  --waf-tools-path=../waf-tools

The bundle options supports a number of different use cases. The following
will bundle all dependencies but the Fifi library which we have to
manually specify a path for::

  python waf configure --bundle=ALL,-fifi --fifi-path=../fifi

Or we may bundle only Fifi::

  python waf configure --bundle=NONE,fifi \
  --cpuid-path=../cpuid \
  --boost-path=../boost \
  --platform-path=../platform \
  --sak-path=../sak \
  --gauge-path=../gauge \
  --gtest-path=../gtest \
  --tables-path=../tables \
  --waf-tools-path=../waf-tools

More libraries may be added to the ``--bundle=`` option using commas e.g.
bundle all, but Fifi and Sak::

  python waf configure --bundle=ALL,-fifi,-sak \
  --fifi-path=../fifi \
  --sak-path=../sak

The bundle options can be seen by running::

  python waf --help
