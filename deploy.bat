@echo off
call npm i alt-uploader
call npx alt-uploader dist node-module/%CI_COMMIT_REF_SLUG%/x64_win32 %GIT_TAG_NAME%