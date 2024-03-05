# Upgrading to EOP V3

The only major difference between EOP v2 and EOP v3 Lua scripts is that any [EOP Events](https://youneuoy.github.io/M2TWEOP-library/_static/LuaLib/index.html#EOP_Events) now export `eventData` instead of individual parameters like `selectedChar` or `settlement`

Let's look at an example using `onCharacterSelected`

## EOP V2

```lua
function onCharacterSelected(selectedChar) 
    print(selectedChar.fullName)
end 
```

## EOP V3

```lua
function onCharacterSelected(eventData)
    local selectedChar = eventData.character
    print(selectedChar.fullName)
end
```

For any other changes (of which there are very very few), please see the [EOP V3 release notes](https://github.com/youneuoy/M2TWEOP-library/releases/tag/M2TWEOP-3.0.0)
