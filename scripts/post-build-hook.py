import git
import glob
import os
import requests
import sys
import ntpath  # cross platform path tools
import argparse
import boto


parser = argparse.ArgumentParser(description="Manage Data-chan binaries")
parser.add_argument('--upload', dest='upload', help='Upload Data-chan binaries', action='store_true')
parser.set_defaults(upload=False) 
parser.add_argument('--bucket', dest='bucket', help='S3 bucket')
parser.add_argument('--trigger-wercker', dest='wercker', help='Trigger a Wercker build to the provided pipeline')
parser.add_argument('--download', dest='download', help='Download all datachan libraries in the provided directory')
parser.set_defaults(download=False) 

args = parser.parse_args()


def isPullRequest():
    """Verify using environmental variables if we are in a PR."""
    if os.environ.get('APPVEYOR_PULL_REQUEST_NUMBER', "false") != "false":
        return True
    if os.environ.get('TRAVIS_PULL_REQUEST', "false") != "false":
        return True
    return False


if isPullRequest() is True:
    print("We're in a PR, exiting")
    sys.exit(0)

# Get the current repository commit hash
repo = git.Repo(search_parent_directories=True)
hash = repo.head.object.hexsha
hash = hash.encode("ascii")
pr = isPullRequest()


expectedFilenames = ["libDataChan.dll", "libDataChan.dylib", "libDataChan.so"]


def getBranch():
    """Get the branch using git or CI environmental variables."""
    try:
        # Works only if head is not detached
        branch = repo.active_branch.name
        return branch
    except TypeError:
        # apparently we are on a detached head, probably beacause we're building from a CI
        CIRepoENVVAr = []
        CIRepoENVVAr.append(os.environ.get('TRAVIS_BRANCH', ""))
        CIRepoENVVAr.append(os.environ.get('APPVEYOR_REPO_BRANCH', ""))
        CIRepoENVVAr.append(os.environ.get('WERCKER_GIT_BRANCH', ""))

        # Pick the first non-empty branch name
        def get_nonempty(list_of_strings):
            for s in list_of_strings:
                if s:
                    return s
        branch = get_nonempty(CIRepoENVVAr)
        return branch


# If AWS credentials are missing throw an error
if os.environ.get('AWS_ACCESS_KEY_ID', "") == "" or os.environ.get('AWS_SECRET_ACCESS_KEY', "") == "":
    print("missing AWS credentials. There is no point in attemping upload or trigger a build")
    sys.exit(1)

# Open connection to s3
s3conn = boto.connect_s3()
bucket = s3conn.get_bucket(args.bucket)

# Upload to S3
if args.upload is True:
    fileToUpload = glob.glob('**/libDataChan.*')[0]
    print("Uploading \"" + fileToUpload + "\"...")
    s3key = boto.s3.key.Key(bucket)
    s3key.key = hash + "/" + ntpath.basename(fileToUpload)
    s3key.set_contents_from_filename(fileToUpload)


# Get a list of all filenames in the current hash S3 folder
a = []
for key in bucket.list(prefix=hash):
    a.append(key.name.encode("ascii"))
a = map(ntpath.basename, a)



# Check for missing filenames
missingFilenames = set(expectedFilenames) - set(a)
if len(missingFilenames) > 0:
    print("CHECK: still missing the following libraries in S3: " + ", ".join(missingFilenames))
    sys.exit(0)
else:
    print("all needed lbraries are present in S3: " + ", ".join(expectedFilenames))
    if args.download is not False:
        # If directory does not exist make it
        if not os.path.exists(args.download):
            os.makedirs(args.download)
        for f in expectedFilenames:
            s3key = boto.s3.key.Key(bucket)
            s3key.key = hash + "/" + f
            s3key.get_contents_to_filename(os.path.join(args.download, f))


def triggerWerckerPipeline():
    """Trigger a Wercker pipeline. Returns false if failed."""
    if args.wercker is True:
        if os.environ.get('WERCKER_MAIN_PIPELINE_STARTED', "") == "":
            print("we're running on Wercker. skip triggering a new Wercker build. exiting.")
            return False;
        if os.environ.get('WERCKER_TOKEN', "") == "":
            print("no WERCKER_TOKEN was found, so we can't trigger a new pipeline. exiting.")
            return False;
        # endpoint to run (or re-run) a pipeline
        url = 'https://app.wercker.com/api/v3/runs'
        # authorization headers
        headers = {'Authorization': 'Bearer ' + os.environ['WERCKER_TOKEN']}
        # get the pipeline id from environmental variables, fallback on default if not found
        payload = {"pipelineId": args.wercker,
                   "branch": getBranch(),
                   "commitHash": hash}
        r = requests.post(url, headers=headers, json=payload)

        if r.status_code == 200:
            print("Wercker build triggered")
        else:
            print("build not triggered, response code " + str(r.status_code))
            return False
        return True


if triggerWerckerPipeline() is False:
    sys.exit(1)
