local Menu = { Job_Select, Place_Select }

function Menu.Job_Select()
print("You can select the Job")
print("1. Warrior")
print("2. Thief")
print("3. Magician")
print("Your selection is : ")
end

function Menu.Place_Select()
print("Places that you can go")
print("1. Field") 
print("2. Store")
print("3. Game Quit")
print("Your selection is : ")
end

function Menu.Field_Select()
print("The Monster that you can hunt")
print("1. Weak Monster") 
print("2. Strong Monster")
print("Your selection is : ")
end

function Menu.Fight_Select()
print("The action that you can do")
print("1. Hit")
print("2. Flee")
print("Your selection is : ")
end


function Menu.Store_Select()
end

return Menu