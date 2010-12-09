-- test

-- Make sure love table exists.
if not blud then blud = {} end

-- Used for setup:
blud.path = {}
blud.arg = {}
blud.touch = {}

-- Unparsed arguments:
argv = {}

function blud.update(dt)
	-- do nothing when initially launching
end

function blud.draw()
	-- do nothing when initially launching
end

function blud.touch.touchdown(x, y, id)
	print("touched")
	-- do nothing when initially launching
end
function blud.touch.touchmoved(x, y, id)
	print("touch moved")
	-- do nothing when initially launching
end
function blud.touch.touchup(x, y, id)
	print("touch up")
	-- do nothing when initially launching
end
