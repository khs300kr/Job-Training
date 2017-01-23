--local redis = require 'redis'
--local client = redis.connect('127.0.0.1', 6379)
JSON = (loadfile "JSON.lua")() -- one-time load of the routines

-- 하나라도 키값이 있을경우 - 테이블, 없으면 Array
-- 패시브스킬 테이블
CPassiveSkill = {aId =0 , bJob = "한글도되냐", cCoolTime=121212, dEffect = {1,2,3}}
CPassiveSkill.sEffect = {Att=0, Def=0}

-- Table_Print Func
function print_r ( t )
    local print_r_cache={}
    local function sub_print_r(t,indent)
        if (print_r_cache[tostring(t)]) then
            print(indent.."*"..tostring(t))
        else
            print_r_cache[tostring(t)]=true
            if (type(t)=="table") then
                for pos,val in pairs(t) do
                    if (type(val)=="table") then
                        print(indent.."["..pos.."] = "..tostring(t).." {")
                        sub_print_r(val,indent..string.rep(" ",string.len(pos)+8))
                        print(indent..string.rep(" ",string.len(pos)+6).."}")
                    elseif (type(val)=="string") then
                        print(indent.."["..pos..'] = "'..val..'"')
                    else
                        print(indent.."["..pos.."] = "..tostring(val))
                    end
                end
            else
                print(indent..tostring(t))
            end
        end
    end
    if (type(t)=="table") then
        print(tostring(t).." {")
        sub_print_r(t,"  ")
        print("}")
    else
        sub_print_r(t,"  ")
    end
    print()
end


--recvTable = JSON:encode_pretty(CPassiveSkill)
--client:set('usr:passive', recvTable)
--print(recvTable)

--redisdata = client:get('usr:passive')
--print(redisdata)

--print_r(JSON:decode(redisdata))


--

local recvJson = JSON:decode(recvTable)
print_r(recvJson)



-- Table을 JSON으로
function EncodeJson(s)
	return JSON:encode_pretty(s);
end;

-- JSON을 Table으로
function EncodeTable(jsonString)
	return JSON:decode(jsonString);
end;

-- JSON을 TABLE로 바꾼후 TABLE 출력하는 함수.
function EncodePrint(jsonString)
	print_r(JSON:decode(jsonString))
end;


