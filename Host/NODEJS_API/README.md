# Data-chan NodeJS FFI library

Since Data-chan is a multi-platform piece of software the NodeJS library needs to be as well.

To achieve that, Data-chan is automatically built to an amazon s3 bucket: 

* libDataChan.so for Linux
* libDataChan.dll for Windows
* libDataChan.dylib for MacOS

You need to set the following environmental variables for the script to run:

* AWS_ACCESS_KEY_ID
* AWS_SECRET_ACCESS_KEY
* AWS_DESTINATION_BUCKET (defaults to "data-chan-js-binaries")
* WERCKER_TOKEN
* WERCKER_DESTINATION_PIPELINE (defaults to 5855662ab7a7370100caf8fd) 
