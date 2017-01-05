-- import library
json = require('json')
require('jsonFileProcessor')

-- main function 
function main()
	local testJson = FileRead("test.json")
	
	
	print(testJson[1]["batters"]["batter"][1]["id"])
	
	-- for table print
	--if testJson then
	--	for i = 1, #testJson, 1 do
	--		table.foreach(testJson[i], print)
	--	end
	--end
	
	--FileWrite(filePath, testJson)
	--local filePath = fileName.."_pretty.json"
	--FileWrite(filePath, testJson, true)

	return
end

-- call main
main()