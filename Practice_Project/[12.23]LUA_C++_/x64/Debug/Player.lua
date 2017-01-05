-- Class ( problem : the number of instance make each function )
--[[
-- make a table
local Player = {}


function Player.new(hp, mp) -- argument became data ( member variables )
	local Object_Player = { hp = hp or nil, mp = mp or nil}

	-- method
	function Object_Player:getHP()
		return self.hp
	end
	
	function Object_Player:Render()
		print( "HP = " ..self.hp.. ", MP = " ..self.mp)		
	end
	
	return Object_Player
end
return Player
]]




-- Init method first
--[[
local function Render(tbl)
	print("HP = " ..tbl.hp..", MP = "..tbl.mp)
end

-- make a table
local Player = {}

function Player.New(hp,mp)
	local Object_Player = { hp = hp, mp = mp}
	
	function Object_Player:Render()
		Render(self)
	end

	return Object_Player
	
end
return Player
]]

-- JOB_ID 
local Warrior, Thief, Magician = '1', '2', '3'

-- make better using metatable
local mtIndex = {}

function mtIndex:Render()
	print("<"..self.strJob.."> HP = " ..self.hp..", MP = "..self.mp.. ", Att = " ..self.Att)
end


-- make a table
local Player = {}

function Player.New(job)
	local Object_Player = {}
	Object_Player.strJob = ""	
	Object_Player.hp = 0
	Object_Player.mp = 0
	Object_Player.Att = 0

	if (job == Warrior) then	
		Object_Player.hp = 100
		Object_Player.mp = 20
		Object_Player.Att = 10
		Object_Player.strJob = "Warrior"
	elseif (job == Thief) then
		Object_Player.hp = 80
		Object_Player.mp = 40
		Object_Player.Att = 15
		Object_Player.strJob = "Thief"
	elseif (job == Magician) then
		Object_Player.hp = 40
		Object_Player.mp = 80
		Object_Player.Att = 5
		Object_Player.strJob = "Magician"		
	end

	return setmetatable(Object_Player, {__index = mtIndex})
end

return Player