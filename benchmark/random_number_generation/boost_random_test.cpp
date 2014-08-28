// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cstdint>

#include <gauge/gauge.hpp>
#include <gauge/console_printer.hpp>

#include <boost/random/mersenne_twister.hpp>         //mt11213b and mt19937
#include <boost/random/shuffle_order.hpp>            //knuth_b and kreutzer1986
#include <boost/random/taus88.hpp>                   //taus88
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#define BUFFER_SIZE 512

namespace testspace
{

    // The benchmark the tests will use.
    // Take a random number generator as a template where this could be any
    // RNG. Here several RNG from the Boost Library are used.
    template<class RandomNumberGenerator, class ValueType>
    class random_number_benchmark  : public gauge::time_benchmark
    {
    public:
        double measurement()
        {
            // Get the time spent per iteration
            double time = gauge::time_benchmark::measurement();


            // The size in bytes bytes of random numbers generated
            uint64_t total_bytes = BUFFER_SIZE;

            return total_bytes / time; // MB/s for each iteration
        }

        std::string unit_text() const
        {
            return "MB/s";
        }

        // Fills a buffer with random numbers.
        // In this banchmark it is defines to be 512.
        void generate(uint8_t *buffer)
        {
            RUN
            {
                assert(buffer != 0);

                uint32_t size = BUFFER_SIZE;
                uint32_t numbers_per_iteration = sizeof(ValueType)/sizeof(uint8_t);

                // printf("The value is %u\n", numbers_per_iteration);
                for(uint32_t i = 0; i < size; i += numbers_per_iteration)
                {
                    buffer[i] = m_distribution(m_random_generator);
                }
            }
        }

    private:

        /// The type of the distribution
        using distribution =  boost::random::uniform_int_distribution<ValueType>;

        /// Distribution that generates random bytes
        distribution m_distribution;

        /// The random generator
        RandomNumberGenerator m_random_generator;

    };

}

////////////////////////////////////////////////////////////
// The taus88 benchmark
////////////////////////////////////////////////////////////
using taus88_uint8_benchmark =
    testspace::random_number_benchmark<boost::random::taus88, uint8_t>;

BENCHMARK_F(taus88_uint8_benchmark, taus88, uint8, 1000)
{
    std::vector<uint8_t> buffer;
    buffer.resize(BUFFER_SIZE);

    generate(buffer.data());

}

using taus88_uint16_benchmark =
    testspace::random_number_benchmark<boost::random::taus88, uint16_t>;

BENCHMARK_F(taus88_uint16_benchmark, taus88, uint16, 1000)
{
    std::vector<uint8_t> buffer;
    buffer.resize(BUFFER_SIZE);

    generate(buffer.data());

}

using taus88_uint32_benchmark =
    testspace::random_number_benchmark<boost::random::taus88, uint32_t>;

BENCHMARK_F(taus88_uint32_benchmark, taus88, uint32, 1000)
{
    std::vector<uint8_t> buffer;
    buffer.resize(BUFFER_SIZE);

    generate(buffer.data());
}

using taus88_uint64_benchmark =
    testspace::random_number_benchmark<boost::random::taus88, uint64_t>;

BENCHMARK_F(taus88_uint64_benchmark, taus88, uint64, 1000)
{
    std::vector<uint8_t> buffer;
    buffer.resize(BUFFER_SIZE);

    generate(buffer.data());
}

////////////////////////////////////////////////////////////
// The mt19937 benchmark
////////////////////////////////////////////////////////////
using mt19937_uint8_benchmark =
    testspace::random_number_benchmark<boost::random::mt19937, uint8_t>;

BENCHMARK_F(mt19937_uint8_benchmark, mt19937, uint8, 1000)
{
    std::vector<uint8_t> buffer;
    buffer.resize(BUFFER_SIZE);

    generate(buffer.data());


}

using mt19937_uint16_benchmark =
    testspace::random_number_benchmark<boost::random::mt19937, uint16_t>;

BENCHMARK_F(mt19937_uint16_benchmark, mt19937, uint16, 1000)
{
    std::vector<uint8_t> buffer;
    buffer.resize(BUFFER_SIZE);

    generate(buffer.data());


}

using mt19937_uint32_benchmark =
    testspace::random_number_benchmark<boost::random::mt19937, uint32_t>;

