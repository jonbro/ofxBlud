-- #bludImage
-- bludImage is an extremely basic wrapper around ofImage. Allows for an image to be loaded and drawn.

-- initialize an instance
image = bludImage();

-- load an image in the instance
-- this path should be in the data folder
image:load('test.jpg')

-- all the drawing functions should take place within the blud draw loop
function blud.draw()

    -- draw the image at `(x, y)` without scaling
    image:draw(0, 0)
    -- draw the image with scaling `(x, y, width, height)`. It should be noted that `image:width()` and `image:height()` return the width and height of the image respectively.
    image:drawScale(0,0, image:width()/2, image:height()/2)
    
end