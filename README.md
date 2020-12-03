# vsockj
Linux VSOCK address family support for Java to communicate between a virtual machine and host.

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

Examples should be run in an environment that supports VSOCK. Example of how to setup an environment on AWS: [Getting started: Hello enclave](https://docs.aws.amazon.com/enclaves/latest/user/getting-started.html).

Client:
```shell script
mvn -f vsockj-example/pom.xml clean compile exec:exec
```

Server:
```shell script
mvn -f vsockj-example/pom.xml clean compile exec:exec -DclassName=ServerDemo
```