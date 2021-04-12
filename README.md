# libscs
Library for SCS file using with Prism3D.

# Requirements

Need k3 prime with Prism3D usages

```
git clone https://github.com/google/cityhash.git
git checkout 00b9287e8c1255b5922ef90e304d5287361b2c2a
./configure --enable-sse4.2
make all check CXXFLAGS="-g -O3 -msse4.2"
sudo make install
```