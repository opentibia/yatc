These files were not tested for installation with Dev-Cpp's .devpak installer. 


However I suspect that SDL devpak won't work out of the box. YATC assumes the 
files are in include/SDL folder, and this installer installs files directly
into include/ folder.


I recommend you MANUALLY install at least SDL. Rename .devpak into .tar.bz2
and proceed with manual installation of include/ subfolder in .tar.bz2 into 
* C:\MinGW\include\SDL or 
* C:\Dev-Cpp\include\SDL or 
* C:\Program Files\CodeBlocks\MinGW\include\SDL

Perhaps I'll later on look for better SDL devpak but atm this is mainly for
my convenience and not yours ;)

Ivan Vucica