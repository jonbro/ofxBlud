-- test

-- Make sure love table exists.
if not blud then blud = {} end

-- Used for setup:
blud.path = {}
blud.arg = {}
blud.mouse = {}
blud.key = {}

-- Unparsed arguments:
argv = {}

function blud.update(dt)
	-- do nothing when initially launching
end

function blud.draw()
	-- do nothing when initially launching
end

function blud.key.pressed(key)
	print("key pressed")
	-- do nothing when initially launching
end
function blud.key.released(key)
	print("key released")
	-- do nothing when initially launching
end

function blud.mouse.moved(x, y)
	print("mouse moved")
	-- do nothing when initially launching
end
function blud.mouse.dragged(x, y, button)
	print("mouse dragged")
	-- do nothing when initially launching
end
function blud.mouse.pressed(x, y, button)
	print("mouse pressed")
	-- do nothing when initially launching
end
function blud.mouse.released(x, y, button)
	print("mouse released")
	-- do nothing when initially launching
end