BENCHMARK_F(mt19937_uint32_benchmark, mt19937, uint32, 1000)
{
    std::vector<uint8_t> buffer;
    buffer.resize(BUFFER_SIZE);

    generate(buffer.data());
}

using mt19937_uint64_benchmark =
    testspace::random_number_benchmark<boost::random::mt19937, uint64_t>;

BENCHMARK_F(mt19937_uint64_benchmark, mt19937, uint64, 1000)
{
    std::vector<uint8_t> buffer;
    buffer.resize(BUFFER_SIZE);

    generate(buffer.data());


}

////////////////////////////////////////////////////////////
// The mt19937_64 benchmark
////////////////////////////////////////////////////////////

using mt19937_64_uint8_benchmark =
    testspace::random_number_benchmark<boost::random::mt19937_64, uint8_t>;

BENCHMARK_F(mt19937_64_uint8_benchmark, mt19937_64, uint8, 1000)
{
    std::vector<uint8_t> buffer;
    buffer.resize(BUFFER_SIZE);

    generate(buffer.data());


}

using mt19937_64_uint16_benchmark =
    testspace::random_number_benchmark<boost::random::mt19937_64, uint16_t>;

BENCHMARK_F(mt19937_64_uint16_benchmark, mt19937_64, uint16, 1000)
{
    std::vector<uint8_t> buffer;
    buffer.resize(BUFFER_SIZE);

    generate(buffer.data());


}

using mt19937_64_uint32_benchmark =
    testspace::random_number_benchmark<boost::random::mt19937_64, uint32_t>;

BENCHMARK_F(mt19937_64_uint32_benchmark, mt19937_64, uint32, 1000)
{
    std::vector<uint8_t> buffer;
    buffer.resize(BUFFER_SIZE);

    generate(buffer.data());
}

using mt19937_64_uint64_benchmark =
    testspace::random_number_benchmark<boost::random::mt19937_64, uint64_t>;

BENCHMARK_F(mt19937_64_uint64_benchmark, mt19937_64, uint64, 1000)
{
    std::vector<uint8_t> buffer;
    buffer.resize(BUFFER_SIZE);

    generate(buffer.data());


}

////////////////////////////////////////////////////////////
// The knuth_b benchmark
////////////////////////////////////////////////////////////
using knuth_b_uint8_benchmark =
    testspace::random_number_benchmark<boost::random::knuth_b, uint8_t>;

BENCHMARK_F(knuth_b_uint8_benchmark, knuth_b, uint8, 1000)
{
    std::vector<uint8_t> buffer;
    buffer.resize(BUFFER_SIZE);

    generate(buffer.data());
}

using knuth_b_uint16_benchmark =
    testspace::random_number_benchmark<boost::random::knuth_b, uint16_t>;

BENCHMARK_F(knuth_b_uint16_benchmark, knuth_b, uint16, 1000)
{
    std::vector<uint8_t> buffer;
    buffer.resize(BUFFER_SIZE);

    generate(buffer.data());
}

using knuth_b_uint32_benchmark =
    testspace::random_number_benchmark<boost::random::knuth_b, uint32_t>;

BENCHMARK_F(knuth_b_uint32_benchmark, knuth_b, uint32, 1000)
{
    std::vector<uint8_t> buffer;
    buffer.resize(BUFFER_SIZE);

    generate(buffer.data());
}

using knuth_b_uint64_benchmark =
    testspace::random_number_benchmark<boost::random::knuth_b, uint64_t>;

BENCHMARK_F(knuth_b_uint64_benchmark, knuth_b, uint64, 1000)
{
    std::vector<uint8_t> buffer;
    buffer.resize(BUFFER_SIZE);

    generate(buffer.data());
}

////////////////////////////////////////////////////////////
// The mt11213b benchmark
////////////////////////////////////////////////////////////
using mt11213b_uint8_benchmark =
    testspace::random_number_benchmark<boost::random::mt11213b, uint8_t>;

BENCHMARK_F(mt11213b_uint8_benchmark, mt11213b, uint8, 1000)
{
    std::vector<uint8_t> buffer;
    buffer.resize(BUFFER_SIZE);

    generate(buffer.data());

}

