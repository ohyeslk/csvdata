# csvdata
This is a terminal-based program to handle csv-file.

### Compile:
You can simply type "make" to compile, but note that you must have a g++ compiler which<br>
surport C++11 to compile this program.

### Usage:
This command goes as follows

Note that all number appears in this program is 0-indexed!!

./csvdata [-p (colume numbers)] [-c (expression)] [-o (output file-name)] [-s (colume numbers)] <br>
             [-j (colume on join)] [-O] [-h] [file-name which be joined with] input-file-name<br>
             
I'll explain each option in the following "Features Section"            

### Features:
1. Select some certain line using -s, you can either use ',' saperated notation or '-' notation.<br>
For example, if the original csv file is:

sample.csv:

1,3,4,5  <br>
2,3,1,2  <br>
2,6,2,1  <br>

And you want to select colume 0,1,2<br>
You can either do ./csvdata -s 0-2 -o output.txt sample.csv<br>
               or ./csvdata -s 0,1,2 -o output.txt sample.csv<br>
  
Note that if you don't use -o to specified a name the default output file name will be csvdata.out

2. Perform arithmetic operations and create a new colume.
For example:<br>

sample.csv:

1,3,4,5  <br>
2,3,1,2  <br>
2,6,2,1  <br>

If you do ./csvdata -c 0+2 -o output.txt sample.csv<br>
Then it will add each number in colume 0 with each number in colume 2 and create a new colume.<br>
The resule will be:

line   0      1      2      3    new<br>
0:     1      3      4      5  5.000000<br>
1:     2      3      1      2  3.000000<br>
2:     2      6      2      1  4.000000<br>

Also note that to multiply it's col1xcol2 instead of col1*col2

3. Output statistics on a column: min, max, median and average<br>
You can either do ./csvdata -p 0-2 -o output.txt sample.csv<br>
               or ./csvdata -p 0,1,2 -o output.txt sample.csv<br>

The result of the statistics of selected colume will goes to standard output.

4. Perform an outer and inner join on two data sets<br>
You can use -j colnumber option to do join using colnumber as a key.<br>
If you want to do outer join, you should specify using -O options.<br>
If not specified, the default way is inner join.<br>

For example:<br>
If you want to inner join f1.csv and f2.csv, using the 0th colume as key word, do:<br>
./csvdata -j 0 -o output.txt f1.csv f2.csv<br>

outer join:<br>
./csvdata -j 0 -o output.txt -O f1.csv f2.csv

5. Other features:<br>
You can use -h to specify if the csv file has a header line at the top.<br>
If not, csvdata will auto-add names using 0-index colume number as the name.<br>



