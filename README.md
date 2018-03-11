# Interval Stabbing using the AP
This repository contains code for finding the query points that *stab* (or overlap) the given intervals.
First, given a set of intervals, precompiled macros are used for programming the intervals on the AP board.
Then, all the points to be checked are streamed to the board and the reports generated by the board are used
to determine the intervals stabbed by every point.

## Requirements
* Following two submodules:
  * [comparators](@ref md_comparators_README_comparators)
  * [apsdk](@ref md_apsdk_README_apsdk)
* **gcc** (with C++11 support)  
This project has been test built only on Linux platform, using gcc with C++11 support.
* **AP SDK** (*v1.6* or *v1.7*)  
The SDK can be downloaded from the following links: [Windows](https://drive.google.com/open?id=0B7SP70jwzihbSHRGMFhmUVQ2MG8),
 [Ubuntu](https://drive.google.com/open?id=0B7SP70jwzihbbzJnN3Y0Zmd1N2c),
 and [CentOS](https://drive.google.com/open?id=0B7SP70jwzihbVUlydlR5a0NwbUE).   
Please note that the provided links to the SDK are for the purpose of evaluation of this work and should not be distributed without the permission of Micron, Inc.
* **Boost**  
Boost libraries are used for parsing the command line options and a few other purposes.  
More information about Boost libraries can be found at [http://boost.org/](http://boost.org/).
* **SCons**  
SCons, a cross-platform Python based build environment, is required for building the project.  
More information about the tool can be found at [http://scons.org/](http://scons.org/).

## Building
After all the dependencies have been installed, the project can be built as:
<pre><code>scons
</code></pre>
This will create an executable named `stab-intervals`, which can be used for, well, stabbing intervals. The default option is to build against AP SDK v1.7.
For building against v1.6, the following can be executed instead:
scons APSDKVERSION=16
</code></pre>
Further, path to Boost libraries can be specified as:
<pre><code>scons BOOSTLIBPATH=&lt;Boost library path, default is /usr/lib/x86_64-lib-gnu&gt;
</code></pre>

For building the debug version of the executable, following can be executed:
<pre><code>scons DEBUG=1
</code></pre>
Debug version of the executable is named `stab-intervals-debug`.

## Execution
Once the project has been built, the application can be used with any combination of user provided or random intervals and points. The executable accepts the following arguments:
<pre><code>-h [ --help ]                         Print this message.
-d [ --device ] arg                   Name of the AP device to be used for
                                      stabbing intervals.
-i [ --intervals ] arg                Name of the file from which intervals
                                      are to be read.
-p [ --points ] arg                   Name of the file from which points are
                                      to be read.
-b [ --bytes ] arg (=4)               Number of bytes.
-s [ --seed ] arg (=0)                Seed for random number generator.
-I [ --random-intervals ] arg (=0)    Number of random intervals to be
                                      programmed.
-P [ --random-points ] arg (=0)       Number of random points to be used for
                                      stabbing.
-c [ --chunks ] arg                   Maximum chunk size for flows to the AP.
--real                                Use real numbers for labeling.
--signed                              Use signed numbers for labeling.
</code></pre>
The application assumes unsigned 4-byte integer intervals, unless specified otherwise using  the options `--bytes=8` for 8-byte numbers, `--signed` for signed numbers, and/or `--real` for real numbers.

An ANML file and an AP-FSM, corresponding to the automaton to be programmed on the AP board, are generated for the provided intervals. If the AP device name is not provided, or if it can not be opened, the application exits.
However, if the application finds an AP device, the AP-FSM is loaded on the device and a flow constructed from all the points is streamed to the device. The application then reports all the intervals stabbed by every point, using the reports generated by the device.


### Example1

<pre><code>./stab-intervals -d /dev/fri0 -i intervals.txt -p points.txt
</code></pre>
The above command will read intervals from the file named `intervals.txt` and points from the file named `points.txt`. A sample of both the files is shown below.
<pre><code>intervals.txt
-------------
1000 1100
1100 1200
...
...

points.txt
----------
1005
994
...
...
</code></pre>


### Example 2

<pre><code>./stab-intervals -d /dev/fri0 -I 100 -P 1000
</code></pre>
This will generate and print out 100 random intervals and 1000 random points and then use them for comparison.

## Publications
* Roy, Indranil, Ankit Srivastava, Matt Grimm, and Srinivas Aluru. "Interval Stabbing on the Automata Processor." _Journal of Parallel and Distributed Computing_ (2018).
* Roy, Indranil, Ankit Srivastava, Matt Grimm, and Srinivas Aluru. "Parallel Interval Stabbing on the Automata Processor." In _Irregular Applications: Architecture and Algorithms (IA3), Workshop on_, pp. 10-17. IEEE, 2016.
