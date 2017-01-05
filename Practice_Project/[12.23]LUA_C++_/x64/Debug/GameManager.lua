local GameManager = {Init,Progress,Render}

local CMenu = require "Menu" -- include "Menu.h"
local CPlayer = require "Player" -- include "player.h"
local CMonster = require "Monster" --include "monster.h"

local Hero = {}
local GameMonster = {}

-- Initialize
function GameManager.Init()
print("=====initialize=====")

end

-- Progress
function GameManager.Progress()
	--print("=====progress=====")
	CMenu.Job_Select()

	-- Player Init
	job = io.read()
	Hero = CPlayer.New(job)

		while true do

		-- Player Render
		Hero:Render()

		CMenu.Place_Select()
		place = io.read()
		
		-- Swich StateMent using Table
		local action = 
		{
		["1"] = function ()
		while true do
			CMenu.Field_Select()
			monster = io.read()
			GameMonster = CMonster.New(monster)
			GameMonster:Render()
			
		end

		end,
		["2"] = function () CMenu.Store_Select() end,
		}

		if place == "3" then break
		else action[place]() end
		
		
		
		end

end -- progress func end


-- Render
function GameManager.Render()
print("=====render=====")
end



return GameManager