using mt11213b_uint16_benchmark =
    testspace::random_number_benchmark<boost::random::mt11213b, uint16_t>;

BENCHMARK_F(mt11213b_uint16_benchmark, mt11213b, uint16, 1000)
{
    std::vector<uint8_t> buffer;
    buffer.resize(BUFFER_SIZE);

    generate(buffer.data());

}

using mt11213b_uint32_benchmark =
    testspace::random_number_benchmark<boost::random::mt11213b, uint32_t>;

BENCHMARK_F(mt11213b_uint32_benchmark, mt11213b, uint32, 1000)
{
    std::vector<uint8_t> buffer;
    buffer.resize(BUFFER_SIZE);

    generate(buffer.data());

}

using mt11213b_uint64_benchmark =
    testspace::random_number_benchmark<boost::random::mt11213b, uint64_t>;

BENCHMARK_F(mt11213b_uint64_benchmark, mt11213b, uint64, 1000)
{
    std::vector<uint8_t> buffer;
    buffer.resize(BUFFER_SIZE);

    generate(buffer.data());

}

////////////////////////////////////////////////////////////
// The kreutzer1986 benchmark
////////////////////////////////////////////////////////////
using kreutzer1986_uint8_benchmark =
    testspace::random_number_benchmark<boost::random::kreutzer1986, uint8_t>;

BENCHMARK_F(kreutzer1986_uint8_benchmark, kreutzer1986, uint8, 1000)
{
    std::vector<uint8_t> buffer;
    buffer.resize(BUFFER_SIZE);

    generate(buffer.data());

}

using kreutzer1986_uint16_benchmark =
    testspace::random_number_benchmark<boost::random::kreutzer1986, uint16_t>;

BENCHMARK_F(kreutzer1986_uint16_benchmark, kreutzer1986, uint16, 1000)
{
    std::vector<uint8_t> buffer;
    buffer.resize(BUFFER_SIZE);

    generate(buffer.data());

}

using kreutzer1986_uint32_benchmark =
    testspace::random_number_benchmark<boost::random::kreutzer1986, uint32_t>;

BENCHMARK_F(kreutzer1986_uint32_benchmark, kreutzer1986, uint32, 1000)
{
    std::vector<uint8_t> buffer;
    buffer.resize(BUFFER_SIZE);

    generate(buffer.data());

}

using kreutzer1986_uint64_benchmark =
    testspace::random_number_benchmark<boost::random::kreutzer1986, uint64_t>;

BENCHMARK_F(kreutzer1986_uint64_benchmark, kreutzer1986, uint64, 1000)
{
    std::vector<uint8_t> buffer;
    buffer.resize(BUFFER_SIZE);

    generate(buffer.data());

}

////////////////////////////////////////////////////////////
// The minstd_rand benchmark
////////////////////////////////////////////////////////////
using minstd_rand_uint8_benchmark =
    testspace::random_number_benchmark<boost::random::minstd_rand, uint8_t>;

BENCHMARK_F(minstd_rand_uint8_benchmark, minstd_rand, uint8, 1000)
{
    std::vector<uint8_t> buffer;
    buffer.resize(BUFFER_SIZE);

    generate(buffer.data());
}

using minstd_rand_uint16_benchmark =
    testspace::random_number_benchmark<boost::random::minstd_rand, uint16_t>;

BENCHMARK_F(minstd_rand_uint16_benchmark, minstd_rand, uint16, 1000)
{
    std::vector<uint8_t> buffer;
    buffer.resize(BUFFER_SIZE);

    generate(buffer.data());
}

using minstd_rand_uint32_benchmark =
    testspace::random_number_benchmark<boost::random::minstd_rand, uint32_t>;

BENCHMARK_F(minstd_rand_uint32_benchmark, minstd_rand, uint32, 1000)
{
    std::vector<uint8_t> buffer;
    buffer.resize(BUFFER_SIZE);

    generate(buffer.data());
}

using minstd_rand_uint64_benchmark =
    testspace::random_number_benchmark<boost::random::minstd_rand, uint64_t>;

BENCHMARK_F(minstd_rand_uint64_benchmark, minstd_rand, uint64, 1000)
{
    std::vector<uint8_t> buffer;
    buffer.resize(BUFFER_SIZE);

    generate(buffer.data());
}
