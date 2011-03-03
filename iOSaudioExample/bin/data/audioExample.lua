
bg = bludGraphics();
bg:setColor(0,0,0,255);

function blud.draw()
	bg:drawRect(10,10,100,100);
end

s = bludSynth();
s:load('low_bell.wav');

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

triggerCallback = function(subdivision){
	if(subdivision%8 == 0){
		synth1.trigger();	
	}
	if(subdivision%4 == 0){
		synth4.trigger();	
	}
}
rate 		= 120; 	-- takes a rate in bpm
subdivision = 16; 	-- the number of divisions in a measure, assuming 4/4
trigger = bludAudioSync(triggerCallback, rate, subdivision);

]]

