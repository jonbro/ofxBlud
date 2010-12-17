dofile(blud.bundle_root .. "/test_scripts/include_test/class.lua")
dofile(blud.bundle_root .. "/test_scripts/include_test/Rectangle.lua")

test_rect = Rectangle(100,100);
test_rect:setPosition(100,100);

function blud.draw()
  test_rect:draw()
end