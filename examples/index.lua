-- #ofxBlud
-- ## a set of lua wrappers around a chunk of openframeworks

-- the core of blud more or less maps to the way that openframeworks is layed out
-- with the following additional components:
-- 
-- *    [bludGraphics](bludGraphics.html) a wrapper around some of the opengl parts of openframeworks. Useful for drawing rectangles.
-- *    [bludImage](bludImage.html) for drawing texture maps, or parts of texturemaps. Has some very basic sprite support.
-- *    [bludSynth](bludSynth.html) a sample based synthesizer
-- *    [bludAudioSync](bludAudioSync.html) an audio syncronized event system

-- you can override any of the app callbacks as follows

-- blud update recieves a variable that corrosponds to how much time has passed since the last update
function blud.update(dt)
end

-- the same as the openframeworks draw function
function blud.draw()
end

-- all of the touch functions receive three parameters
function blud.touch.down(x, y, id)
end
function blud.touch.moved(x, y, id)
end
function blud.touch.up(x, y, id)
end
function blud.touch.double_tap(x, y, id)
end
