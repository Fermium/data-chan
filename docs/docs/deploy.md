# Deploy

Data-chan is a cross-platform piece of software. 
That means it needs to be built on many platform. 

To achieve that, we use three CIs:

* [Travis](https://travis-ci.org/)
  * Ubuntu trusty
  * MacOS
* [Appveyor](https://www.appveyor.com/)
  * Windows
* [Wercker](http://www.wercker.com/)
  * Ubuntu Xenial

The libraries created by Travis and Appveyor are automatically uploaded to a [Amazon S3](https://aws.amazon.com/s3/) bucket by the post-build-hook.py script.

Three dinamic libraries are uploaded for each commit, under a directory with the same name as the long [commit hash](https://git-scm.com/book/it/v2/Git-Basics-Viewing-the-Commit-History):

* libDataChan.so for Linux
* libDataChan.dll for Windows
* libDataChan.dylib for MacOS

The script fetches a few environmental variables that you're supposed to set up in your CI if you want to seriously fork data-chan:

* AWS_ACCESS_KEY_ID
* AWS_SECRET_ACCESS_KEY
* AWS_DESTINATION_BUCKET (defaults to ["data-chan-js-binaries"](https://s3-eu-west-1.amazonaws.com/data-chan-js-binaries/), hosted by [Fermium LABS](https://fermiumlabs.com)) [WERCKER_TOKEN](http://devcenter.wercker.com/docs/api/getting-started/authentication)
* WERCKER_DESTINATION_PIPELINE (defaults to [5855662ab7a7370100caf8fd](https://app.wercker.com/fermiumlabs/data-chan/runs)) 

The default bucket is publicly accessible [here](https://s3-eu-west-1.amazonaws.com/data-chan-js-binaries/)
