rm -r ../build_profile_generate
rm -r ../build_result
rm -r ../profile

#========================================================

mkdir ../build_profile_generate
cd ../build_profile_generate

#========================================================

# change compiler, if needed
#export CC=""
#export CXX=""

export CXXFLAGS="-fprofile-generate -fprofile-dir=../profile"
export LDFLAGS="-fprofile-generate -fprofile-dir=../profile"

# -fprofile-update=atomic should be used in multithreaded program, but older compilers don't support it.
#export CXXFLAGS="-fprofile-generate -fprofile-dir=../profile -fprofile-update=atomic"
#export LDFLAGS="-fprofile-generate -fprofile-dir=../profile -fprofile-update=atomic"

cmake -DCMAKE_BUILD_TYPE=Release ..

make -j 4

#========================================================

./benchmark

#========================================================

mkdir ../build_result
cd ../build_result

# -fprofile-correction should be used if compiler does not support -fprofile-update=atomic
# because profile data may be corrupted in multithreaded program
export CXXFLAGS="-fprofile-use -fprofile-dir=../profile -fprofile-correction"
export LDFLAGS="-fprofile-use -fprofile-dir=../profile -fprofile-correction"

cmake -DCMAKE_BUILD_TYPE=Release ..

make -j 4

