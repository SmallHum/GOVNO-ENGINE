# GOVNO-ENGINE
My attempt at video game engines using SFML. Pretty crappy.

# Struct tree

There's a struct tree. I'm stealing it from Godot Engine.
I'm gonna list some details about the structs cause you can play with Nodes in the console.
All of this information is only for use in the console.

Node - basic struct that can have children and a parent.

construction: ```string name```

icon: ```.```

avaliable variables:

```string name```
```bool visible``` - determines if this tree should be drawn.
```bool active``` - determines if this tree should be processed.

Spatial - inherits Node, has a spatial transform. It's spatial children recursively inherit its parent's transform.

icon: ```└```

avaliable variables:

```vec2f pos```
```float angle``` - 
```vec2f scale``` - determines if this tree should be processed.

That's it for now.

# Console
It's a simple console.
by default, you can toggle it by pressing ```F9```.
DON'T CLOSE IT WITH ```X``` IT'S GONNA CLOSE THE SFML WINDOW TOO

BTW unfortunately, spaces aren't avaliable. That's why commands are typed with ```-```.

commands list:

```help```
A little description for every command. There's less information, look it up here.

```print-tree```
Prints the node tree. Testing command.

```find-node [path]```
Finds a node. Testing command.

Example:

Y root
  Y child_1
    Y child_3
  Y child_2

```find-node "child_1/child_3"```
The console will tell that it found node ```child_3``` successfully.
```find-node "child_3"```
The console will throw an error, saying that the path is invalid. There's no
```child_3``` in the root node, it's in ```child_1```.



```make-node [path] [class] [arg 1] [arg 2] ...```
Creates a node of type ```[class]```, adding it as a child to node ```[path]```
with ```[arg 1] [arg 2] ...``` as construction arguments.

Example:

```make-node "" Node test-node```
This will create a node called ```test-node``` in the ```root``` node.
```make-node "test-node" Node test-child```
This will create a node ```test-child``` in ```test-node```



```delete-node [path]```
Delets a node in the ```[path]```



```info [path]```
Shows avaliable variables of a node and it's type.
Avaliable variables are listed above in the struct tree section.


```edit [path] [var name] [new value]```
Edit node's avaliable variables.


```debug [option]```
Currently this command allows you to switch flags that are responsible for displaying debug data.
You can type ```-``` to switch all of them.
Flags include:

```show-fps``` - Show FPS at the top-left side of the screen.
```show-spatial-origin``` - Show X and Y axies of a Spatial transform.
```show-spatial-name``` - Show Spatial's name next to it's position.
```show-picked-node-info``` - Show info of a Node found via ```info``` command. The difference is that it's in real time.


```cd [path]```
CMD change directory, just for the node tree.