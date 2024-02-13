.. M2TWEOP documentation master file, created by
   sphinx-quickstart on Thu Apr 22 15:18:38 2021.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

M2TWEOP: Medieval 2 Engine Overhaul Project
===================================


.. raw:: html

	<div align="center">
		<a target="_blank" href="https://www.twcenter.net/forums/forumdisplay.php?2296-M2TW-Engine-Overhaul-Project"><img src="_static/M2TWEOP.png" class="eopBanner" alt="EOP" /></a>
		<br>
		<br>
	  <p>
	  <div  class="imageDiv">
	  	<a target="_blank" href="https://www.twcenter.net/forums/forumdisplay.php?2296-M2TW-Engine-Overhaul-Project"><img src="_static/twc.png" alt="TWC" width="200" height="70"/></a>
		<a target="_blank" href="https://discord.gg/Epqjm8u2WK"><img src="_static/discorsd.png" alt="Discord server" width="200" height="70"></a>
		<a target="_blank" href="https://www.youtube.com/channel/UCMyHomaKeeGR4ZPGrBo9dYw"><img src="_static/youtube.png" alt="YouTube" width="200" height="70"/></a>
	  </p>
	  </div>
	<p>M2TW Engine Overhaul Project is designed to expand the capabilities of the game Medieval 2: Total War.
	<br>
	EOP uses C++ and Assembly to modify the game's code in-memory and exposes this to the end user via a Lua API and the <a href="./_static/LuaLib/index.html">EOP Lua Plugin</a></p>
	</div>

	<h2>Features</h2>
	<ul>
		<li>Hugely expanded Lua scripting system that integrates seamlessly with existing campaign_script scripts, hot-reload, new custom console, debugger and much more</li>
		<li>Breaks the EDU limit allowing for more than 500 units in a mod</li>
		<li>Place .fbx models anywhere on the strategy or battle map with full animation and texture support</li>
		<li>Render .cas models for characters, settlements, forts, or anywhere on the map based on any kind of condition you can imagine</li>
		<li>Play custom sounds or music with support for WAV, OGG/Vorbis and FLAC</li>
		<li>Unlocks all vanilla console commands</li>
		<li>Rome: Total War style <a href="https://www.youtube.com/watch?v=RrGi4zxr7bU">tactical map viewer</a></li>
		<li>Create your own GUIs and overlays using the popular <a href="https://github.com/ocornut/imgui">ImGUI</a></li>
		<li>New custom EDU system that allows the creation and modification of new and existing units without the need to restart the game</li>
		<li>Play out Hotseat battles online and transfer the results back to the campaign map</li>
		<li>Set your own limits for the number of religions, ancillaries, bodyguard units, building chains, max unit size</li>
		<li>Set the boundaries of the number of soldiers in units</li>
		<li>Fixed many engine bugs, crashes and oversights (e.g crashes when using berserkers in battle)</li>
		<li>Edit the .worldpkgdesc for any settlement on the fly without the need to restart the game</li>
		<li>Custom keybinding support</li>
		<li>Added ability to write your own add-ons in C++. The library exports many different functions.</li>
		<li>Utilize the packaged <strong>M2TWEOP GUI</strong> launcher with full customization support as well as the included <strong>EOP Health Checker</strong> or just allow your mod to be launched directly</li>
		<li>Many, many more!</li>
	</ul>

	<h3>Breaking Engine Limits</h3>
	<pre class="type">
	<span class="comment">-- &lt;YOUR_MOD_FOLDER&gt;/youneuoy_Data/plugins/lua/luaPluginScript.lua</span>
	<span class="kr">function</span><span class="p"> </span><span class="nf">onPluginLoad()</span>
	  <span class="n">M2TWEOP</span><span class="p">.</span><span class="f">unlockGameConsoleCommands</span><span class="p">();</span>
	  <span class="n">M2TWEOP</span><span class="p">.</span><span class="f">setAncillariesLimit</span><span class="p">(</span><span class="mi">16</span><span class="p">);</span>
	  <span class="n">M2TWEOP</span><span class="p">.</span><span class="f">setMaxBgSize</span><span class="p">(</span><span class="mi">100</span><span class="p">);</span>
	  <span class="n">M2TWEOP</span><span class="p">.</span><span class="f">setReligionsLimit</span><span class="p">(</span><span class="mi">50</span><span class="p">);</span>
	  <span class="n">M2TWEOP</span><span class="p">.</span><span class="f">setBuildingChainLimit</span><span class="p">(</span><span class="mi">40</span><span class="p">);</span>
	  <span class="n">M2TWEOP</span><span class="p">.</span><span class="f">setGuildCooldown</span><span class="p">(</span><span class="mi">3</span><span class="p">);</span>
	<span class="p"></span><span class="kr">end</span><span class="p"></span>
	</pre>

	<h3>Playing sounds, displaying GUIs and spawning units</h3>
	<pre class="type">
	<span class="kr">function</span> <span class="nf">onGeneralAssaultsGeneral</span><span class="p">(</span><span class="m">eventData</span><span class="p">)</span>
		<span class="comment">-- Check if the attacker is a Nazgul</span>
		<span class="kr">if</span><span class="p">(</span><span class="n">hasTrait</span><span class="p">(</span><span class="m">attacker</span><span class="p">,</span> <span class="s2">"NazgulRace"</span><span class="p">))</span> <span class="kr">then</span>
			<span class="comment">-- Play a sound</span>
			<span class="n">playSound</span><span class="p">(</span><span class="m">nazgulScream</span><span class="p">);</span>
			<span class="comment">-- Display some text</span>
			<span class="n">showWindow</span><span class="p">(</span><span class="s2">"The Nazgul have arrived."</span>,<span class="s2">"Nazgul_Arrived.png"</span>,<span class="m"> NAZGUL_ARRIVAL_TEXT</span><span class="p">);</span>
			<span class="comment">-- Spawn a new unit</span>
			<span class="n">spawnUnit</span><span class="p">(</span><span class="s2">"Citadel Guard"</span><span class="p">,</span> <span class="s2">"Anorien"</span><span class="p">,</span><span class="s2">"Minas Tirith"</span><span class="p">, </span><span class="mi">3</span><span class="p">, </span><span class="mi">4</span><span class="p">, </span><span class="mi">1</span><span class="p">, </span><span class="mi">1</span><span class="p">);</span>
		<span class="kr">end</span>
	<span class="kr">end</span>
	</pre>

	<h3>Creating custom UI's with M2TWEOP's ImGUI Lua Plugin</h3>
	<a target="_blank" href="https://cdn.discordapp.com/attachments/1116459429039194152/1157017402769428501/mercenaryViewer.jpg?ex=65d78eed&is=65c519ed&hm=7dd51841c000f30bdd44450303c8c2cefc8564fd84667974c298a947e8b81165&"><img class="imGuiImage" src="https://cdn.discordapp.com/attachments/1116459429039194152/1157017402769428501/mercenaryViewer.jpg?ex=65d78eed&is=65c519ed&hm=7dd51841c000f30bdd44450303c8c2cefc8564fd84667974c298a947e8b81165&"></a>
	<a target="_blank" href="https://cdn.discordapp.com/attachments/943211834947829800/1077682337166393344/options.jpg"><img class="imGuiImage" src="https://cdn.discordapp.com/attachments/943211834947829800/1077682337166393344/options.jpg"></a>
	<a target="_blank" href="https://cdn.discordapp.com/attachments/1116459429039194152/1154107279931609168/Alpha_1.jpg?ex=65d63329&is=65c3be29&hm=f8047c3c13b89b493f36b71822b00025edc3f84abca1a98541c88523056e10b7&"><img class="imGuiImage" src="https://cdn.discordapp.com/attachments/1116459429039194152/1154107279931609168/Alpha_1.jpg?ex=65d63329&is=65c3be29&hm=f8047c3c13b89b493f36b71822b00025edc3f84abca1a98541c88523056e10b7&"></a>
	<a target="_blank" href="https://cdn.discordapp.com/attachments/939640870343426059/1110700778101944340/image.png?ex=65d52c35&is=65c2b735&hm=1c51d43f47b5d62c6270e76429ccf8ebb3b84297a69cb0b03f9655e4203b8ac7&"><img class="imGuiImage" src="https://cdn.discordapp.com/attachments/939640870343426059/1110700778101944340/image.png?ex=65d52c35&is=65c2b735&hm=1c51d43f47b5d62c6270e76429ccf8ebb3b84297a69cb0b03f9655e4203b8ac7&"></a>
	<a target="_blank" href="https://cdn.discordapp.com/attachments/939640870343426059/1112032699616608337/image.png?ex=65da04a8&is=65c78fa8&hm=15296d6fee0d613411450ad094eb37be9db83d5f677ed4c0c5af62cd5fd0385b&"><img class="imGuiImage" src="https://cdn.discordapp.com/attachments/939640870343426059/1112032699616608337/image.png?ex=65da04a8&is=65c78fa8&hm=15296d6fee0d613411450ad094eb37be9db83d5f677ed4c0c5af62cd5fd0385b&"></a>

	<h2>Videos</h2>
	<div class="videoGallery">
	<iframe frameborder="0" src="https://www.youtube.com/embed/itTng85dG5I" allowfullscreen></iframe>
	<iframe frameborder="0" src="https://www.youtube.com/embed/caOiB0NaGGI" allowfullscreen></iframe>
	<iframe frameborder="0" src="https://www.youtube.com/embed/RrGi4zxr7bU" allowfullscreen></iframe>
	<iframe frameborder="0" src="https://www.youtube.com/watch?v=Vbpqde_YI68" allowfullscreen></iframe>

	<a target="_blank" href="https://www.youtube.com/watch?v=hUvbcz8i7Wk">
	<img src="https://i.imgur.com/ByebdSI.png">
	</a>
	</div>

.. toctree::
	:maxdepth: 2

	faq
	articles
	mainInfo


