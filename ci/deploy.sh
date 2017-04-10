#!/bin/bash
if [ "$1" == "win64" ]; then
    arch=win64
elif [ "$1" == "html5" ]; then
    arch=html5
elif [ "$1" == "linux64" ]; then
    arch=linux64
else
    echo "Not implemented"
    exit
fi

pacman --noconfirm -Sy zip

rm -rf deploy && mkdir deploy && cd deploy

# copy binaries
if [ "$arch" == "win64" ]; then
    cp -R ../res .
    cp ../ci/dll/* .
    cp ../bin/*.exe .
elif [ "$arch" == "html5" ]; then
    cp -R ../res .
    cp ../bin/*.js .
    cp ../bin/*.html .
    cp ../bin/*.mem .
    cp ../bin/*.data .
elif [ "$arch" == "linux64" ]; then
    cp -R ../res .
    find ../bin -maxdepth 1 -perm /a+x -exec cp {} . \;
fi

# init ssh
eval $(ssh-agent -s)
ssh-add <(echo "$SSH_PRIVATE_KEY")
mkdir -p ~/.ssh
[[ -f /.dockerenv ]] && echo "$SSH_SERVER_HOSTKEYS" > ~/.ssh/known_hosts

# upload build
if [ "$arch" == "html5" ]; then
    scp -r . deploy@builds.et.tc:/var/www/et.tc/Builds/raycaster/$CI_COMMIT_REF_NAME-$arch
else
    zip -9 -r $arch.zip .
    scp $arch.zip deploy@builds.et.tc:/var/www/et.tc/Builds/raycaster/$CI_COMMIT_REF_NAME-$arch.zip
fi
