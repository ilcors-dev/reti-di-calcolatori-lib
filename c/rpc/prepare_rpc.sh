#!/bin/sh
filename=$1
server=${2:"${filename%.*}_client.c"}
client=${3:"${filename%.*}_server.c"}

if [ "$#" -lt 1 ] && ["$#" -gt 3 ] ; then
    echo "Usage: $0 <file.x> <?server> <?client>"
    exit 1
fi

noextension=${filename%.*}
echo "--> Removing old rpc files.."

if [ -f "client" ]; then
    rm "client"
fi

if [ -f "server" ]; then
    rm "server"
fi

if [ -f "${noextension}.h" ]; then
    rm "${noextension}.h"
fi

if [ -f "${noextension}_xdr.c" ]; then
    rm "${noextension}_xdr.c"
fi

if [ -f "${noextension}_clnt.c" ]; then
    rm "${noextension}_clnt.c"
fi

if [ -f "${noextension}_svc.c" ]; then
    rm "${noextension}_svc.c"
fi

echo "--> Generating new rpc files.."
echo

rpcgen "${filename}"

echo "--> Bundle Client.."
echo
# Note: _xdr.c may not be present check the files created by rpcgen in case

gcc "${client}"  "${noextension}_clnt.c" "${noextension}_xdr.c" -o client 

echo "--> Bundle Server.."
echo

gcc "${server}"  "${noextension}_svc.c" "${noextension}_xdr.c" -o server 

echo "--> Setup complete!"