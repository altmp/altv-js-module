set SCOPE=SHARED
IF NOT "%1" == "" (
    set SCOPE=%1
)
node ../../tools/convert-bindings.js .. %SCOPE%
