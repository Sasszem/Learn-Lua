__oldprint=print
__printed={}
function print(mit)
    table.insert(__printed,mit)
    __oldprint(mit)
end

run()

if not __printed[1]=="Szia!" then
    return false
end

if not __printed[2]=="1000" then
    return false
end

if not __printed[3]==1000 then
    return false
end


print("gratulálok")

return true
