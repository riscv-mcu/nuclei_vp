#!/bin/env bash

dockerrepo=${DOCKERREPO:-}

if [ "x$dockerrepo" == "x" ] ; then
    if ping -qc 2 rego > /dev/null 2>&1; then
        dockerrepo=rego.corp.nucleisys.com/software
    else
        dockerrepo=docker.io/nucleisoftware
    fi
fi

dockercmd=podman
if which docker > /dev/null 2>&1 ; then
    dockercmd=docker
fi

$dockercmd run -it -v $(pwd):/work $dockerrepo/ubuntu:2004-vp
