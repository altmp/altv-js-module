SCOPE=SHARED
if [ $# -eq 1 ]; then
    SCOPE=$1
fi
node ../../tools/convert-bindings.js .. $SCOPE
