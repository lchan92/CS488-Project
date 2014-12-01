-- TYPES
-- 0: wood
-- 1: dirty glass

rootnode = gr.node('maproot')

box = gr.block('box', 0, 0, 0, 0)
rootnode:add_child(box)
box:translate(-5, -15, 0)
box:scale(10,10,10)

box2 = gr.block('box2', 1, 0, 0, 0)
rootnode:add_child(box2)
box2:translate(-5, -11, -20)
box2:scale(10,10,10)

box3 = gr.block('box3', 0, 0, 0, 0)
rootnode:add_child(box3)
box3:translate(-15, -11, -30)
box3:scale(10,30,3)

box6 = gr.block('box6', 0, 0, 15, 0)
rootnode:add_child(box6)
box6:translate(-15,-5,-27)
box6:scale(10, 1.5, 1.5)

box4 = gr.block('box4', 0, 0, 0, 0)
rootnode:add_child(box4)
box4:translate(-20, 18, -40)
box4:scale(3,5,25)

box5 = gr.block('box5', 2, 0, 0, -10)
rootnode:add_child(box5)
box5:translate(-23.5, 18, -52)
box5:scale(10,1.5,10)

box7 = gr.block('box7', 2, 10, 0, 0)
rootnode:add_child(box7)
box7:translate(-20, 18, -75)
box7:scale(10,1.5,10)

box8 = gr.block('box8', 0, 0, 0, 0)
rootnode:add_child(box8)
box8:translate(5, 18, -75)
box8:scale(20,1.5,10)

box9 = gr.block('box9', 1, 0, 0, 0)
rootnode:add_child(box9)
box9:translate(15, 29, -80)
box9:scale(20,10,10)

box10 = gr.block('box10', 2, 0, 10, 10)
rootnode:add_child(box10)
box10:translate(25, 18, -60)
box10:scale(10,1.5,10)

box11 = gr.block('box11', 2, 0, -15, 15)
rootnode:add_child(box11)
box11:translate(35, 38, -80)
box11:scale(10,1.5,10)

box12 = gr.block('box12', 0, 0, 0, 0)
rootnode:add_child(box12)
box12:translate(20, 50, -100)
box12:scale(10,1,5)

box13 = gr.block('box13', 0, 0, 0, 0)
rootnode:add_child(box13)
box13:translate(20, 52, -105)
box13:scale(10,1,5)

box14 = gr.block('box14', 0, 0, 0, 0)
rootnode:add_child(box14)
box14:translate(20, 54, -110)
box14:scale(10,1,5)

box15 = gr.block('box15', 2, 0, 0, 0)
rootnode:add_child(box15)
box15:translate(20, 56, -130)
box15:scale(10,1,20)

box16 = gr.block('box16', 1, 0, 0, 0)
rootnode:add_child(box16)
box16:translate(20, 56, -140)
box16:scale(10,1,10)

return rootnode	