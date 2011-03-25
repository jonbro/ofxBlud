-- need an instance of blud graphics so that we can get information about screen size
bg = bludGraphics();

-- setup a new sprite sheet with 1 layer, 10000 tiles per layer, default layer of 0, tile size of 32
sheet = bludSpriteSheet(1, 10000, 0, 32);

-- load the texture onto the sheet
-- takes the following parameters `(filename, sheetsize)`. The sheet must be a power of 2, and square.
sheet:loadTexture("spriteSheetExample.png", 256)

-- setup an empty table to hold all of the sprites
sprites = {}

function blud.update(t)
    -- remove all of the sprites that we have added to the sheet during the last frame
    sheet:clear()
    -- update the sheet so that all of the animations play properly
    sheet:update(t)
    
    -- sort the table so that the lower sprites are drawn last
    table.sort(sprites, function(a, b) return a.y<b.y end)
    
    -- loop through the sprite table
    for i, v in ipairs(sprites) do
        -- update the vertical position of the sprite
        v.y = v.y+v.speed
        
        if(v.y > bg:getHeight+16)
            -- if the sprite is off the screen, remove it from the array
            v = nil
        else
            -- otherwise, add it to the sheet that is about to be drawn
            sheet:addCenteredTile(v.sprite, v.x, v.y)
        end
    end
    
    -- every frame add a whole bunch of sprites so that the screen fills with an army
    for i=1,10 do
        
        -- blud sprite is a container for the animation info that the spritesheet needs to render. It doesn't contain information about where to render on the screen though, so we need to store some additional information in the table
        sprite = bludSprite();
                
        -- the animation length in frames
        sprite:setTotalFrames(4)
        
        -- the number of tiles each animation frame is in width and height
        sprite:setWidth(1)
        sprite:setHeight(1)
        
        -- how long each frame displays for
        sprite:setFrameDuration(75)
        
        -- how many times the animation plays
        sprite:setLoops(-1)
                
        -- start the sprite at a random x position, and somewhere off the screen
        sprites.insert({sprite=sprite, x=math.random(0, bg:getWidth()),y=-16, speed=math.random(1,5)});
        
    end
    sheet:addSprite(sprites)
end

function blud.draw()
    -- draws all the sprites that have been added to the sheet during the update function
    sheet:draw()
end