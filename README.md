# fsimilar

## fsimilar is a tool that allows you to find similar files on your pc.

### Running fsimilar:

##### $> export PATH=./:$PATH
##### $> fsimilar
###### This command will show you available arguments.

### Example usages
##### fsimilar -s assignment_tb.exe -d assignment.exe -t ff
##### fsimilar -s assignment_tb.exe -d ./ -t fp -m 95
##### fsimilar -s assignment_tb.exe -d / -t fp -m 20 (Dangerous)
##### fsimilar -s assignment_tb.exe -d / -t fp -m 80 (Less Dangerous)
##### fsimilar -s ../ -d ./ -t pp

### TODO List
##### Further optimizations.
##### Include/Exclude feature
##### Exclude exactly same files(same absolute path) from results
##### Quick search feature causes segmentation fault on too many recursive calls.


### Usage: fsimilar -s {source} -d {destination} -t {type} -m {similarity} {-q}

##### {source} is the base path for file/directory
##### {destination} is the path for comparing
##### {similarity} is the minimum similarity to print similar files
##### {type} is for to compare what to what? {ff, fp, pp}
##### {-q} enables quick search (available for file_path)

