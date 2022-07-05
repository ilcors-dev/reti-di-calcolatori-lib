#!/bin/sh

src=$1
interface=$2
port=${3:-1099}

kill $(lsof -ti:$port)

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <server_rmi.java> <interface_rmi.java> <?port (optional, default: 1099)>"
    exit 1
fi


echo "--> Compiling.."
echo

# ${src%.*} is the filename without extension
compiledSrc=${src%.*}
compiledInterface=${interface%.*}

if [ -f "${compiledSrc}.class" ]; then
    rm "${compiledSrc}.class"
fi

if [ -f "${compiledInterface}.class" ]; then
    rm "${compiledInterface}.class"
fi

javac $src $interface

echo "--> Removing old Stubs & Skeletons.."

if [ -f "${compiledSrc}_Stub.class" ]; then
    rm "${compiledSrc}_Stub.class"
fi

if [ -f "${compiledSrc}_Skel.class" ]; then
    rm "${compiledSrc}_Skel.class"
fi

echo "--> Generating Stub & Skeleton classes.."
echo

rmic -vcompat $compiledSrc

echo "--> Starting rmiregistry.."
echo

rmiregistry &

echo "--> Started!"
echo "[Note] registry is running in the background, to kill the process run"
echo "--> kill \$(lsof -ti:$port)"

echo "Starting server.."

java $compiledSrc