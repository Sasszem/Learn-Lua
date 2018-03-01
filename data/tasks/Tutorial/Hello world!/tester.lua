
__old_print=print

__success=false

function print(text)
    if text=="Hello World!" then
        __old_print(text)
        __success=true
    end
end
run()
if not __success then
    __old_print("Úgy látszik valamit elírtál.\nÜgyelj a kis- és nagybetűkre!")
end
return __success
