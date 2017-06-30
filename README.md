Benchmark
=========

Description
-----------

C++ benchmarks

Results
-------

[https://vovaprog.github.io/benchmark](https://vovaprog.github.io/benchmark)

Build and run
-------------

Install required packages. For debian/ubuntu:<br/>
apt-get install g++ cmake libboost-dev libboost-filesystem-dev<br/>
apt-get install markdown python-matplotlib<br/>

Create build directory, enter it and run commands:<br/>

cmake -DCMAKE\_BUILD\_TYPE=Release /path/to/benchmark/source/code<br/>
make site<br/>

These commands will build program, run benchmark, create plots and html pages.<br/> 
To see generated results open /build/directory/site/index.html
