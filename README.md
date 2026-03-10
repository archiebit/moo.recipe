# moo.recipe

My attempt to create a silly build system. Just for fun.

> [!NOTE]
> Under construction!




## Code draft

This code example is just a draft for me.
```ini
# Super useful comment.
[config]
name = "fancy configuration name"
list = "game.exe util.exe"


[recipe]
name    = "executable"
parent  = "fancy configuration name"
pattern = "*.exe"
output  = "out/${file}"
objects = prepare "src/*.cc"
command = "clang++ -o ${output} ${objects}"


[recipe]
name    = "object file"
parent  = "executable program"
pattern = "src/*.cc"
output  = "obj/${file:name}.o"
command = "clang++ -o ${output} 'src/${file}'"
```