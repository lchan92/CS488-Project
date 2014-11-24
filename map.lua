red = gr.material({1.0, 0.0, 0.0}, {0.3, 0.3, 0.3}, 25)
blue = gr.material({0.0, 0.0, 1.0}, {0.3, 0.3, 0.3}, 25)

rootnode = gr.node('maproot')

box = gr.block('box')
box:set_material(red)
rootnode:add_child(box)
-- box:translate(-5, -11, 0)
box:translate(-5, -15, 0)
box:scale(10,10,10)

box2 = gr.block('box2')
box2:set_material(blue)
rootnode:add_child(box2)
box2:translate(-5, -11, -20)
box2:scale(10,10,10)

return rootnode	