import 'CoreLibs/graphics.lua'

print("Figure out my code and I'll give you a flag!")
print("Turn the crank to reset the pin. ")

local gfx = playdate.graphics
playdate.display.setRefreshRate(50)

gfx.setBackgroundColor(gfx.kColorWhite)

function generateFlag()
  local flag = ""
  for i = 1, 64 do
    flag = flag .. string.char(math.random(32, 126))
  end
  return flag
end

function makeTextDisplayable(text)
  local displayableText = ""
  for i = 1, #text do
    local char = text:sub(i, i)
    if char == "_" then
      displayableText = displayableText .. "__"
    elseif char == "*" then
      displayableText = displayableText .. "**"
    else
      displayableText = displayableText .. char
    end
  end
  return displayableText
end

noiseSeed = 1234567
math.randomseed(noiseSeed)

pressedButtons = ""
counter = 0
buttons = { "left", "right", "up", "down", "A", "B" }

function generateOrder()
  local pinSeed = ""
  for i = 1, 20 do
    pinSeed = pinSeed .. i
  end
  return pinSeed
end

function playdate.cranked(change, acceleratedChange)
  print("Cranked! " .. change .. " " .. acceleratedChange)
  pressedButtons = ""
  counter = 0
end

function clean(input)
  -- Split the number into pairs, then ensure the minimum is 65 and the max is 90
  local cleaned = ""
  for i = 1, #input, 2 do
    local pair = input:sub(i, i + 1)
    local num = tonumber(pair)
    num = num % 26 + 65
    cleaned = cleaned .. string.char(num)
  end
  return cleaned
end

index = ""
lastPressed = "Press a button!"

function playdate.update()
  -- Setup base text
  gfx.clear(gfx.kColorWhite)
  counter = counter + 1
  gfx.drawTextAligned(generateFlag(), 400 / 2, 240 / 2, kTextAlignment.center)

  for i = 1, #buttons do
    if playdate.buttonJustPressed(buttons[i]) then
      pressedButtons = pressedButtons .. i
      lastPressed = buttons[i]
    end
  end

  gfx.drawTextAligned(lastPressed, 400 / 2, 240 / 2 + 40, kTextAlignment.center)
  gfx.drawTextAligned(makeTextDisplayable(pressedButtons), 400 / 2, 240 / 2 + 60, kTextAlignment.center)
  gfx.drawTextAligned("Rotate the crank to reset the challenge.", 400 / 2, 240 / 2 + 80, kTextAlignment.center)
  if pressedButtons == generateOrder() then
    print("Pin entered correctly!")
    gfx.setFont(gfx.font.kVariantBold)
    cleaned = clean(pressedButtons)
    print("Flag: sun{" .. cleaned .. "}")
    gfx.drawTextAligned("Flag: \nsun{" .. cleaned .. "}", 400 / 2, 240 / 3, kTextAlignment.center)
  end
end
