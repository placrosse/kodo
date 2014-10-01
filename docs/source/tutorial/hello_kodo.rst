.. _hello_kodo:

Hello Kodo
----------
The purpose of this example is to provide an easy starting point for people who
want to try out Kodo.
The example shows how to use our build system to build a simple executable which
depends on Kodo. The example consists of 3 files; ``main.cpp``, ``wscript``, and
``waf``.

``main.cpp`` contains of a very limited amount of code:

.. literalinclude:: ../../../examples/hello_kodo/main.cpp
    :language: c++
    :linenos:

It's basically a main function which prints ``Hello Kodo!`` and exits. In this
example, we include a particular version of a RLNC (Random Linear Network Code)
located in the following header file:

.. literalinclude:: ../../../examples/hello_kodo/main.cpp
    :language: c++
    :start-after: //! [0]
    :end-before: //! [1]
    :linenos:

The include is not used however. Its only purpose is to detect whether or not
the include paths for the Kodo library has been setup correctly.

The remaining two files are related to building the executable using Kodo.
The ``waf`` file is a `complete stand-alone build system
<https://code.google.com/p/waf/>`_, whereas the ``wscript`` is the recipe used by
``waf`` to build ``main.cpp``.
The ``wscript`` contains information regarding dependencies and build flags.
The simplest way to get started is to copy the files to a folder of your own
choosing, and then run the following commands in a terminal (while using the
selected folder as your working directory).

.. code-block:: none

    python waf configure
    python waf build
    ./build/{platform}/hello_kodo

You can use this as a starting point for the coming examples, or even your own
Kodo application.

For more information regarding how to use the waf build systems, go
:ref:`here<waf_build_system>`.
