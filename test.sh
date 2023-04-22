#!/bin/bash
docker build -t matching_engine . && docker run -i --rm matching_engine bazel test --test_output=all //test:all_tests
