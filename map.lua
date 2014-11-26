-- red = gr.material({1.0, 0.0, 0.0}, {0.3, 0.3, 0.3}, 25)
-- green = gr.material({0.0, 1.0, 0.0}, {0.3, 0.3, 0.3}, 25)
-- blue = gr.material({0.0, 0.0, 1.0}, {0.3, 0.3, 0.3}, 25)

rootnode = gr.node('maproot')

box = gr.block('box')
rootnode:add_child(box)
box:translate(-5, -15, 0)
box:scale(10,10,10)

box2 = gr.block('box2')
rootnode:add_child(box2)
box2:translate(-5, -11, -20)
box2:scale(10,10,10)

box3 = gr.block('box3')
rootnode:add_child(box3)
box3:translate(-15, -11, -30)
box3:scale(10,30,3)

box4 = gr.block('box4')
rootnode:add_child(box4)
box4:translate(-20, 18, -40)
box4:scale(3,5,25)

return rootnode	