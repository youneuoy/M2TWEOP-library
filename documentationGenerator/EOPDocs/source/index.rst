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
	<p>The M2TW Engine Overhaul Project (M2TWEOP) is designed to expand the capabilities of the strategy game Medieval 2: Total War. EOP uses C++ and Assembly to modify the game's code in-memory and exposes this to the end user via a Lua API.</p>

	<h2>Features</h2>
	<ul>
		<li>
			<strong><a href="https://youneuoy.github.io/M2TWEOP-library/_static/LuaLib/index.html">Lua scripting system with 1000+ functions</a></strong>
			<ul>
				<li>Full documentation with code examples and VSCode Intellisense</li>
				<li>Seamless integration with existing campaign_script scripts</li>
				<li>Hot-reload</li>
				<li>Custom console</li>
				<li>Debugging support (VSCode)</li>
				<li><a href="https://github.com/ocornut/imgui">ImgGui bindings</a></li>
			</ul>
		</li>
		<li>
			Removes engine limitations for
			<ul>
				<li>Number of unit types and unit sizes</li>
				<li>Number of religions and cultures</li>
				<li>Number of buildings and their chains</li>
				<li>Number of ancillaries</li>
				<li>Number of custom settlements</li>
				<li>Number of custom cas models (characters and settlements)</li>
			</ul>
		</li>
		<li>Unlocks all vanilla console commands</li>
		<li>
			Fixes many engine bugs, crashes, and oversights such as
			<ul>
				<li>Berserker units instantly crashing</li>
				<li>Elephant units breaking sieges</li>
				<li>Elephant cannons unable to attack gates</li>
			</ul>
		</li>
		<li>Draw cas models at any scale, anywhere on the campaign map</li>
		<li>Fully customizable and configurable launcher</li>
		<li>Discord Rich Presence support for 30+ mods</li>
		<li><a href="https://www.youtube.com/watch?v=RrGi4zxr7bU">Rome: Total War style Tactical Map Viewer</a></li>
		<li>Play out Hotseat battles online and transfer the results back to the campaign map</li>
		<li>Custom keybinding support</li>
		<li>Play custom sounds or music with support for WAV, OGG/Vorbis, and FLAC</li>
		<li>Supports both disk (1.5) and Steam (1.52) versions</li>
	</ul>

	<h3>Breaking Engine Limits</h3>
	<pre class="type">
	<span class="comment">-- &lt;YOUR_MOD_FOLDER&gt;/eopData/eopScripts/luaPluginScript.lua</span>
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

	<h2>Videos</h2>
	<div class="videoGallery">
	<iframe frameborder="0" src="https://www.youtube.com/embed/itTng85dG5I" allowfullscreen></iframe>
	<iframe frameborder="0" src="https://www.youtube.com/embed/caOiB0NaGGI" allowfullscreen></iframe>
	<iframe frameborder="0" src="https://www.youtube.com/embed/RrGi4zxr7bU" allowfullscreen></iframe>
	</div>

.. toctree::
	:maxdepth: 2

	faq
	articles
	mainInfo


