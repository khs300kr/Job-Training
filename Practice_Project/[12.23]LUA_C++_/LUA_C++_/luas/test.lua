-- 어떠한 객체의 metatable을 통해 등록된 클래스와 그 함수들을 살펴보는 함수
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
-- 사용 예
print("objinfo(g_test)")
objinfo(g_test)