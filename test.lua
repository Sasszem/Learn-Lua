--Test.lua for testing code highlighting funcionality

if true then
   print("true")
elseif false then
print("false")
else
   print("???")
   end

while true do
   print("Hello there")
    break
end

for i,j in pairs({1,4,9,16}) do
   print("i: "..tostring(i)..", j:"..tostring(j))
end

x=0
1
repeat
   print(x)
   x=x+1
until x==5

print(1 and 2)
print(1 or 2)
print(not true)


function foo()
   return "bar"
   
end


local n = nil

print("Random escape seq inside a string: \n\0\a\b\\\"")
--Comments are also highlighted until the end of the line

