require "class.lua"

require "Rectangle.lua"

require "Brick.lua"
require "Paddle.lua"

gameFrame = Rectangle(blud.width(), blud.height())

function blud.setup()
  gameFrame.setPosition(0,0)
  createBricks()
  createBalls()
  paddle = Paddle()
end