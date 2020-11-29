# vsockj
VSOCK support for Java to communicate between a virtual machine and host.

## Introduction

More information: https://man7.org/linux/man-pages/man7/vsock.7.html

## Building locally

```shell script
docker build -t maven-gcc - < Dockerfile-build
docker run -v "$HOME/.m2":/var/maven/.m2 -ti --rm -u `id -u` -v "$PWD":/usr/src/mymaven \
-e MAVEN_CONFIG=/var/maven/.m2 -w /usr/src/mymaven \
maven-gcc mvn -Duser.home=/var/maven clean package install
```

## Running example
```shell script
docker run -v "$HOME/.m2":/var/maven/.m2 -ti --rm -u `id -u` -v "$PWD":/usr/src/mymaven \
-e MAVEN_CONFIG=/var/maven/.m2 -w /usr/src/mymaven \
maven-gcc mvn -f vsockj-example/pom.xml -Duser.home=/var/maven clean compile exec:exec
```