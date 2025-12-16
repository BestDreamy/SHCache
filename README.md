# Intrduction
L3 Cache with CHI_b protocol

# Pre-Install
```
make
python 3.12
c++
pyjson
```

# Run
```
# To generate csrc/include/autoconfig.h and vsrc/include/autoconfig.v
make config

make
```

docker run -it -p 3333:22  -v /Users/aric/work:/work --name SHCache antrea/antrea-ubuntu-arm:latest bash
