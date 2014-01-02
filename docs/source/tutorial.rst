.. _tutorial:

Tutorial
========

Include tutorial here...


Adding code samples
-------------------

This is an example of an included code sample.

.. literalinclude:: ../../examples/tutorial/basic.cpp
    :language: c++
    :start-after: main()
    :end-before: return 0;
    :emphasize-lines: 12,15-18
    :linenos:

We should consider only using start-after and end-before as this would allow the
documentation to change with the code.
By using linenos and emphasize-lines we can possibly introduces bugs in the
docs as they are line dependent.

.. source: http://sphinx-doc.org/markup/code.html#includes
