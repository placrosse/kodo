.. _hello_kodo:

Hello Kodo
----------

The purpose of this example is to show how to use our build system to build a
simple file which depends on Kodo.
The example consists of 3 files.

* ``main.cpp``,
* ``wscript``, and
* ``waf``.

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

The remaining two files are related to building Kodo. The ``waf`` file is a
build system, where as the ``wscript`` is the recipe used by ``waf`` to build
``main.cpp``.
The ``wscript`` contains information regarding dependencies and build flags.
The simplest way to get started is to run the following commands in a terminal
(using the hello_kodo example directory as your working directory).

.. code-block:: none

    python waf configure
    python waf build
    ./build/{platform}/hello_kodo

If you want, you can copy the files from this example and use them as a starting
point for the coming examples, or even your own Kodo application.

For more information regarding how to use the waf build systems, go
:ref:`here<waf_build_system>`.
