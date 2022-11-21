return [==[
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
   "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
<meta http-equiv="Content-Type" content="text/html; charset=$(ldoc.doc_charset)"/>
<head>
    <title>$(ldoc.title)</title>
    <link rel="stylesheet" href="$(ldoc.css)" type="text/css" />
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Rubik:wght@300&display=swap" rel="stylesheet">

    <!-- Meta stuff -->
    <meta property="og:title" content="M2TWEOP LuaPlugin Documentation">
    <meta property="og:description" content="Documentation, tutorials and example code for M2TWEOP's LuaPlugin.">
    <meta property="og:image" content="https://i.imgur.com/mQIK1oo.png">

    <!-- Favicon-->
   <link rel="icon" type="image/x-icon" href="https://i.imgur.com/mQIK1oo.png">


# if ldoc.custom_css then -- add custom CSS file if configured.
    <link rel="stylesheet" href="$(ldoc.custom_css)" type="text/css" />
# end
</head>
<body>

<div id="container">

<div id="main">

# local no_spaces = ldoc.no_spaces
# local use_li = ldoc.use_li
# local display_name = ldoc.display_name
# local iter = ldoc.modules.iter
# local function M(txt,item) return ldoc.markup(txt,item,ldoc.plain) end
# local nowrap = ldoc.wrap and '' or 'nowrap'

<!-- Menu -->

<div id="navigation">
<br/>

# if ldoc.body then -- verbatim HTML as contents; 'non-code' entries
    <h1><a href="../../../index.html">Home</a></h1>
    <h1><a href="../index.html">Back</a></h1>
# elseif module then
    <h1><a href="../../index.html">Home</a></h1>
# end


# if not ldoc.single and module then -- reference back to project index
<ul>
  <li><a href="../$(ldoc.output).html">Index</a></li>
</ul>
# end

# --------- contents of module -------------
# if module and not ldoc.no_summary and #module.items > 0 then
<h2>Data Types</h2>
<ul>

# for kind,items in module.kinds() do
<details>
  <summary><a href="#$(no_spaces(kind))">$(kind)</a></summary>
# for item in items() do
    <li><a href="#$(item.name)">$(display_name(item))</a></li>
# end
</details>


# end
</ul>
# end
</div>

<div id="content">
    <div class="welcomeDiv">
        <h2 class="bannerText"></h2>

        <h2 class="bannerText">Medieval 2: Total War - Engine Overhaul Project Lua Plugin Documentation</h2>
        <h3 class="welcomeMessage">Welcome to the EOP Lua Plugin Documentation!</h3>
        <br>
        <h3 class="disclaimerMessage">Please note that many of the examples require additional checks and not all functions are fully documented. If you need additional help, we welcome you to join the Discord!</h3>
        <p class="lastUpdatedMessage">Last updated: $(ldoc.updatetime)</p>
        <a target="_blank" href="https://discord.gg/Epqjm8u2WK">
            <img class="discordImage" src="https://img.shields.io/discord/713369537948549191?color=282828&label=DISCORD&style=for-the-badge"/>
        </a>
        <br>
    <div class="quickLinks">
        <h2>Quick Links</h2>

# if ldoc.body then
    <ul>
        <li><a href="../../../articles.html">M2TWEOP Tutorials</li>
    <ul>
        <li><a href="https://www.youtube.com/watch?v=hUvbcz8i7Wk">Scripting Basics (Video Tutorial)</li>
        <li><a href="../../../_static/Introduction_to_ImGui_v3.pdf">Introduction to ImGUI</a></li>
    <ul>
    </ul>
# elseif module then
       <ul>
        <li><a href="../../articles.html">M2TWEOP Tutorials</li>
    <ul>
        <li><a href="https://www.youtube.com/watch?v=hUvbcz8i7Wk">Scripting Basics (Video Tutorial)</li>
        <li><a href="../../_static/Introduction_to_ImGui_v3.pdf">Introduction to ImGUI</li>
        <li><a href="../../_static/LuaLib/extra/readme_imgui.md.html">EOP's ImGUI Bindings</li>
    <ul>
    </ul>
# end
</div>

# if ldoc.body then -- verbatim HTML as contents; 'non-code' entries
    $(ldoc.body)
# elseif module then -- module documentation
<p class="moduleSummary">$(M(module.summary,module))</p>
<p class="moduleDescription">$(M(module.description,module))</p>
#   if module.tags.include then
        $(M(ldoc.include_file(module.tags.include)))
#   end
#   if module.see then
#     local li,il = use_li(module.see)
    <h3>See also:</h3>
    <ul>
#     for see in iter(module.see) do
         $(li)<a href="$(ldoc.href(see))">$(see.label)</a>$(il)
#    end -- for
    </ul>
#   end -- if see
#   if module.usage then
#     local li,il = use_li(module.usage)
    <h3>Usage:</h3>
    <ul>
#     for usage in iter(module.usage) do
        $(li)<pre class="example">$(ldoc.escape(usage))</pre>$(il)
#     end -- for
    </ul>
#   end -- if usage
</div>


# if not ldoc.no_summary then
# -- bang out the tables of item types for this module (e.g Functions, Tables, etc)
# for kind,items in module.kinds() do
<h2><a href="#$(no_spaces(kind))">$(kind)</a></h2>
<table class="function_list">
#  for item in items() do
	<tr>
	<td class="name" $(nowrap)><a href="#$(item.name)">$(display_name(item))</a></td>
	<td class="summary">$(M(item.summary,item))</td>
	</tr>
#  end -- for items
</table>
#end -- for kinds

<br/>
<br/>

#end -- if not no_summary

# --- currently works for both Functions and Tables. The params field either contains
# --- function parameters or table fields.
# local show_return = not ldoc.no_return_or_parms
# local show_parms = show_return
# for kind, items in module.kinds() do
#   local kitem = module.kinds:get_item(kind)
#   local has_description = kitem and ldoc.descript(kitem) ~= ""
    <h2 class="section-header $(has_description and 'has-description')"><a name="$(no_spaces(kind))"></a>$(kind)</h2>
    $(M(module.kinds:get_section_description(kind),nil))
#   if kitem then
#       if has_description then
          <div class="section-description">
          $(M(ldoc.descript(kitem),kitem))
          </div>
#       end
#       if kitem.usage then
            <h3>Usage:</h3>
            <pre class="example">$(ldoc.prettify(kitem.usage[1]))</pre>
#        end
#   end
    <dl class="function">
#  for item in items() do
    <dt>
    <a name = "$(item.name)"></a>
    <strong>$(display_name(item))</strong>
#   if ldoc.prettify_files and ldoc.is_file_prettified[item.module.file.filename] then
    <a style="float:right;" href="$(ldoc.source_ref(item))">line $(item.lineno)</a>
#  end
    </dt>
    <dd>
    <div class="functionDescription">
    $(M(ldoc.descript(item),item))
    </div>
#   if ldoc.custom_tags then
#    for custom in iter(ldoc.custom_tags) do
#     local tag = item.tags[custom[1]]
#     if tag and not custom.hidden then
#      local li,il = use_li(tag)
    <h3>$(custom.title or custom[1]):</h3>
    <ul>
#      for value in iter(tag) do
         $(li)$(custom.format and custom.format(value) or M(value))$(il)
#      end -- for
#     end -- if tag
    </ul>
#    end -- iter tags
#   end

#  if show_parms and item.params and #item.params > 0 then
#    local subnames = module.kinds:type_of(item).subnames
#    if subnames then
    <h3>$(subnames):</h3>
#    end
    <ul>
#   for parm in iter(item.params) do
#     local param,sublist = item:subparam(parm)
#     if sublist then
        <li><span class="parameter">$(sublist)</span>$(M(item.params.map[sublist],item))
        <ul>
#     end
#     for p in iter(param) do
#        local name,tp,def = item:display_name_of(p), ldoc.typename(item:type_of_param(p)), item:default_of_param(p)
        <li><span class="parameter">$(name)</span>
#       if tp ~= '' then
            <span class="types">$(tp)</span>
#       end
        $(M(item.params.map[p],item))
#       if def == true then
         (<em>optional</em>)
#      elseif def then
         (<em>default</em> $(def))
#       end
#       if item:readonly(p) then
          <em>readonly</em>
#       end
        </li>
#     end
#     if sublist then
        </li></ul>
#     end
#   end -- for
    </ul>
#   end -- if params

#  if show_return and item.retgroups then local groups = item.retgroups
    <h3>Returns:</h3>
#   for i,group in ldoc.ipairs(groups) do local li,il = use_li(group)
    <ol>
#   for r in group:iter() do local type, ctypes = item:return_type(r); local rt = ldoc.typename(type)
        $(li)
#     if rt ~= '' then
           <span class="types">$(rt)</span>
#     end
        $(M(r.text,item))$(il)
#    if ctypes then
      <ul>
#    for c in ctypes:iter() do
            <li><span class="parameter">$(c.name)</span>
            <span class="types">$(ldoc.typename(c.type))</span>
            $(M(c.comment,item))</li>
#     end
        </ul>
#    end -- if ctypes
#     end -- for r
    </ol>
#   if i < #groups then
     <h3>Or</h3>
#   end
#   end -- for group
#   end -- if returns

#   if show_return and item.raise then
    <h3>Raises:</h3>
    $(M(item.raise,item))
#   end

#   if item.see then
#     local li,il = use_li(item.see)
    <h3>See also:</h3>
    <ul>
#     for see in iter(item.see) do
         $(li)<a href="$(ldoc.href(see))">$(see.label)</a>$(il)
#    end -- for
    </ul>
#   end -- if see

#   if item.usage then
#     local li,il = use_li(item.usage)
    <h3>Usage:</h3>
    <ul>
#     for usage in iter(item.usage) do
        $(li)<pre class="example">$(ldoc.prettify(usage))</pre>$(il)
#     end -- for
    </ul>
#   end -- if usage

</dd>
# end -- for items
</dl>
# end -- for kinds

# else -- if module; project-level contents

# if ldoc.description then
  <h2>$(M(ldoc.description,nil))</h2>
# end
# if ldoc.full_description then
  <p>$(M(ldoc.full_description,nil))</p>
# end

# for kind, mods in ldoc.kinds() do
<h2>$(kind)</h2>
# kind = kind:lower()
<table class="module_list">
# for m in mods() do
	<tr>
		<td class="name"  $(nowrap)><a href="$(no_spaces(kind))/$(m.name).html">$(m.name)</a></td>
		<td class="summary">$(M(ldoc.strip_header(m.summary),m))</td>
	</tr>
#  end -- for modules
</table>
# end -- for kinds
# end -- if module

</div> <!-- id="content" -->
</div> <!-- id="main" -->
<div id="about">
</div> <!-- id="about" -->
</div> <!-- id="container" -->
</body>
</html>
]==]

