var Git = require( 'nodegit' );
var s3 = require('s3');
var path = require('path');
var glob = require("glob");


var client = s3.createClient({
  maxAsyncS3: 20,     // this is the default 
  s3RetryCount: 3,    // this is the default 
  s3RetryDelay: 1000, // this is the default 
  multipartUploadThreshold: 20971520, // this is the default (20 MB) 
  multipartUploadSize: 15728640, // this is the default (15 MB) 
  s3Options: {
    // any other options are passed to new AWS.S3() 
    // See: http://docs.aws.amazon.com/AWSJavaScriptSDK/latest/AWS/Config.html#constructor-property 
  },
});

Git.Repository.open( "../.." ).then( function( repository ) {
  return repository.getHeadCommit( );
} ).then( function ( commit ) {
  return commit.sha();
} ).then( function ( hash ) {

  
  // how to get this file out of the class? i dunno JS
  glob("./libDataChan.*", function (er, files) {
    fileToUpload = files[0];
    console.log("file to be uploaded:", fileToUpload);
  });

 
  var params = {
  localFile: "libDataChan.so",
  s3Params: {
    Bucket: "data-chan-js-binaries",
    Key: hash + "/" + "libDataChan.so",
  },
};


var uploader = client.uploadFile(params);
uploader.on('error', function(err) {
  console.error("unable to upload:", err.stack);
});
uploader.on('progress', function() {
  console.log("progress", uploader.progressMd5Amount,
            uploader.progressAmount, uploader.progressTotal);
});
uploader.on('end', function() {
  console.log("done uploading");
});
  
  
} );
