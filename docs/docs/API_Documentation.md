# API Documentation

API documentation is built directly from software source to provide the better
documentation level possible.

If you install doxygen you can build the documentation from the source code by yourself: this will provide you with the best possible documentation for your
version of the data-chan software:

```sh
chmod +x fetch_submodules.sh && ./fetch_submodules.sh
make documentation
```

## Online versions

The API documentation you can find online is related to a specific git revision, which is the latest commit in the master branch.

You can read the the git commit hash of the documentation on the left side of the documentation title.

### C API

The C API is the core of the system. Every other API (like C++ or node.js) are built wrapping the C API.

You can find the C API documentation [here](Doxygen/C_API/html).

