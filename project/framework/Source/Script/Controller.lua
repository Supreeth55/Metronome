config = {
    mpositiony = 0.1,
    mpositionx = 0.1,
    mpositionz = 0.1,
    shape = "Bullet",
    noofsteps = 4,
    speed = 1.0,
    StopNum = 30.0,
    StartNum = 8.0,
    WalkNum = 1.0,
    StopDen = 24.0,
    StartDen = 24.0,
    WalkDen = 1.0,
    walk_animation = "run",
    start_animation = "idle",
    stop_animation = "idle"
 
}
left_surface = {
    mVelo_x = -2.0,
    mVelo_y = 0.0,
    mVelo_z = 0.0
}
 
right_surface = {
    mVelo_x = 2.0,
    mVelo_y = 0.0,
    mVelo_z = 0.0
}
 
up_surface = {
    mVelo_x = 0.0,
    mVelo_y = 2.0,
    mVelo_z = 0.0
}
 
down_surface = {
    mVelo_x = 0.0,
    mVelo_y = -2.0,
    mVelo_z = 0.0
}
 
left_falling = {
    mVelo_x = -2.0,
    mVelo_y = 0.0,
    mVelo_z = 0.0
}
 
right_falling = {
    mVelo_x = 2.0,
    mVelo_y = 0.0,
    mVelo_z = 0.0
}
 
up_falling = {
    mVelo_x = 0.0,
    mVelo_y = 2.0,
    mVelo_z = 0.0
}
 
down_falling = {
    mVelo_x = 0.0,
    mVelo_y = -2.0,
    mVelo_z = 0.0
}
 
up_space = {
    mVelo_x = 0.0,
    mVelo_y = 10.0,
    mVelo_z = 0.0
}
 
down_space = {
    mVelo_x = 0.0,
    mVelo_y = -10.0,
    mVelo_z = 0.0
}
 
left_space = {
    mVelo_x = -10.0,
    mVelo_y = 0.0,
    mVelo_z = 0.0
}
 
right_space = {
    mVelo_x = 10.0,
    mVelo_y = 0.0,
    mVelo_z = 0.0
}
local class = require 'middleclass'
Class = class('Class')
 
function Class:StopTime(number1, number2)
  return number1 / number2
end
 
function Class:StartTime(number1, number2)
  return number1 / number2
end
 
function Class:WalkTime(number1, number2)
  return number1 / number2
end