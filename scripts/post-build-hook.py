import git
import tinys3
import glob
import os
import requests
import json
import sys

#Check if we are executing in a pull request, if so exit with a non error code
if os.environ.get('APPVEYOR_PULL_REQUEST_NUMBER', "false")  != "false":
    print("This is an AppVeyor pull request. Nothing to do.")
    sys.exit(0)
if os.environ.get('TRAVIS_PULL_REQUEST', "false")  != "false":
    print("This is a Travis pull request. Nothing to do.")
    sys.exit(0)


# Get the current repository commit hash
repo = git.Repo(search_parent_directories=True)
hash = repo.head.object.hexhash.encode("ascii")

def getBranch():
    "Get the branch using git or CI environmental variables"
    
    #try if head is not detached
    try:
        branch = repo.active_branch.name 
        return branch
    except TypeError:
        # apparently we are on a detached head, probably beacause we're building from a CI
        # in this case, pick the first non-empty branch name from the ENV variables the CI provides
        CIRepoENVVAr = []
        CIRepoENVVAr.append(os.environ.get('TRAVIS_BRANCH', ""))
        CIRepoENVVAr.append(os.environ.get('APPVEYOR_REPO_BRANCH', ""))
        CIRepoENVVAr.append(os.environ.get('WERCKER_GIT_BRANCH', ""))
        
        #Pick the first non-empty string
        def get_nonempty(list_of_strings):
            for s in list_of_strings:
                if s:
                    return s
        
        branch = get_nonempty(CIRepoENVVAr)
        return branch
        
        
    
print("hash of this commit is " + hash + " on branch " + getBranch())

# Find the file to upload
fileToUpload = glob.glob('Host/libDataChan.*')[0]
filename = fileToUpload.replace("Host", "").replace("/", "").replace("\\", "")

print("The file \"" + fileToUpload +  "\" will be uploaded to s3")

# If credentials are missing throw an error
if os.environ.get('AWS_ACCESS_KEY_ID', "") == "" or os.environ.get('AWS_SECRET_ACCESS_KEY', "")  == "":
    print("Missing AWS credentials. There is no point in attemping upload or trigger a build")
    sys.exit(1)

# Upload the datachan library to Amazon S3
s3conn = tinys3.Connection(os.environ['AWS_ACCESS_KEY_ID'], os.environ['AWS_SECRET_ACCESS_KEY'])
f = open(fileToUpload,'rb')
bucket = os.environ.get('AWS_DESTINATION_BUCKET', "data-chan-js-binaries") 
s3conn.upload(hash + "/" + filename, f, bucket )


# if we are not on wercker
if os.environ.get('WERCKER_MAIN_PIPELINE_STARTED', "") == "": 
    #if the wercker token is not empty
    if os.environ.get('WERCKER_TOKEN', "") != "":
        print("We're not on Wercker, Triggering a new Wercker build")
        
        #endpoint to run (or re-run) a pipeline
        url = 'https://app.wercker.com/api/v3/runs'
        #authorization headers
        headers = { 'Authorization': 'Bearer ' + os.environ['WERCKER_TOKEN']}
        
        #get the pipeline id from environmental variables, fallback on default if not found
        payload = {"pipelineId": os.environ.get('WERCKER_DESTINATION_PIPELINE', "5855662ab7a7370100caf8fd"), 
            "branch": getBranch(), 
            "commitHash": hash }
        r = requests.post(url, headers=headers, json=payload)
        
        if r.status_code == 200:
            print("Wercker build triggered")
        else:
            print("Build not triggered, response code " + str(r.status_code))
        print()

    else:
        print("We're not on Wercker but no WERCKER_TOKEN was found, so we can't trigger a new pipeline. Exiting.")
        sys.exit(0)
else:
    print("We're running on Wercker. Skip triggering a new Wercker build. Exiting.")
    sys.exit(0)

    
