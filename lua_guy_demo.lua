function mainLoop()
   -- Here we suppose that GLFW, ALUT, etc. already initialized
   -- So, let's get right into action
   jumpMaxValue = 100
   jumpingSequenceStop, jumpingSequenceUp, jumpingSequenceDown = 0,1,2
   guy_x = 0.0;
   guy_y = 0.0;
   guy_size = 70;
   jumpSequence = jumpingSequenceStop
   -- Load guy's texture into game
   -- C function!
   textureID = __loadTexture("resources/some_guy.png")

   local loop = true
   while loop do
      if jumpSequence == jumpingSequenceUp then
         guy_y = guy_y + 3.0
      elseif jumpSequence == jumpingSequenceDown then
         guy_y = guy_y - 3.0
      end

      if guy_y <= 0.0 then
         jumpSequence = jumpingSequenceStop
         guy_y = 0.0
      elseif (guy_y >= jumpMaxValue) and (jumpSequence ~= jumpingSequenceStop) then
         jumpSequence = jumpingSequenceDown
      end

      if not redraw() then loop = false end
      processInput()
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
      guy_size,           -- zoom value for glScale
      -guy_x,100-guy_y,0  -- offset for glTranslate
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
   elseif key == "LEFT" then
      guy_x = guy_x - 5
   elseif key == "SPACE" and jumpSequence == jumpingSequenceStop then
      jumpSequence = jumpingSequenceUp
   end
end

-- In this demo we will ignore gamepad's ID. We'll always work with ID = 0
function gamepadButton()
   if __gamepadButtonPressed() and jumpSequence == jumpingSequenceStop then
      jumpSequence = jumpingSequenceUp
   end
end

function gamepadAxis()
   axisValue = __gamepadGetAxisValue()
   guy_x = guy_x + 5*axisValue
end

print("Priming run")
