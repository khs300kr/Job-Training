-- Monster_ID 
local Weak, Strong = '1', '2'

-- make better using metatable
local mtIndex = {}

function mtIndex:Render()
	print("<"..self.type.."> HP = " ..self.hp..", MP = "..self.mp.. ", Att = " ..self.Att)
end


-- make a table
local Monster = {}

function Monster.New(monster)
	local Object_Monster = {}
	Object_Monster.t = ""
	Object_Monster.hp = 0
	Object_Monster.mp = 0
	Object_Monster.Att = 0

	if (monster == Weak) then	
		Object_Monster.hp = 50
		Object_Monster.mp = 10
		Object_Monster.Att = 5
		Object_Monster.type = "Strong"
	elseif (monster == Strong) then
		Object_Player.hp = 100
		Object_Player.mp = 30
		Object_Player.Att = 20
		Object_Player.strJob = "Weak"
	end

	return setmetatable(Object_Monster, {__index = mtIndex})
end

return Montser