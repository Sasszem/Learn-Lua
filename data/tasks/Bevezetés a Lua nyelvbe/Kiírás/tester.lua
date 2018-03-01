__oldprint=print
__printed={}
function print(mit)
    table.insert(__printed,mit)
    __oldprint(mit)
end

run()
r=true
if __printed[1]~="Szia!" then
r=false
end

if __printed[2]~="1000" then
r=false
end

if __printed[3]~=1000 then
r=false
end



if r then
    print("Gratuálok, megoldottad a feladatot!\nJöhet a következő!")
end

return r
