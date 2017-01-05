 
-- file read
function FileRead(filePath)
    local data = nil
    local handle = io.open(filePath, "r")
 
    if handle then
        data = json:decode(handle:read("*a"))
        io.close(handle)
    end
    
    return data
end
 
-- file write
function FileWrite(filePath, data, pretty)    
    local handle = io.open(filePath, "w+")
    
    if handle then
        if pretty then
            handle:write(json:encode_pretty(data))
        else
            handle:write(json:encode(data))
        end
        io.close(handle)
    end
end
