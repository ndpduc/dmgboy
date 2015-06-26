# HOW TO TRANSLATE DMGBOY #

DMGBoy can be translated to any language thanks to the use of wxWidgets. The
only thing you need is an application compatible with gettext, for example
Poedit. The next lines explains how to translate DMGBoy with Poedit.

## POEDIT ##

You can download Poedit from: http://poedit.net/
<br>
Install it.<br>
<br>
<h2>START A TRANSLATION TO A NEW LANGUAGE</h2>

Open Poedit and select "File > New from POT/PO file". Select the dmgboy.pot<br>
file. You can find it in the source code of DMGBoy. For example:<br>
<br>
<code>C:\DMGBoy\src\Languages\dmgboy.pot</code>
<br>
Enter the language of the new translation.<br>
Translate the strings and save the file as a .po. Remember to leave the \n<br>
character at the same place the original is.<br>
<br>
<h2>UPDATE AN EXISTING TRANSLATION</h2>

Follow the above section but select a dmgboy.po intead the dmgboy.pot. You can<br>
find some translations in the "Languages" directory inside the source code.<br>
<br>
<h2>UPDATE AN EXISTING TRANSLATION WITH LATEST STRINGS UPDATES</h2>

Open a dmgboy.po of any language. Select "Catalog > Update from POT file" and<br>
select the dmgboy.pot. If some string was missing in the translation, it will be<br>
added.<br>
<br>
<h2>TEST IT</h2>

After saving the translation, Poedit puts two files in the destination<br>
directory. A .po file and a .mo file. The .po is used to make new changes to the<br>
translation and the .mo is a binary file and is the one that really translates<br>
the application. So you can replace an existing .mo to try your new translation.<br>
Once installed the application, each .mo is in a folder with a language code as<br>
the name. These are the places where they are:<br>
<ul><li>Windows: "languages" directory inside the install path.<br>
</li><li>Linux: /usr/share/DMGBoy/languages<br>
</li><li>Mac: DMGBoy.app/Contents/Resources