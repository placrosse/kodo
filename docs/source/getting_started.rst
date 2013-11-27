.. _getting_started:

Getting Started
===============

Obtain a License for Kodo
-------------------------
Before you download or use Kodo you **MUST** first obtain a valid license.
Please note that you own all your changes; you just accept to only use the code
for research and educational purposes.
If you need to include Kodo in a commercial product you should obtain the
commercial license instead. Both licenses can be aquired by filling out the
license request form_.

.. _form: http://steinwurf.com/license/

.. _tools-needed:

Tools Needed
------------
In order to start developing using Kodo you need the following tools installed:

1. A **C++11 compliant compiler:** Kodo is a C++11 library so you will need
   a C++ compiler that supports the new C++11 standard. You
   can see a list of compilers on our buildbot page (`Steinwurf Buildbot`_).

2. **Git:** we use the git version control system for managing our libraries.

If you wish to build the Kodo examples and unit tests, it is recommended
to also install the following:

3. **Python:** needed by our build scripts. We use the Python based `waf`_
   build system.

.. _waf: https://code.google.com/p/waf/
.. _Steinwurf Buildbot: http://buildbot.steinwurf.dk


The following sections describe how to install the tools on
a number of different platforms.

Download Tools (Ubuntu or other Debian-based distributions)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Get the dependencies using the following command::

    sudo apt-get install g++ python git-core

Download Tools (Windows)
~~~~~~~~~~~~~~~~~~~~~~~~

1. **C++11 compliant compiler:** You need a working C++ compiler. We have
   tested using `Visual Studio Express 2012`_ compiler which is free of
   charge. Newer versions should also be fine.

2. **Python:** You need a working Python installation. Find the available
   download on the `Python homepage`_. If you are in doubt which version
   to install you may use the `Python 2.7.3 Windows Installer`_.

3. **Git:** There are several ways to get git on Windows. If you plan to use
   the waf build scripts to build the Kodo examples and unit tests, you should
   install the msysgit_ tool. If you do not know which version to install, you
   may select the latest version from the `msysgit downloads`_ page
   (version 1.8.x and above).

4. **TortoiseGit (Optional):**
   You can also install the latest version of TortoiseGit_ if you prefer to use
   a GUI instead of the command-line git tools. Version 1.8.1 and later should
   work fine.

.. _`Visual Studio Express 2012`:
   http://www.microsoft.com/visualstudio/eng/downloads

.. _`Python homepage`:
   http://www.python.org/download/

.. _`Python 2.7.3 Windows Installer`:
   http://www.python.org/ftp/python/2.7.3/python-2.7.3.msi

.. _msysgit:
   http://msysgit.github.com/

.. _`msysgit downloads`:
   https://code.google.com/p/msysgit/downloads/list?q=full+installer+official+git

.. _`TortoiseGit`:
   https://code.google.com/p/tortoisegit/

Download Tools (Mac OSX)
~~~~~~~~~~~~~~~~~~~~~~~~

**C++11 compliant compiler:** You need a working C++ compiler. We have
tested using `XCode`_ 4.6.2 and the Apple LLVM 4.2 compiler which can be
downloaded free of charge. Newer versions should also be fine.

Option A:
   1. Download and install `XCode`_ from the Mac App Store
   2. Open XCode Preferences/Downloads and install **Command Line Tools**

Option B:
   1. Install **Command Line Tools** without XCode from the
   `Apple Open Source download page`_


.. _`Apple Open Source download page`:
   https://developer.apple.com/opensource/

.. _`XCode`:
   https://developer.apple.com/xcode/



Download the Kodo Source Code
-----------------------------

We recommend downloading the library using git. This will allow you to
easily get new updates whenever the library is updated. In addition to
this, it is also possible to download the library as either zip or tar.gz
archives. We will describe both approaches in the following sections.

Recommended: Clone the Git Repository Using the Terminal (Linux and Mac OSX)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

1. (Optional) Create a suitable directory (e.g. 'dev') for the projects using
   the terminal::

    mkdir dev
    cd dev

2. Clone and download the Kodo library by running (this will create a
   new directory called 'kodo')::

    git clone git://github.com/steinwurf/kodo.git

Recommended: Clone the Git Repository Using TortoiseGit (Windows)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Open the directory where you want to clone the project, right-click on empty
space and select **Git Clone...** from the context menu. The TortoiseGit clone
dialog will appear, copy this to the URL field::

    https://github.com/steinwurf/kodo.git

