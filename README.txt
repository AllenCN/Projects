This is a collection of projects made in Visual Studio Community 2015. Some things may not work as expected. 

Currently, there are the following projects in this git repository:

TextLineGameThing - The first version of a command line game I made based off of
these tutorials: http://cplussplussatplay.blogspot.com/2012/11/text-adventure-games-c-part-1.html
I made my own story and build, since I wanted to incorporate combat and leveling mechanics.

TextLineGameThing2 - The second version of a command line game I made based off the tutorials above.
Here, I made everything a class object, so that I could better use this as a base engine for a game I 
truly wanted to make. As such, the game data is relegated to text files. Future versions may attempt
to make use of XML or JSON files to better store the data. SQL has also been considered for data storage.

YuGiOhConsoleCalc - In the same vein as the above, I wished to make a version of my go-to for programming 
practice that used the command line for all its events. This would keep track of not only the remaining
life points of each player, but how the life points changed and what turn it was. This also outputs the
LP change to the console and a file. Of note is the fact the console text changes color to display the 
players' "health bars"; I may just use this technique in my next iteration of TextLineGameThing to make 
physical life/mana/EXP meters.