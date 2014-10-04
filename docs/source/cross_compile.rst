.. _cross_compile:

Cross-compilation and tool options
==================================

This section describes the waf configuration options that are used to
cross-compile Kodo for different platforms and to change some properties
of your builds.

Generic tool options
--------------------

- cxx_debug
By default, our build system will remove all debugging info from the generated
binaries. You can enable the debugging symbols with the ``cxx_debug`` option::

    python waf configure --options=cxx_debug

- run_tests
You can use this option to run the unit tests after your build is completed::

    python waf build --options=run_tests

- run_benchmark
You can use this option to run a specific benchmark after your build is
completed::

    python waf build --options=run_benchmark=my_benchmark

- run_always
This option is used in conjunction with run_tests and run_benchmark to
always run the given targets, even if waf already performed this task after
a successful build::

    python waf build --options=run_tests,run_always


Cross-compilation
-----------------

The tool options are also useful to specify makespecs for given toolchains
that can compile binaries for different platforms. The makespecs are also
handy when you want to select a specific compiler version or CPU architecture.

Different makespecs are available on different operating systems. You can
get the list of the currently supported makespecs with the config helper script
located in the kodo root folder::

    python config.py

This helper script automatically updates itself when you run it. You can use
this script to go through the common configuration options without typing
too much.


Android
.......
You need a standalone Android toolchain to compile for Android. You can follow
the instructions in our `Android guide`_ to quickly create a toolchain using
the Android NDK.

You can also download a toolchain for your platform from this page:
http://bongo.steinwurf.dk/files/toolchains

You also need to have the Android SDK, because we need to find the ``adb`` tool
during the configure step. The easiest solution is to add the path to ``adb``
and the ``bin`` folder of the standalone toolchain to your PATH. For example,
you can add the following lines to your ``~/.profile`` (please adjust the
paths to match your folder names and locations)::

    PATH="$PATH:$HOME/toolchains/android-sdk-linux-r22.6/platform-tools"
    PATH="$PATH:$HOME/toolchains/arm-linux-androideabi-4.8-r10/bin"

You need to log in again or open a new terminal to get the updated PATH.
You can check that the required binaries are really in your PATH with these
commands::

    adb version
    arm-linux-androideabi-g++ --version

Once you have everything in your PATH, use the following mkspec when you
configure Kodo (you may also select another Android mkspec if available
in the list provided by ``config.py``)::

    python waf configure --options=cxx_mkspec=cxx_android_gxx48_arm

The configure command should find your toolchain and the necessary binaries,
and you can build the codebase as usual after this::

    python waf build

You can find the generated Android binaries in the
``build/cxx_android_gxx48_arm`` folder. You can transfer these binaries to your
Android device with adb. Read our `Android guide`_ for more information on this.

If you don't want to add the Android toolchains to your PATH, then we also
provide explicit options to specify these folders during the configure step.
Here is an example for that::

    python waf configure --options=cxx_mkspec=cxx_android_gxx48_arm,\
    android_sdk_dir=~/toolchains/android-sdk-linux-r22.6,\
    android_ndk_dir=~/toolchains/arm-linux-androideabi-4.8-r10

.. _Android guide: https://github.com/steinwurf/steinwurf-labs/blob/master/docs/android-c-application.rst


iOS
...
You need to install the latest XCode to compile for iOS. Please make sure
that you also have the Apple command-line tools in your PATH by executing
the following command on OSX Mavericks::

    xcode-select --install

XCode installs the iOS SDK to a standard location, so you only need to specify
the iOS mkspec when you configure (please note that the version numbers in
the name of the mkspec may change, so use ``config.py`` to list the currently
available versions)::

    python waf configure --options=cxx_mkspec=cxx_ios50_apple_llvm60_armv7

Then you can build Kodo as usual::

    python waf build

You can find the generated iOS binaries in the
``build/cxx_ios50_apple_llvm60_armv7`` folder. You can transfer these binaries
to your iOS device with any tool you like. Please note that these are
command-line binaries, so you will need a terminal application to run them.


Raspberry Pi
............
You can download our pre-built Raspberry Pi toolchain for 64-bit Linux here:
http://buildbot.steinwurf.dk:12344/toolchains/linux/

Extract ``raspberry-gxx47-arm.zip`` to a folder of your liking. You also need
to add the `bin`` folder of the Raspberry toolchain to your PATH. For example,
you can add the following lines to your ``~/.profile`` (please adjust the
paths to match your folder names and locations)::

    PATH="$PATH:$HOME/toolchains/raspberry-gxx47-arm/bin"

You need to log in again or open a new terminal to get the updated PATH.
You can check that the required binaries are in your PATH with this command::

    raspberry-gxx47-arm-g++ --version

configure Kodo with the following mkspec::

    python waf configure --options=cxx_mkspec=cxx_raspberry_gxx47_arm

The configure command should find your toolchain binaries,
and you can build the codebase as usual after this::

    python waf build

You can find the generated binaries in the
``build/cxx_raspberry_gxx47_arm`` folder. You can transfer these binaries
to your Raspberry Pi with any tool you like (e.g. SCP).


OpenWRT
.......
You should build a compatible OpenWRT toolchain for your target device.
Instructions to do that...


Other toolchains
................
Other toolchains might also work if you specify your custom compiler with
the CXX variable when you configure Kodo::

    CXX=/path/to/custom/compiler/g++ python waf configure

This compiler must have a recognizable name (e.g. it contains the ``g++``
string) and waf must be able to determine its version to accept it.
