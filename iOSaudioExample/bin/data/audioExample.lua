-- bg = bludGraphics();
-- 
-- function test()
--     local s;
--     s = bludSynth()
--     s:load('low_bell.wav');
--     s:trigger();
-- end
-- 
-- test()
-- 
-- function blud.draw()
--  bg:setColor(0,0,0,255)
--  bg:drawRect(10,10,100,100)
-- end

function blud.touch.down(x, y, id)
	-- s:setNote(y/bg:getHeight()*30+45)
	-- s:trigger();
    print("garbage amount" .. collectgarbage("count"))
	collectgarbage("collect")
end

triggerCallback = function()
    print("garbage amount" .. collectgarbage("count"))
 -- s:trigger()
end

rate         = 44100 * 120 / 60 / 8;     -- takes a rate in bpm
trigger = bludAudioSync(rate);
trigger:setCallback(triggerCallback, 1);


print("loading the code, just proving that print works\n")

-- target code

-- an example that loads two sound files and triggers them in sequence
--[[
synth1 = bludSynth();
synth1.loadWav("bass_drum.wav");
synth1.loop(NO_LOOP);

synth2 = bludSynth();
synth2.loadWav("closed_hh.wav");
synth2.loop(NO_LOOP);

-- kinda like the metro object in max,
-- but linked to the sample rate

]]

