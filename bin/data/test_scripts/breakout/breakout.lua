dofile(blud.bundle_root .. "/test_scripts/breakout/class.lua")
dofile(blud.bundle_root .. "/test_scripts/breakout/Rectangle.lua")

dofile(blud.bundle_root .. "/test_scripts/breakout/Brick.lua")
dofile(blud.bundle_root .. "/test_scripts/breakout/Paddle.lua")

g = bludGraphics();

gameFrame = Rectangle(g:getWidth(), g:getHeight())
bricks = {};

gameFrame:setPosition(0,0);

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

createBricks()

--  createBalls()
--  paddle = Paddle()

function blud.draw()
  for i,v in ipairs(bricks) do
    v:refresh()
  end
end