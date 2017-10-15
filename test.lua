--Test.lua for testing code highlighting funcionality

if true then
   print("true")
elseif false
print("false")
else
   print("???")
   end

while true do
   print("Hello there")
end

for i,j in pairs({1:1,2:2}) do
   print("i: "..tostring(i)..", j:"..tostring(j))
end

x=0

repeat
   print(x)
   x=x+1
until x==5 end

print(1 and 2)
print(1 or 2)
print(not true)

function foo()
   return "bar"
   break
end

local n = nil

print("Random escape seq inside a string: \n\0\a\b\\\"")
--Comments are also highlighted until the end of the line

