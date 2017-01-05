-- import library
json = require('json')
require('jsonFileProcessor')
 
-- main function 
function main()
    local fileName = "test_pretty"
    local filePath = ""..fileName..".json"
    local testJson = FileRead(filePath)

    -- for table print
    if testJson then
    print("in")
    --print(#testJson)
        for i = 1, #testJson, 1 do
            table.foreach(testJson[i], print)
        end
    end
    
    FileWrite(filePath, testJson)
 
    local filePath = ""..fileName.."_Writen.json"
    FileWrite(filePath, testJson, true)
 
    return
end
 
-- call main
main()
