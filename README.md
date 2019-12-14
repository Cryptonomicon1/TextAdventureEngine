*Text Adventure RPG Engine*

Right now, the code is crude, but I have the very basic part working.

My todo list is to add a messenger bus. I don't plan to use a
component-entity-system pattern. Later, I might make a 2d game
engine with a CES pattern and messenger bus.

I was thinking that no object will have a pointer to another object, except handlers will keep a hash table on thier object. I think I will have a rooms handler with a hash table of the id numbers. The rooms handler will offer id numbers to any requester on the messenger bus. The handler will access the rooms through their public functions. However, the handler will have a gtMssg function to determine which room object to address, and what to do about it. I think a global static enum class will give each class on the messenger bus a class number.

I also plan to create a better parser and linker for input.

I plan to add some basic rpg elements to this engine.

I plan to come back and break up my classes further.

I am thinking about redoing my trigger codes for the input game files. I may writer a parser-linker for those.

*Basic Syntax Guidelines*
I don't always follow these guidelines. However...

Classes and Files are in upper camel case ApplePie
Functions are in lower camel case and start with a verb moveApplePie
variables and structs are all lower but spaced with underscores apple_count

I eliminated vowels to save screenspace

I will later put a comment at the top for every pre-definition to offer at least one plain english definition of my abridged names.
