red = gr.material({1.0, 0.0, 0.0}, {0.3, 0.3, 0.3}, 25)

rootnode = gr.node('maproot')

box = gr.cube('box')
box:set_material(red)
rootnode:add_child(box)
box:translate(-5, -15, 0)
box:scale(10,10,10)

return rootnode