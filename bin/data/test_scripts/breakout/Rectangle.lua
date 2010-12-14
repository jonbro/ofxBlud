bg = bludGraphics();

Vector2d = class(function(vec,x,y)
  vec.x = x
  vec.y = y
end)

Line2d = class(function(line,x1,y1,x2,y2)
  line.v1 = Vector2d(x1,y1)
  line.v2 = Vector2d(x2,y2)
end)

-- http://www.angelfire.com/fl/houseofbartlett/solutions/lineinter2d.html
function Line2d:check_tri_clock_dir(v1,v2,v3)
  test = (((v2.x - v1.x)*(v3.y - v1.y)) - ((v3.x - v1.x)*(v2.y - v1.y)));
  if (test > 0) then return 0 -- counter clockwise
  elseif(test < 0) then return 1 -- clockwise
  else return 3 end -- line
end

function Line2d:check_intersection(l2)
  test1_a = self:check_tri_clock_dir(self.v1,self.v2,l2.v1)
  test1_b = self:check_tri_clock_dir(self.v1,self.v2,l2.v2)
  if(test1_a ~= test1_b) then  
    test2_a = self:check_tri_clock_dir(l2.v1,l2.v2,self.v1)
    test2_b = self:check_tri_clock_dir(l2.v1,l2.v2,self.v2)
    if(test2_a ~= test2_b) then
      return true
    end
  end
  return false
end

-- Rectangle Class
Rectangle = class(function(rect, w, h)
  rect.w = w
  rect.h = h
  rect.x1 = 0
  rect.y1 = 0  
  rect.x2 = 0
  rect.y2 = 0  
  rect.red = 0;
  rect.green = 0;
  rect.blue = 0;
end)

function Rectangle:setPosition(x,y)
  self.x1 = x
  self.y1 = y
  self.x2 = x+self.w
  self.y2 = y+self.h
end

function Rectangle:setColor(r,g,b)
	self.red = r;
	self.green = g;
	self.blue = b;
end

function Rectangle:draw()
	bg:setColor(self.red,self.green,self.blue,255);
--	bg:setColor(0,0,255);
	bg:drawRect(self.x1,self.y1,self.w,self.h);
end

function Rectangle:doesPointTouch(v)
  if (v.x >= self.x1 and v.x <= self.x2) then
    if (v.y >= self.y1 and v.y <= self.y2) then
      return true
    end
  end
  return true
end

function Rectangle:whatSideDoesLineTouch(line, velocity)
  -- check the top and bottom intersections
  if velocity.y > 0 then
    side = Line2d(self.x1,self.y1,self.x2,self.y2)
    if(line.check_intersection(side)) then
      return 1
    end
  elseif velocity.y < 0 then
    side = Line2d(self.x1,self.y2,self.x2,self.y2)
    if(line.check_intersection(side)) then
      return 3
    end
  end 
  -- check the left and right intersections
  if velocity.x > 0 then
    side = Line2d(self.x1,self.y1,self.x1,self.y2)
    if(line.check_intersection(side)) then
      return 4
    end
  elseif velocity.x < 0 then
    side = Line2d(self.x2,self.y2,self.x2,self.y2)
    if(line.check_intersection(side)) then 
      return 2
    end
  end 
  return 0
end