Paddle = class(function(paddle,frame)
  paddle.width = 60
  paddle.height = 5
  paddle.x = frame.w/2-30
  paddle.rectangle = Rectangle(paddle.width,paddle.height)
  paddle.rectangle:setPosition(frame.w/2-30,450)
end)

function Paddle:refresh()
  self.rectangle:setPosition(self.x, 450)
  self.rectangle:draw()
end