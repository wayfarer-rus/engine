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
   brickSize = 20;
   bricksCount = 33;
   bricksArray = {}
   bricksArray[1] = __loadTexture("resources/textures/brick_texture.png")

   for i = 2, bricksCount do
      bricksArray[i] = __copyTexture(bricksArray[1])
   end

   guyId = __loadTexture("resources/textures/some_guy_2.png")
   -- Apply phisycs
   __applyPhysics(guyId,      -- Id of the texture for physics model
                  4,          -- Mass attribute
                  2,          -- Friction attribute
                  guy_size/2, -- 2D box size width
                  guy_size,   -- 2D box size heihgt
                  "dynamic"   -- Define "static" or "dynamic" body type
   )
   __setPosition(guyId, 0, 0);

   local startPos_x = -320;
   local startPos_y = -180;

   for i =1, bricksCount do
      __applyPhysics(bricksArray[i], 0, 1, brickSize, brickSize, "static")
      __setPosition(bricksArray[i], startPos_x, startPos_y);
      startPos_x = startPos_x + brickSize;
   end

   local loop = true
   while loop do
      local x = os.clock()
      if not redraw() then loop = false end
      processInput()

      -- before __getPosition we need to set it during redraw first.
      -- or initialize before while loop
      guy_x, guy_y = __getPosition(guyId)

      if jumpSequence == jumpingSequenceUp then
         __applyImpulse(guyId, 0, 2000)
         jumpSequence = jumpingSequenceDown
      end

      local vel_x, vel_y = __getBodysVelocity(guyId)
      print(string.format("guy's velocity: [%.2f; %.2f]\n", vel_x, vel_y))
      if vel_y == 0.0 then
         jumpSequence = jumpingSequenceStop
      end
--      for i=1, bricksCount do
--         x, y = __getPosition(bricksArray[i]);
--         io.write(string.format("brick%d [%.2f ; %.2f]\n", i, x, y))
--      end

      __sleepMillis(1/60*1000) -- 1/60 of a second
      print(string.format("elapsed time: %.2f\n", os.clock() - x))
   end
end

function processInput()
   keyboard()
   gamepadButton()
   gamepadAxis()
end

function redraw()
   -- C function!
   __drawTexture(guyId, -- ID of the texture
      guy_size/2, guy_size,   -- zoom value for glScale(x, y, 0)
      guy_x, guy_y, 0,
      angle,0,1,0           -- argments for glRotatef(angle, x, y, z)
   )

   for i =1, bricksCount do
      __drawTextureWithPhysics(bricksArray[i],
                               brickSize, brickSize,
                               0,0,0,0)
   end

   -- C function!
   return __redraw()
end

function keyboard()
   key = __keyboardGetKeyPressed()

   if key == "ESCAPE" then
      __closeWindow()
   elseif key == "RIGHT" then
      __applyImpulse(guyId, 300, 0)
      angle = 0
      if jumpSequence == jumpingSequenceStop then __playSound(stepsSoundFile, sfxVolume) end
   elseif key == "LEFT" then
      __applyImpulse(guyId, -300, 0)
      angle = 180
      if jumpSequence == jumpingSequenceStop then __playSound(stepsSoundFile, sfxVolume) end
   elseif key == "SPACE" and jumpSequence == jumpingSequenceStop then
      jumpSequence = jumpingSequenceUp
      __playSound(jumpSoundFile, sfxVolume)
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
