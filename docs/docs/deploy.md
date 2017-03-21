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

The libraries created by Travis and Appveyor are automatically uploaded to an [Amazon S3](https://aws.amazon.com/s3/) bucket by the post-build-hook.py script using the `--upload` flag. The file will be uploaded under a folder with the same name as the long [commit hash](https://git-scm.com/book/it/v2/Git-Basics-Viewing-the-Commit-History):

The destination bucket must be selected with the `--bucket my-s3-bucket` option.

For a complete cross-platform build three dynamic libraries needs to be uploaded:

* libDataChan.so for Linux
* libDataChan.dll for Windows
* libDataChan.dylib for MacOS


Since Wercker is extremely fast, it's build can be re-triggered automatically after upload if the `--trigger-wercker 123-this-is-my-pipelineId-456` option is used. This way Wercker it can be used to deploy on platforms other than s3, such as [GitHub releases](https://help.github.com/articles/about-releases/).

The script needs a few environmental variables that you're supposed to set up in your CI if you want to seriously fork data-chan:

* AWS_ACCESS_KEY_ID
* AWS_SECRET_ACCESS_KEY

The default bucket is publicly accessible [here](https://data-chan-js-binaries.s3.amazonaws.com/index.html)
