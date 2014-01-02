.. _tutorial:

Tutorial
========

Include tutorial here...


Adding code samples
-------------------

To add code samples, to like so:

.. include:: ../../examples/tutorial/basic.cpp
    :start-line: 7
    :end-line: 37
    :code: c++

We can make the inclusion dynamic. For instance this includes the code after the
first occurence of main() and until the first occurrence of return0;. Maybe we
should add some comments so that we can search for when including tutorial
files.

.. include:: ../../examples/tutorial/basic.cpp
   :start-after: main()
   :end-before: return 0
   :code: c++

.. source: http://docutils.sourceforge.net/docs/ref/rst/directives.html#including-an-external-document-fragment