import git
import tinys3
import glob
import os
import requests
import json

# Get the current repository commit hash
repo = git.Repo(search_parent_directories=True)
sha = repo.head.object.hexsha
branch =  repo.head.ref
print("SHA of this commit is " + sha + " on branch " + branch)

# Get the file to upload
fileToUpload = glob.glob('Host/libDataChan.*')[0]
fileToUploadName = fileToUpload.replace("Host/", "")

print("The file \"" + fileToUploadName +  "\" will be uploaded to s3")


# Upload the datachan library to Amazon S3
s3conn = tinys3.Connection(os.environ['AWS_ACCESS_KEY_ID'], os.environ['AWS_SECRET_ACCESS_KEY'])
f = open(fileToUpload,'rb')
bucket = os.environ.get('AWS_DESTINATION_BUCKET', "data-chan-js-binaries") 
s3conn.upload(sha + "/" + fileToUploadName,f, bucket )


#if we are not on wercker
if os.environ.get('WERCKER_MAIN_PIPELINE_STARTED', "") == "": 
    print("We're not on Wercker, Triggering a new Wercker build")
    
    #endpoint to run (or re-run) a pipeline
    url = 'https://app.wercker.com/api/v3/runs'
    #authorization headers
    headers = {'Authorization':'Bearer ' + os.environ['WERCKER_TOKEN'] }
    
    #get the pipeline id from environmental variables, fallback on default if not found
    a = {"pipelineId": os.environ.get('WERCKER_DESTINATION_PIPELINE', "5855662ab7a7370100caf8fd"), 
        "branch": branch, 
        "commitHash": sha }
    r = requests.get(url, headers=headers, verify=True, params=a)
    
else:
    print("We're running on Wercker. Skip triggering a new Wercker build")

    
    
