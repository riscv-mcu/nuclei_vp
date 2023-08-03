# Nuclei Open Source Virtual Platform

Nuclei Open Source Virtual Platform is modified based on https://github.com/nvdla/vp repo.

It integrated nuclei qemu into systemc environment to provide a virtual platform based on systemc tlm.

It use technology such as greensoc's qbox and greenlib to support systemc modeling.

## Download Source Code

Git clone the repository and update the submodules.
After cloning the repository, run the following command to update the submodule:

```sh
# NOTE: for internal developer, please use internal git repo url
git clone https://github.com/riscv-mcu/nuclei_vp
cd nuclei_vp
# checkout wip branch or other branch you need
# this is still under development, so branch is named wip
git checkout wip
# make sure this step below executed successfully
git submodule update --init --recursive
# check repo status and git submodule status
git status
git submodule
```

## Build steps

> Not all scripts located in scripts works.

We are using docker ubuntu 20.04 to build and test this repo.

### Download and run docker

> Make sure docker is installed in your system, and you have executation right.

~~~shell
# in host now
bash scripts/run_docker.sh
# now in docker
~~~

### Build repo in docker

We have made a script to build this project using cmake, see `scripts/build.sh`,
if you want to know the details, please check script content.

~~~shell
# in docker now
cd /work
# build nuclei vp project in vp.build folder
# it will automatically build and install this project
# if you want to build it in a new folder, change vp.build to another name
bash scripts/build.sh vp.build
~~~

## Test in docker

~~~shell
# in docker now
## build freeloader and qemu disk images using nuclei linux sdk or use a prebuilt image
## see https://github.com/Nuclei-Software/nuclei-linux-sdk/tree/dev_nuclei_6.1_v2#booting-linux-on-nuclei-qemu
## simple steps in nuclei-linux-sdk repo is as below
## $ make SOC=evalsoc CORE=ux900fd freeloader gendisk
## then you can copy <nuclei-linux-sdk>/work/evalsoc/freeloader.elf and <nuclei-linux-sdk>/work/evalsoc/disk.img to <nuclei_vp>/images/
## or you can download a prebuilt version from https://drive.weixin.qq.com/s?k=ABcAKgdSAFcZkIVSxF
## extract it to <nuclei_vp>/images folder
# run this virtual platform located in vp.build
bash scripts/run.sh vp.build
~~~

Sample output can be found in [runlog.txt](scripts/runlog.txt)

## Licence

See [License](License).