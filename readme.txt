a few lua wrappers.

not much, but it is enough.

make sure to include the libs.

the callbacks that you can attach to in your lua are as follows:

function blud.update(time)
function blud.draw()

function blud.mouse.moved(x, y)
function blud.mouse.dragged(x, y, button)
function blud.mouse.pressed(x, y, button)
function blud.mouse.released(x, y, button)