import git
import tinys3
import glob
import os
import requests
import json
import sys


# Get the current repository commit hash
repo = git.Repo(search_parent_directories=True)
sha = repo.head.object.hexsha

try:
    #try if head is not detached
    branch =  repo.active_branch.name 
except TypeError:
    # apparently we are on a detached head, probably beacause we're building from a CI
    # in this case, pick the first non-empty branch name from the ENV variables the CI provides
    ciRepoVar = []
    ciRepoVar.append(os.environ.get('TRAVIS_BRANCH', ""))
    ciRepoVar.append(os.environ.get('APPVEYOR_REPO_BRANCH', ""))
    ciRepoVar.append(os.environ.get('WERCKER_GIT_BRANCH', ""))
    def get_nonempty(list_of_strings):
        for s in list_of_strings:
            if s:
                return s
    branch = get_nonempty(ciRepoVar)
        
    
print("SHA of this commit is " + sha + " on branch " + branch)

# Get the file to upload
fileToUpload = glob.glob('Host/libDataChan.*')[0]
fileToUploadName = fileToUpload.replace("Host/", "")

print("The file \"" + fileToUploadName +  "\" will be uploaded to s3")

if os.environ.get('AWS_ACCESS_KEY_ID', "") == "" or os.environ.get('AWS_SECRET_ACCESS_KEY', "")  == "":
    print("Missing AWS credentials. There is no point in attemping upload or trigger a build")
    sys.exit(1)

# Upload the datachan library to Amazon S3
s3conn = tinys3.Connection(os.environ['AWS_ACCESS_KEY_ID'], os.environ['AWS_SECRET_ACCESS_KEY'])
f = open(fileToUpload,'rb')
bucket = os.environ.get('AWS_DESTINATION_BUCKET', "data-chan-js-binaries") 
s3conn.upload(sha + "/" + fileToUploadName,f, bucket )


#if we are not on wercker
if os.environ.get('WERCKER_MAIN_PIPELINE_STARTED', "") == "": 
    #if the wercker token is not empty
    if os.environ.get('WERCKER_TOKEN', "") != "":
        print("We're not on Wercker, Triggering a new Wercker build")
        
        #endpoint to run (or re-run) a pipeline
        url = 'https://app.wercker.com/api/v3/runs'
        #authorization headers
        headers = { 'Authorization':'Bearer ' + os.environ['WERCKER_TOKEN'] }
        
        #get the pipeline id from environmental variables, fallback on default if not found
        a = {"pipelineId": os.environ.get('WERCKER_DESTINATION_PIPELINE', "5855662ab7a7370100caf8fd"), 
            "branch": branch, 
            "commitHash": sha }
        r = requests.get(url, headers=headers, verify=True, params=a)
    else:
        print("We're not on Wercker but no WERCKER_TOKEN was found, so we can't trigger a new pipeline")
else:
    print("We're running on Wercker. Skip triggering a new Wercker build")

    
