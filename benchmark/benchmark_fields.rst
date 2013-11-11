Data indices
============

Description of the index keys in the benchmark data in the mongodb database, be
aware that not all keys are present in all benchmarks.

For an introduction to mongodb see these tutorials:

 * http://api.mongodb.org/python/current/tutorial.html
 * http://blog.pythonisito.com/2012/01/moving-along-with-pymongo.html

Currently our mongoDB is runnings at 176.28.49.184 and you can access data with
the user: "guest" and the password "none"

the benchmaring database is named **benchmark** and currently contain the
following **collections**

 * kodo_count_operations
 * kodo_decoding_probability
 * kodo_overhead
 * kodo_throughput


benchmark
  (from gauge) The name of a the sub-testcase specific benchmark

branch
  (git related)

buildername
  (buildbot related) name of the buildslave, a concatenation project name,
  buildplatform and python version

buildnumber
  (buildbot) the build number

commit
  (git related) The git commit id

cxx_mkspec
  (buildbot related) the mkspec used on the buildslave

iterations
  the number of iterations in each subtest

platform
  (buildbot related) platform name on the buildslave

run_number
  (buildbot related)

scheduler
  (buildbot related) reason for the benchmark triggering

slavename
  (buildbot related)

symbol_size
  (RLNC related) the symbol size used

symbols
  (RLNC related) the generation (or block) size

testcase
  (from gauge) The name of the overall testcast

throughput
  The throughput measured only set for throughput benchmarks

type
  (from gauge)

unit
  The unit of the measured values

utc_date
  timestamp
