pushd ./libs/need_deps
./build.sh
popd
pushd ./libs
./build.sh
popd
pushd ./libs/need_lrt
./build.sh
popd
make
