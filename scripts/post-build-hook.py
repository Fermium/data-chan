import git
import tinys3
import glob
import os
import requests
import sys
import ntpath  # cross platform path tools


# Check if we are executing in a pull request, if so exit with a non error code
if os.environ.get('APPVEYOR_PULL_REQUEST_NUMBER', "false") != "false":
    print("this is an AppVeyor pull request. Nothing to do.")
    sys.exit(0)
if os.environ.get('TRAVIS_PULL_REQUEST', "false") != "false":
    print("this is a Travis pull request. Nothing to do.")
    sys.exit(0)


# Get the current repository commit hash
repo = git.Repo(search_parent_directories=True)
hash = repo.head.object.hexsha
hash = hash.encode("ascii")


def getBranch():
    "Get the branch using git or CI environmental variables"

    # try if head is not detached
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

        # Pick the first non-empty string
        def get_nonempty(list_of_strings):
            for s in list_of_strings:
                if s:
                    return s

        branch = get_nonempty(CIRepoENVVAr)
        return branch


print("hash of this commit is " + hash + " on branch " + getBranch())

# Find the file to upload
fileToUpload = glob.glob('**/libDataChan.*')[0]

print("file \"" + fileToUpload + "\" will be uploaded (overwriting of needed) to s3")

# If credentials are missing throw an error
if os.environ.get('AWS_ACCESS_KEY_ID', "") == "" or os.environ.get('AWS_SECRET_ACCESS_KEY', "") == "":
    print("missing AWS credentials. There is no point in attemping upload or trigger a build")
    sys.exit(1)

# Upload the datachan library to Amazon S3
s3conn = tinys3.Connection(os.environ['AWS_ACCESS_KEY_ID'], os.environ['AWS_SECRET_ACCESS_KEY'])
f = open(fileToUpload, 'rb')
bucket = os.environ.get('AWS_DESTINATION_BUCKET', "data-chan-js-binaries")
s3conn.upload(hash + "/" + ntpath.basename(fileToUpload), f, bucket)

# Get a list of all filenames in the current folder
a = [d["key"] for d in list(s3conn.list(hash, bucket))]
a = map(ntpath.basename, a)


# Check for missing filenames
expectedFilenames = ["libDataChan.dll", "libDataChan.dylib", "libDataChan.so"]
missingFilenames = set(expectedFilenames) - set(a)
if len(missingFilenames) > 0:
    print("still missing the following libraries in S3: " + ", ".join(missingFilenames))
    sys.exit(0)
else:
    print("all needed lbraries are present in S3: " + ", ".join(expectedFilenames))


# if we are not on wercker
if os.environ.get('WERCKER_MAIN_PIPELINE_STARTED', "") == "":
    # if the Wercker token is not empty
    if os.environ.get('WERCKER_TOKEN', "") != "":
        print("we're not on Wercker, triggering a new Wercker build")

        # endpoint to run (or re-run) a pipeline
        url = 'https://app.wercker.com/api/v3/runs'
        # authorization headers
        headers = {'Authorization': 'Bearer ' + os.environ['WERCKER_TOKEN']}

        # get the pipeline id from environmental variables, fallback on default if not found
        payload = {"pipelineId": os.environ.get('WERCKER_DESTINATION_PIPELINE', "5855662ab7a7370100caf8fd"),
                   "branch": getBranch(),
                   "commitHash": hash}
        r = requests.post(url, headers=headers, json=payload)

        if r.status_code == 200:
            print("Wercker build triggered")
        else:
            print("build not triggered, response code " + str(r.status_code))
            sys.exit(1)

    else:
        print("we're not on Wercker but no WERCKER_TOKEN was found, so we can't trigger a new pipeline. exiting.")
        sys.exit(0)
else:
    print("we're running on Wercker. skip triggering a new Wercker build. exiting.")
    sys.exit(0)
