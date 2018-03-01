
jo=0
for i=1,5 do
    A=math.random(100)-50
    B=math.random(100)-50
    C=math.random(100)-50
    D=(B^2-4*A*C)>=0
    if run()==D then jo=jo+1 end
end
print(jo)
if jo==5 then
    print("Gratuálok, megoldottad a feladatot!\nJöhet a következő!")
end


return jo==5
