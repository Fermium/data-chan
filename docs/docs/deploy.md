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

Since Wercker is extremely fast, it's build is re-triggered automatically by the script if a `WERCKER_TOKEN` environmental variable is defined. This way it can be used to deploy the code in other ways, such as an automatic deployment on [GitHub releases](https://help.github.com/articles/about-releases/) on every [tag](https://git-scm.com/book/en/v2/Git-Basics-Tagging).

Three dinamic libraries are uploaded for each commit, under a directory with the same name as the long [commit hash](https://git-scm.com/book/it/v2/Git-Basics-Viewing-the-Commit-History):

* libDataChan.so for Linux
* libDataChan.dll for Windows
* libDataChan.dylib for MacOS

The script fetches a few environmental variables that you're supposed to set up in your CI if you want to seriously fork data-chan:

* AWS_ACCESS_KEY_ID
* AWS_SECRET_ACCESS_KEY
* AWS_DESTINATION_BUCKET (defaults to ["data-chan-js-binaries"](https://data-chan-js-binaries.s3.amazonaws.com/index.html), hosted by [Fermium LABS](https://fermiumlabs.com)) [WERCKER_TOKEN](http://devcenter.wercker.com/docs/api/getting-started/authentication)
* WERCKER_DESTINATION_PIPELINE (defaults to [5855662ab7a7370100caf8fd](https://app.wercker.com/fermiumlabs/data-chan/runs)) 

The default bucket is publicly accessible [here](https://data-chan-js-binaries.s3.amazonaws.com/index.html)
