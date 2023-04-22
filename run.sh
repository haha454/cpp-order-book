#!/bin/bash
docker build -t matching_engine . && docker run -i --rm matching_engine bazel run //app:main
