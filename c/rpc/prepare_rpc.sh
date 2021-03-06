#!/bin/sh
filename=$1

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <file.x>"
    exit 1
fi

noextension=${filename%.*}
echo "--> Removing old rpc files.."

if [ -f "RPC_Client" ]; then
    rm "RPC_Client"
fi

if [ -f "RPC_Server" ]; then
    rm "RPC_Server"
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

gcc "RPC_Client.c"  "${noextension}_clnt.c" "${noextension}_xdr.c" -o client 

echo "--> Bundle Server.."
echo

gcc "RPC_Server.c"  "${noextension}_svc.c" "${noextension}_xdr.c" -o server 

echo "--> Setup complete!"