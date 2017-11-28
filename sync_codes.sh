#!/bin/bash

echo "fetch tl auth iii codes"
git fetch tl_auth
git branch -a

echo "merge tl_auth to local branch"
git merge tl_auth/master

echo "make sure your branch on master,otherwise resolve all conflicts"
git branch

