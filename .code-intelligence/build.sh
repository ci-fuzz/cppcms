#!/bin/bash -eu
# apt-get install -y python build-essential libpcre3-dev libicu-dev libgcrypt11-dev zlib1g-dev cmake
mkdir build
cd build
cmake -DCMAKE_TESTING_ENABLED=OFF -DDISABLE_SHARED=ON -DDISABLE_GCRYPT=ON -DDISABLE_OPENSSL=ON -DDISABLE_FCGI=ON -DDISABLE_SCGI=ON -DDISABLE_HTTP=ON -DDISABLE_CACHE=ON -DDISABLE_TCPCACHE=ON -DDISABLE_GZIP=ON ..
make -j$(nproc)