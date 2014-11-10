print(i)

i = 5 + 2

for i, v in ipairs(j) do
	print(v)
end

nums = {3, 2, 1}

map = {hi = 5, world = 3}

print("add: "..add(1, 3))
print("dot: "..dot(5, 2, 3, 7))

print(m.first)
print(m.second)

j[2] = {2, 3}

say("hello from Lua!")

function printSomething(x, y)
	print("x: "..x)
	print("y: "..y)
end

function sayHello()
	return "hello!"
end

function return2Nums()
	return 5, 3
end