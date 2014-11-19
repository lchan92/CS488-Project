--
-- CS488 -- Introduction to Computer Graphics
-- 
-- a3mark.lua
--
-- A very simple scene creating a trivial puppet.  The TAs will be
-- using this scene as part of marking your assignment.  You'll
-- probably want to make sure you get reasonable results with it!

rootnode = gr.node('root')
rootnode:scale(0.2,0.2,0.2)

red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
green = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10)
white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10)

torso = gr.sphere('torso')
rootnode:add_child(torso)
torso:set_material(white)
torso:scale(2.0, 5.0, 1.0)

shoulders = gr.sphere('shoulders')
rootnode:add_child(shoulders)
shoulders:set_material(green)
shoulders:scale(5.0, 1.0, 1.0)
shoulders:translate(0, 5.0, 0)

hips = gr.sphere('hips')
rootnode:add_child(hips)
hips:set_material(green)
hips:scale(3.0, 1.0, 1.0)
hips:translate(0, -5.0, 0)

-- NECK AND HEAD
neckjoint = gr.joint('neckjoint', {0, 0, 45}, {-15, 0, 15})
rootnode:add_child(neckjoint)
neckjoint:translate(0, 5.0, 0)

neck = gr.sphere('neck')
neckjoint:add_child(neck)
neck:set_material(white)
neck:translate(0,1.0,0)
neck:scale(0.5, 2.0, 0.5)

headjoint = gr.joint('headjoint', {-20, 0, 45}, {-45, 0, 45})
neckjoint:add_child(headjoint)
headjoint:translate(0, 2.0, 0)

head = gr.sphere('head')
headjoint:add_child(head)
head:set_material(red)
head:translate(0, 2.0, 0)
head:scale(2.0,2.0,2.0)

nose = gr.sphere('nose')
headjoint:add_child(nose)
nose:set_material(red)
nose:translate(0, 2.0, 1.0)
nose:scale(0.5,0.5,2.0)

-- LEFT ARM
leftshoulderjoint = gr.joint('leftshoulderjoint', {-150,0,45}, {-15,0,15})
rootnode:add_child(leftshoulderjoint)
leftshoulderjoint:translate(-5.0, 5.0, 0)

leftupperarm = gr.sphere('leftupperarm')
leftshoulderjoint:add_child(leftupperarm)
leftupperarm:set_material(blue)
leftupperarm:translate(0,-2.5,0)
leftupperarm:scale(0.7, 2.5, 0.7)

leftelbow = gr.joint('leftelbow', {-150,0,0}, {0,0,0})
leftshoulderjoint:add_child(leftelbow)
leftelbow:translate(0,-5.0,0)

leftforearm = gr.sphere('leftforearm')
leftelbow:add_child(leftforearm)
leftforearm:set_material(blue)
leftforearm:translate(0,-2.0,0)
leftforearm:scale(0.5, 2.5, 0.5)

leftwrist = gr.joint('leftwrist', {-30,0,30}, {-90,0,90})
leftelbow:add_child(leftwrist)
leftwrist:translate(0,-5.0,0)

lefthand = gr.sphere('lefthand')
leftwrist:add_child(lefthand)
lefthand:set_material(blue)
lefthand:translate(0,-0.5,0)
lefthand:scale(0.3,1,0.5)

-- RIGHT ARM
rightshoulderjoint = gr.joint('rightshoulderjoint', {-150,0,45}, {-15,0,15})
rootnode:add_child(rightshoulderjoint)
rightshoulderjoint:translate(5.0, 5.0, 0)

rightupperarm = gr.sphere('rightupperarm')
rightshoulderjoint:add_child(rightupperarm)
rightupperarm:set_material(blue)
rightupperarm:translate(0,-2.5,0)
rightupperarm:scale(0.7, 2.5, 0.7)

rightelbow = gr.joint('rightelbow', {-150,0,0}, {0,0,0})
rightshoulderjoint:add_child(rightelbow)
rightelbow:translate(0,-5.0,0)

rightforearm = gr.sphere('rightforearm')
rightelbow:add_child(rightforearm)
rightforearm:set_material(blue)
rightforearm:translate(0,-2.0,0)
rightforearm:scale(0.5, 2.5, 0.5)

rightwrist = gr.joint('rightwrist', {-30,0,30}, {-90,0,90})
rightelbow:add_child(rightwrist)
rightwrist:translate(0,-5.0,0)

righthand = gr.sphere('righthand')
rightwrist:add_child(righthand)
righthand:set_material(blue)
righthand:translate(0,-0.5,0)
righthand:scale(0.3,1,0.5)

-- LEFT LEG

lefthipjoint = gr.joint('lefthipjoint', {-120,0,60}, {-30,0,30})
rootnode:add_child(lefthipjoint)
lefthipjoint:translate(-2.0, -5.0, 0)

leftthigh = gr.sphere('leftthigh')
lefthipjoint:add_child(leftthigh)
leftthigh:set_material(blue)
leftthigh:translate(0,-2.5,0)
leftthigh:scale(1.0, 3.0, 1.0)

leftknee = gr.joint('leftknee', {0,0,150}, {0,0,0})
lefthipjoint:add_child(leftknee)
leftknee:translate(0,-6.0,0)

leftcalf = gr.sphere('leftcalf')
leftknee:add_child(leftcalf)
leftcalf:set_material(blue)
leftcalf:translate(0,-2.0,0)
leftcalf:scale(0.7, 3.0, 0.7)

leftankle = gr.joint('leftankle', {-90,0,90}, {20,50,120})
leftknee:add_child(leftankle)
leftankle:translate(0,-5.0,0)

leftfoot = gr.sphere('leftfoot')
leftankle:add_child(leftfoot)
leftfoot:set_material(red)
leftfoot:translate(-1.0,0,0)
leftfoot:scale(1.5,0.5,0.5)

-- RIGHT LEG

righthipjoint = gr.joint('righthipjoint', {-120,0,60}, {-30,0,30})
rootnode:add_child(righthipjoint)
righthipjoint:translate(2.0, -5.0, 0)

rightthigh = gr.sphere('rightthigh')
righthipjoint:add_child(rightthigh)
rightthigh:set_material(blue)
rightthigh:translate(0,-2.5,0)
rightthigh:scale(1.0, 3.0, 1.0)

rightknee = gr.joint('rightknee', {0,0,150}, {0,0,0})
righthipjoint:add_child(rightknee)
rightknee:translate(0,-6.0,0)

rightcalf = gr.sphere('rightcalf')
rightknee:add_child(rightcalf)
rightcalf:set_material(blue)
rightcalf:translate(0,-2.0,0)
rightcalf:scale(0.7, 3.0, 0.7)

rightankle = gr.joint('rightankle', {-90,0,90}, {-120,-50,-20})
rightknee:add_child(rightankle)
rightankle:translate(0,-5.0,0)

rightfoot = gr.sphere('rightfoot')
rightankle:add_child(rightfoot)
rightfoot:set_material(red)
rightfoot:translate(1.0,0,0)
rightfoot:scale(1.5,0.5,0.5)

return rootnode
