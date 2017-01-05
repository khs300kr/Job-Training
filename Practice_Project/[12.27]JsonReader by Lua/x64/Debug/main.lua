-- import library
json = require('json')
require('jsonFileProcessor')


local testJson = FileRead("test.json")

if testJson then
	print(testJson[1]["batters"]["batter"][1]["id"])
else
	print("No file")
end
	
