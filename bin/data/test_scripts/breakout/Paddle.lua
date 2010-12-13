Paddle = class(function(paddle)
  self.width = 60
  self.height = 5
  hasStroke = false
  hasFill = true
  self.rectangle = Rectangle()
end)

function Rectangle:setPosition(x,y)
  self.x1 = x
  self.y1 = y
  self.x2 = x+self.w
  self.y2 = y+self.h
end
