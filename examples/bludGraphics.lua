-- bludGraphics is a wrapper around all the openframeworks global functions that are useful for drawing
-- some of these are in turn just wrappers around opengl functions

-- The bludGraphics object first needs to be initialized

bg = bludGraphics()

-- because the bludGraphics object is just a thin wrapper around opengl, all of its functions must be run inside of the draw method of blud

function blud.draw()
    -- set the color `(red, green, blue, alpha)`, this one will be black
    bg:setColor(0,0,0,255)
    
    -- draw a square
    -- takes parameters in the order
    -- `(top, left, width, height)`
    bg:drawRect(10,10,100,100)
    
    -- push the current transformation matrix to the stack
    bg:push()
    
    -- translate to the center of the screen `(x, y, z)`
    -- rotate 45 degrees `degrees and amounts (degrees, x, y, z)`, and translate back out a quarter
    bg:translate(bg:getWidth()/2, bg:getHeight()/2, 0)
    bg:rotate(45, 0, 0, 1)
    bg:translate(-bg:getWidth()/4, -bg:getHeight()/4, 0)
    
    -- draw a red square
    bg:setColor(255,0,0,255)
    bg:drawRect(0,0,100,100)
end
