parok={["42"]="42",[5]=5,["Hello"]="Hello"}
jo=0

for k,v in pairs(parok) do
    BE=k
    if v==run() then
        jo=jo+1
    end     
end
print(jo)
if jo==3 then
    print("Gratuálok, megoldottad a feladatot!\nJöhet a következő!")
end
return jo==3
