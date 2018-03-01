
data={}

for i=1,5 do
    table.insert(data,{2*i,i/2,i^2,i%3})
end

jo=0
for k,v in pairs(data) do
    SZAM=k    
    dupla,fel,negyzet,marad=run()
    if dupla==v[1] and fel==v[2] and negyzet==v[3] and marad==v[4] then jo=jo+1 end
end

if jo==5 then
    print("Gratuálok, megoldottad a feladatot!\nJöhet a következő!")
end


return jo==5
