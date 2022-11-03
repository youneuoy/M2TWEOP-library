# EOP Documentation Generator

To generate the docs, simply run the following command

`./generateDocs.ps1`

# Lua Plugin
To edit the CSS of the Lua Plugin, navigate to
`EOP-Theme/ldoc.css`

To edit the HTML template of the Lua Plugin, navigate to
`./luarocks/lua/ldoc/html/ldoc_ltp.lua`

# How the docs are generated
In `config.ld`, a few CPP files are referenced. In these CPP files, functions are annotated with the following
```
--- Summary ends with a period.
-- Some description, can be over several lines.
-- @param p1 first parameter
-- @param p2 second parameter
-- @return a string value
-- @see second_fun
function mod1.first_fun(p1,p2)
end
```
This is how each function is described in the documentation and how it is iterated through, in order (according to the order which the files are read in `config.ld`)