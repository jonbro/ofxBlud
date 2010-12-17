dofile(blud.bundle_root .. "/test_scripts/breakout/class.lua")
dofile(blud.bundle_root .. "/test_scripts/breakout/Rectangle.lua")

dofile(blud.bundle_root .. "/test_scripts/breakout/Brick.lua")
dofile(blud.bundle_root .. "/test_scripts/breakout/Ball.lua")
dofile(blud.bundle_root .. "/test_scripts/breakout/Paddle.lua")

g = bludGraphics();

gameFrame = Rectangle(g:getWidth(), g:getHeight())

bricks = {};
balls = {};

gameFrame:setPosition(0,0);
paddle = Paddle(gameFrame)

function constrain(val,low,high)
  return math.min(math.max(val,low),high)
end

function createBricks()
  -- BRICK GROUP PROPERTIES --
	numberOfBricks = 60;
	bricksPerRow = 10;
	brickWidth = gameFrame.w/bricksPerRow;
	brickHeight = 10;
	yBricks = 50; 
  rowsColors = {{255,0,255}, {255,0,0}, {255,160,0}, {255,255,0}, {0,255,0}, {0,0,255}};
  numColors = 6;
  for i=0,numberOfBricks,1 do
    rowNum = math.floor(i/bricksPerRow);
    -- coords
	x = brickWidth*i;
    x = x-math.floor(rowNum*bricksPerRow*brickWidth);
    y = yBricks+math.floor(i/bricksPerRow)*brickHeight;
	print(y)
    -- color
    num = math.min(rowNum, numColors);
    rowColor = rowsColors[num];
    -- create brick
    bricks[i] = Brick(brickWidth, brickHeight,x, y, rowsColors[(rowNum%numColors)+1]);
  end
end

function createBalls()
  numberOfBalls = 1;
  yBalls = 150;
  for i=0,numberOfBalls, 1 do
    x = i*20;
    balls[i] = Ball(x, yBalls, 5, 5, gameFrame, bricks, paddle);
  end
end

createBricks()
createBalls()

function blud.draw()
  for i,v in ipairs(bricks) do
    v:refresh()
  end
  for i,v in ipairs(balls) do
    v:refresh()
  end
  paddle:refresh()
end

function blud.touch.touchdown(x, y, id)
  paddle.x = x
end

function blud.touch.touchmoved(x, y, id)
  paddle.x = x
end 