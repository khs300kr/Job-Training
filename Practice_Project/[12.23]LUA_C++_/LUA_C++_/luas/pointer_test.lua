-------------------------------------------------------------------------------
function objinfo(obj)

	local meta = getmetatable(obj)
	if meta ~= nil then
		metainfo(meta)
	else
		print("no object infomation !!")
	end
end

function metainfo(meta)

	if meta ~= nil then
		local name = meta["__name"]
		if name ~= nil then
			metainfo(meta["__parent"])
			print("<"..name..">")
			for key,value in pairs(meta) do 
				if not string.find(key, "__..") then 
					if type(value) == "function" then
						print("\t[f] "..name..":"..key.."()") 
					elseif type(value) == "userdata" then
						print("\t[v] "..name..":"..key)
					end
				end
			end
		end
	end
end
-------------------------------------------------------------------------------

--[[
-- C++ ���� ����� ������ ����Ѵ�.
print("recv_from_c++  -- "..type(pointer))	

function add_number(pointer)
--	local t = {pointer}

	
	return pointer;
end

did_something = add_number(pointer)
print("send_from_lua  -- "..type(did_something))	
]]

-- C++ ���� ����� g_test �� ��ü ������ ���캻��.
print("objinfo(pPlayer)")
objinfo(pPlayer)
print("type.."..type(pPlayer))
print("objinfo(Player)")
objinfo(Player)
print("type.."..type(Player))


print(pPlayer:get_HP())
print(Player:get_HP())

pPlayer:set_Damage(20)
Player:set_Damage(20)

print(pPlayer:get_HP())
print(Player:get_HP())

a = pPlayer:get_HP()
b = Player:get_HP()

--print(pPlayer)
--print(Player:get_HP())

--[[
-- ���� g_test �� ��� ���� _test �� ���� ����Ѵ�.
print(g_test._test)

-- const char* test::is_test() �Լ��� ����� ����Ѵ�.
print(g_test:is_test())

-- test::ret_int() �Լ��� ������� ����Ѵ�.
print(g_test:ret_int())

-- temp �� �� test ��ü�� ����� �ִ´�.
temp = test(4)

-- test �� ��� ���� _test ���� ����Ѵ�.
print(temp._test)

-- Lua �� �߰����� ���� A ����ü���� a��� ������ �ִ´�.
a = g_test:get()

-- ������ ��ü a�� Lua->C++�� �����Ѵ�.
temp:set(a)

-- test::set(A a) �Լ� ȣ��� _test �� ���� ��ȭ�� Ȯ���Ѵ�.
print(temp._test)

-- ��ӹ��� �θ��� �Լ� ȣ��
print(temp:is_base())

-- �ڱ� �ڽ��� �Լ� ȣ��
print(temp:is_test())
-- Lua �� �߰����� ���� A ����ü���� a��� ������ �ִ´�.
a = Ctest:get()

-- temp �� �� test ��ü�� ����� �ִ´�.
temp = test(4)

-- ������ ��ü a�� Lua->C++�� �����Ѵ�.
temp:set(a)
-- test::set(A a) �Լ� ȣ��� _test �� ���� ��ȭ�� Ȯ���Ѵ�.
print(temp._test)

-- ��ӹ��� �θ��� �Լ� ȣ��
print(temp:is_base())

-- �ڱ� �ڽ��� �Լ� ȣ��
print(temp:is_test())



-- Lua ���� ��ü�� userdata�� �νĵȴ�.
print("g_test	-> ", g_test)
print("temp	-> ", temp)
print("a	-> ", a)



-- constructor �� ���� ������ temp ��ü ������ ���캻��.
print("objinfo(temp)")
objinfo(temp)

-- ������� ���� A ����ü ���� a ��ü ������ ���캻��.
print("objinfo(a)")
objinfo(a)

]]