You can also customize the name of the target directory if you wish.
By default, a new folder called 'kodo' will be created when the progress bar
reaches the end.


Alternative: Download a Release of the Source Code as an Archive
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Download the zip or tar.gz archive from the following location and
extract it to a desired location.

* https://github.com/steinwurf/kodo/tags


Waf (Build System)
------------------

We use the Waf build system to build all Kodo examples and
unit tests. Waf is a Python-based build system that supports
a wide variety of use cases. You may read more about Waf at
the project homepage: http://code.google.com/p/waf/

A Waf project typically contains two types of files:

1. The ``waf`` file is the actual build system executable.
   This binary file is not meant to be edited.

2. Several ``wscript`` and ``wscript_build`` files: These files contain the
   project build information. You can think of it as a type
   of ``makefile`` written in Python.

.. note:: See the ``waf`` build options by running ``python waf -h``
          in your terminal.

In the following, we will look at how you can build Kodo's examples, benchmarks
and unit tests using the Waf build system.

Quick Start (Building Kodo Examples and Unit Tests)
---------------------------------------------------

.. _quick-start:

If you are primarily interested in quickly trying some Kodo examples
or building the unit tests, we have tried to make that as easy as possible.
Provided that you have the `Tools Needed`_ installed.

.. note:: We recommend trying to build and run the unit tests before
          using Kodo in your own project. However, if you want to skip this step
          you may jump directly to :ref:`including-kodo-in-your-application`

1. Navigate to the directory where you have downloaded the Kodo source code::

    cd dev/kodo/

2. Invoke ``waf`` to build the Kodo unit tests and examples::

    python waf configure

   The ``waf configure`` command ensures that all tools needed by Kodo are
   available and prepares to build Kodo. This step will also download
   several auxiliary libraries (see :ref:`kodo-dependencies`) into a
   folder called ``bundle_dependencies`` within the kodo folder.

   .. note:: The ``waf configure`` step might take several minutes depending on
             the speed of your Internet connection. This would be a
             good time to grab a coffee or similar while the dependencies are
             downloaded. If you do not have an Internet connection you can see
             the :ref:`including-kodo-in-your-application` section which shows
             how to manually download and specify the Kodo dependencies.

   .. note:: You can specify the ``--bundle=ALL`` option to instruct waf
             to download all dependencies. You can also use the
             ``--bundle-path`` option to specify the download location for the
             dependencies (if you want to change the default location).

             On Linux and Mac OSX::

               python waf configure --bundle-path=~/dev/bundle_dependencies


             On Windows, you can also specify the ``bundle-path`` as an absolute
             path with a drive letter, for example::


                python waf configure --bundle-path=C:\dev\bundle_dependencies


   .. note:: If you downloaded the library as a zip archive and you have not
             setup git to automatically authenticate against github.com you
             might see an error similar to the following::

               Resolve dependency sak: Command ['C:\\Program Files (x86)\\Git\\
               bin\\git.exe', 'clone', 'git@github.com:steinwurf/sak.git',
               'C:\\dev\\bundle_dependencies\\sak-2baed8\\master'] returned 128


             In that case, you can instruct the waf tools to use the ``https``
             protocol for downloading the dependencies by adding the following
             option ``--git-protocol=https://`` to the ``configure`` command::


                python waf configure --git-protocol=https://


3. Invoke ``waf`` to build the unit tests and examples::

    python waf build

4. Run the Kodo unit tests::

    python waf --options=run_tests,run_always

   You can also run the produced executables by specifying their relative path
   (this will vary depending on the host platform):

   a. **Linux**: Run unit tests from the Kodo directory by running
      ``./build/linux/test/kodo_tests`` in your shell.

   b. **Mac**: Run unit tests from the Kodo directory by running
      ``./build/darwin/test/kodo_tests`` in your shell.

   c. **Windows**: Run unit tests from the Kodo directory by running
      ``build/win32/test/kodo_tests.exe`` in your command prompt.

Customize the Build
-------------------

You may also customize the build process. The list below describes the most
common changes you may want for your build.

* Changing the compiler

  To change compiler, set the ``CXX`` variable to your preferred compiler.
  In case you want to use clang++, add ``CXX=clang++`` in front of
  ``waf configure``::

    CXX=clang++ python waf configure

  .. note:: ``clang++`` is currently the default compiler on Mac OSX, therefore
            this customization step is not needed on that operating system.