# Usage:
#   Build the image:
#     docker build -f Dockerfile -t cpp_proj:v1 .
#   Create and attach container:
#     mkdir -p /tmp/bazel_output
#     docker run -it --name cpp_proj -p 8080:8080 -v/tmp/bazel_output:/tmp/bazel_output -v ${PATH_TO_PROJECT}/cpp-project:/src/cpp-project cpp_proj:v1
#     docker start -i cpp_proj

FROM ubuntu:18.04
MAINTAINER Mark <mark@laioffer.com>

# install packages
RUN apt-get update
RUN apt-get --yes --force-yes install openjdk-11-jdk curl gnupg2 git libicu-dev liblzma-dev libbz2-dev
RUN echo "deb [arch=amd64] http://storage.googleapis.com/bazel-apt stable jdk1.8" >> /etc/apt/sources.list.d/bazel.list
RUN curl https://bazel.build/bazel-release.pub.gpg | apt-key add -
RUN apt-get update
RUN apt-get --yes --force-yes install bazel

VOLUME ["/src"]

# port
EXPOSE 8080

ENTRYPOINT ["/bin/bash"]
