
Mixin-Layers / Parameterized Inheritance
........................................

In this context a mixin is not a stand-alone class but represents a
"slice" of functionality.  The slices are then combined to compose the
final functionality.  In C++ we do this through templates.  Kodo
heavily relies on this technique.

* Flexibility
* Performance

Template
^^^^^^^^

.. code-block:: c++
    :linenos:

    template<class T>
    class foo
    {
    public:
        T m_t;
    };

    foo<int> f1;
    foo<my_other_type> f2;

Mixin-Layers
^^^^^^^^^^^^

.. code-block:: c++
    :linenos:

    template<class Super>
    class add_layer : public Super
    {
    public:

        int add(int a, int b)
        {
            return a + b;
        }
    };

    class final_layer
    { };

    class calculator
        : public add_layer<final_layer> >
    { };

Main

.. code-block:: c++
    :linenos:

    int main()
    {
        calculator calc;
        std::cout << calc.add(4,2) << std::endl;
        return 0;
    }

output
6

Adding functionality

.. code-block:: c++
    :linenos:

    template<class Super>
    class sub_layer : public Super
    {
    public:

        int subtract(int a, int b)
        {
            return a - b;
        }
    };

    class calculator
        : public sub_layer<
                     add_layer<final_layer> >
    { };

Main2

.. code-block:: c++
    :linenos:

    int main()
    {
        calculator calc;
        std::cout << calc.add(4,2) << std::endl;
        std::cout << calc.subtract(2,5) << std::endl;
        return 0;
    }

output
6
-3

Customization of layers

.. code-block:: c++
    :linenos:

    template<class Super>
    class modulo_layer : public Super
    {
    public:

        int add(int a, int b)
        {
            return Super::add(a,b) % 5;
        }

        int subtract(int a, int b)
        {
            int res =
                Super::subtract(a,b) % 5;
            return res < 0 ? res + 5 : res;
        }
    };

    class calculator
        : public modulo_layer<
                 sub_layer<
                 add_layer<final_layer> > >
    { };


Main3

.. code-block:: c++
    :linenos:

    int main()
    {
        calculator calc;
        std::cout << calc.add(2,2) << std::endl;
        std::cout << calc.subtract(2,2) << std::endl;
        return 0;
    }

output

1
2