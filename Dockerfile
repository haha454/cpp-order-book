FROM gcc:11
WORKDIR /mengine
RUN apt -y update && apt -y install build-essential bazel-bootstrap
COPY . .
RUN bazel build //app:main
