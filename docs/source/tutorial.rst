.. _tutorial:

Tutorial
========

Include tutorial here...


Adding code samples
-------------------

To add code samples, to like so

one

.. code-block:: c++

    .. include:: ../../examples/tutorial/basic.cpp
        :start-after: main()
        :end-before: return 0

two

.. include:: ../../examples/tutorial/basic.cpp
    :code: c++

We can make the inclusion dynamic. For instance this includes the code after the
first occurence of main() and until the first occurrence of return0;. Maybe we
should add some comments so that we can search for when including tutorial
files.

three

.. literalinclude:: ../../examples/tutorial/basic.cpp
   :language:c++

four

.. literalinclude:: ../../examples/tutorial/basic.cpp

five

.. code-block:: c++
.. include:: ../../examples/tutorial/basic.cpp
   :start-after: main()
   :end-before: return 0

six

.. include:: ../../examples/tutorial/basic.cpp
   :code: xml

.. source: http://docutils.sourceforge.net/docs/ref/rst/directives.html#including-an-external-document-fragment
