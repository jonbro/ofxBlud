Brick = class(function(brick, w, h, x, y, color)
	rect = Rectangle(w,h);
	rect:setPosition(x,y);
	rect:setColor(color[1], color[2], color[3])
	brick.rect = rect
	-- brick.color = color;
	brick.frame = 0;
	brick.alive = true;
	brick.respawns = false;
	brick.timeToRespawn = 500;
end)

function Brick:refresh() 
	if self.alive then
	  self.rect:draw();
	else
	  if self.respawns then
		self.frame = self.frame + 1 
		if self.frame>self.timeToRespawn then
		  -- rise up from your grave, brick
		  imAlive=true;
		end
	  end
	end
end
function Brick:die()
  self.alive = false;
  self.frame = 0;
end
