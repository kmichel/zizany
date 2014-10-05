# Zizany - Unity binary assets parser

Parse and extract information from Unity binary assets files, for debugging, diffing or mere curiosity.

## Installation

### Requirements

 - cmake
 - clang++ or g++

### Instructions

```shellsession
$ cmake .
$ make
$ sudo make install
```

## Commands

### Dump file content as a json object

```shellsession
$ zizany dump [-tarpdlm] <filename>
```

#### Options :
 - `-t`, `--types`  
     Dump structure of the stored types.
 - `-a`, `--assets`  
     Dump value of the stored assets.
 - `-r`, `--file-references`  
     Dump value of the file references.
 - `-p`, `--previews`  
     Dump value of the stored previews.
 - `-d`, `--type-defaults`  
     Dump type attributes even if they are equal to their default value.
 - `-l`, `--file-layout`  
     Dump attributes related to the internal layout of the file.
 - `-m`, `--magic`  
     Dump attributes whose meaning is not yet known.
### Extract previews as png files

```shellsession
$ zizany extract_previews [-o <path>] [<filename> ...]
```

#### Options :
 - `-o <path>`, `--output_dir=<path>`  
     Select a directory where preview files will be saved.  
     The directory will be created if it does not already exist.  
     Defaults to the current directory.

## Using zizany as a git filter

If for some reason you have binary assets in your source repository,
you can configure git to use zizany as a text filter and display human readable diffs.

In `.gitattributes`:

```gitattributes
*.asset diff=zizany
```

In `.git/config`:

```ini
[diff "zizany"]
    textconv = zizany dump -tarp
    binary = true
```

## License

Code released under the MIT License.
