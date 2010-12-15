Ball = class(function(ball, x, y, w, h, frame, bricks,paddle)
  ball.rect = Rectangle(w,h);
	ball.rect:setPosition(x,y);
	ball.rect:setColor(255, 255, 255)
	ball.velX = 3
	ball.velY = 3
	ball.x = x
	ball.y = y
	ball.height = h
	ball.width = w
	ball.px = x
	ball.py = y
	
	ball.frame = frame
	ball.bricks = bricks
	ball.paddle = paddle
	
  ball.xcentre = ball.x+ball.width/2;
  ball.ycentre = ball.y+ball.height/2;
end)

function Ball:refresh()
  self:update()
  self.rect:setPosition(self.x,self.y)
  self.rect:draw()
end

function Ball:update()
  -- add velocity to position
  self.x=self.x+self.velX;
  self.y=self.y+self.velY;
  
  -- collision with limits
  if(self.x<=0 or self.x>=self.frame.w-self.width) then
    self.velX = -self.velX;
    self.x = constrain(self.x, 0, self.frame.w-self.width);
  end
  if(self.y<=0 or self.y>=self.frame.h-self.height) then
    self.velY = -1 * self.velY;
    self.y = constrain(self.y, 0, self.frame.h-self.height);
  end
  
  self.xcentre = self.x+self.width/2;
  self.ycentre = self.y+self.height/2;
  result = 0
  -- collision with paddle
  result = self:checkCollisionWithRectangle(self.paddle.rectangle);
  -- if collides on top, control direction of ball
  if (result == 1) then
      if (self.xcentre < self.paddle.rectangle.x1+self.paddle.rectangle.w/2) then
        if (self.velX>0) then
          self.velX = -self.velX;
        end
      else
        if (self.velX<0) then
          self.velX = -self.velX;  
        end
      end
  end
  
  -- collision with bricks
  if (result == 0) then
    for i,brick in ipairs(bricks) do
      if brick.alive then
        res = self:checkCollisionWithRectangle(brick.rect);
        if res ~= 0 then
          brick:die()
          break
        end
      end
    end
  end
  self.px = self.x;
  self.py = self.y;
end

function Ball:checkCollisionWithRectangle(R)
  result = 0;
  if (R:doesPointTouch(Vector2d(self.xcentre, self.ycentre))) then
    -- which side did it collide
    lineaBola = Line2d(self.xcentre,self.ycentre,self.px+self.width/2,self.py+self.height/2);
    result = R:whatSideDoesLineTouch(lineaBola, Vector2d(self.velX, self.velY))
    -- top
    if (result==1) then
      self.velY = -1*self.velY;
      self.y = R.y1-self.height;
      -- right
    elseif (result==2) then
      self.velX = -self.velX;
      self.x = R.x2;
      -- bottom
    elseif(result==3) then
      self.velY = -self.velY;
      self.y = R.y2;
      -- left
    elseif(result==4) then
      self.velX = -self.velX;
      self.x = R.x1-self.width;
    else
      result = 5;
    end
  end
  return result
end