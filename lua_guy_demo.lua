function mainLoop()
   -- Here we suppose that GLFW, ALUT, etc. already initialized
   -- So, let's get right into action
   jumpMaxValue = 200
   jumpingSequenceStop, jumpingSequenceUp, jumpingSequenceDown = 0,1,2
   guy_x = 0.0;
   guy_y = 0.0;
   guy_size = 70;
   angle = 0;
   jumpSequence = jumpingSequenceStop
   -- Sounds
   stepSoundFile = __loadSoundFile("resources/sound_sfx/guy_step.wav")
   stepsSoundFile = __loadSoundFile("resources/sound_sfx/guy_steps.wav")
   jumpSoundFile = __loadSoundFile("resources/sound_sfx/guy_jump.wav")
   sfxVolume = 100
   -- Load guy's texture into game
   -- C function!
   textureID = __loadTexture("resources/textures/some_guy.png")

   local loop = true
   while loop do
      if jumpSequence == jumpingSequenceUp then
         guy_y = guy_y + 3.0
      elseif jumpSequence == jumpingSequenceDown then
         guy_y = guy_y - 3.0
      end

      if guy_y < 0.0 then
         jumpSequence = jumpingSequenceStop
         guy_y = 0.0
         __playSound(stepSoundFile, sfxVolume)
      elseif (guy_y >= jumpMaxValue) and (jumpSequence ~= jumpingSequenceStop) then
         jumpSequence = jumpingSequenceDown
      end

      if not redraw() then loop = false end
      processInput()
      __sleepMillis(100)
   end
end

function processInput()
   keyboard()
   gamepadButton()
   gamepadAxis()
end

function redraw()
   -- C function!
   __drawTexture(textureID, -- ID of the texture
      guy_size, guy_size,   -- zoom value for glScale(x, y, 0)
      -guy_x,100-guy_y,0,   -- offset for glTranslate
      angle,0,1,0           -- argments for glRotatef(angle, x, y, z)
   )

   -- C function!
   return __redraw()
end

function keyboard()
   key = __keyboardGetKeyPressed()

   if key == "ESCAPE" then
      __closeWindow()
   elseif key == "RIGHT" then
      guy_x = guy_x + 5
      angle = 0
      if jumpSequence == jumpingSequenceStop then __playSound(stepsSoundFile, sfxVolume) end
   elseif key == "LEFT" then
      guy_x = guy_x - 5
      angle = 180
      __playSound(stepsSoundFile, sfxVolume)
   elseif key == "SPACE" and jumpSequence == jumpingSequenceStop then
      jumpSequence = jumpingSequenceUp
      if jumpSequence == jumpingSequenceStop then __playSound(stepsSoundFile, sfxVolume) end
   end
end

-- In this demo we will ignore gamepad's ID. We'll always work with ID = 0
function gamepadButton()
   if __gamepadButtonPressed() and jumpSequence == jumpingSequenceStop then
      jumpSequence = jumpingSequenceUp
      __playSound(jumpSoundFile, sfxVolume)
   end
end

function gamepadAxis()
   axisValue = __gamepadGetAxisValue()
   guy_x = guy_x + 5*axisValue

   if axisValue < 0 then
      angle = 180
      if jumpSequence == jumpingSequenceStop then __playSound(stepsSoundFile, sfxVolume) end
   elseif axisValue > 0 then
      angle = 0
      if jumpSequence == jumpingSequenceStop then __playSound(stepsSoundFile, sfxVolume) end
   end
end

print("Priming run")
