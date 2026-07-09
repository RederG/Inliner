# Inliner

## Intro

The "Inliner" removes all inline charaters ("\n"), tabulations (2/3/4/... spaces) in a file and stores the result in another file.

The result will be a long text in one line

## Ho to use

First of all, this tool is a command line tool. So, you need a terminal to use it.

After you opened a terminal, call the name of the tool : `inline`.

Then specify one of the parameters :
```
-src <file_path>
```

or 

```
-out <file_path>
```

After that, change "file_path" to the path of the file you want to inline/store the result.

At last, you should have the following command (as an example) :
```
inline -src file.html -out result.html
```

By default, the number of tab's spaces is 4.

You can specify the number of spaces of the tabulations by adding the parameter : 
```
-tab-spaces <spaces_number>
```

Here, the "spaces_number" will be a number that indicates the number of spaces a tab has.

In the end, you should have (for example) : 
```
inline -src file.css -out result.css -tab-spaces 4
```

If you need more helps, you can specify the parameter : 
```
-help
```

So, you can have this :
```
inline -help
```

## Utility

It can be used to inline all web files (HTML, CSS, JavaScript, etc) for the micro-controllers. It is usefull not to remove all tabulations or inline when copy/paste the code from one editor to one another.

It is also usefull when we want to spare some time when loading pages, or spare some memory for the micro-controllers which has limited memory space.

Other usages can be listed but I only found these for now.

## Building the project

If you want to build the project, you must have the g++ compiler installed on your machine.

After that, go to the folder containing the source files (all ".cpp" and ".hpp" files).

Then, execute the following command :
```
g++ *.cpp -o inline
```

## So, enjoy the tool !!!